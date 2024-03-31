#include "olympics24a2.h"
#include "Pair.h"

/*
 * private functions:
 */

void olympics_t::increase_win(Team* team, int change)
{
    this->teamsTree->update_extra(Pair<int,int>(team->getStrength().ans(), team->getId()), change);
    int teamRank = this->teamsTree->get_subtreeSize(Pair<int,int>(team->getStrength().ans(), team->getId()));
    auto selectRank = this->teamsTree->search_subtreeSize(teamRank-1);
    if (selectRank.status() == StatusType::SUCCESS)
    {
        this->teamsTree->update_extra(Pair<int,int>(selectRank.ans()->getStrength().ans(), selectRank.ans()->getId()), -change);
    }
}

/*
 * public functions:
 */

olympics_t::olympics_t(): teamsTable(new hashTable()), teamsTree(new AVL<Team*, Pair<int,int>>()), numOfTeams(0)
{

}

olympics_t::~olympics_t()
{
    // deletes the teams competing
    teamsTree->clear();

    // deletes the array of trees and their nodes
    delete teamsTable;

    // deletes the tree and its nodes
    delete teamsTree;
}


StatusType olympics_t::add_team(int teamId)
{
    if (teamId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    if (teamsTable->find(teamId).status() == StatusType::SUCCESS)
    {
        return StatusType::FAILURE;
    }
    try
    {
        Team* team = new Team(teamId);
        StatusType status = teamsTable->add(team, teamId);
        if (status != StatusType::SUCCESS)
        {
            return status;
        }
        status = teamsTree->insert(team, teamId);
        if (status != StatusType::SUCCESS)
        {
            return status;
        }
        numOfTeams++;
    }
    catch (const std::bad_alloc& e)
    {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType olympics_t::remove_team(int teamId)
{
    // invalid input
    if (teamId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }

    // find the team to delete
    output_t<Team*> out = teamsTable->find(teamId);

    // team is not in the table, cannot remove it
    if (out.status() != StatusType::SUCCESS)
    {
        return StatusType::FAILURE;
    }

    Team* teamToDelete = out.ans();

    // remove the team from the table
    StatusType status = teamsTable->remove(teamId);
    if (status != StatusType::SUCCESS)
    {
        return status;
    }

    // remove the team from the tree
    status = teamsTree->remove(teamId);
    if (status != StatusType::SUCCESS)
    {
        return status;
    }

    // free the players of the team
    delete teamToDelete;

    numOfTeams--;

    return StatusType::SUCCESS;
}

StatusType olympics_t::add_player(int teamId, int playerStrength)
{
    // invalid input
    if (teamId <= 0 || playerStrength <= 0)
    {
        return StatusType::INVALID_INPUT;
    }

    // check if the team with the given id exists
    output_t<Team*> out = teamsTable->find(teamId);
    if (out.status() != StatusType::SUCCESS)
    {
        return StatusType::FAILURE;
    }

    // the team we need to add to
    Team* teamToAddPlayer = out.ans();

    // add the player to the team
    StatusType status = teamToAddPlayer->addPlayer(playerStrength);
    if (status != StatusType::SUCCESS)
    {
        return status;
    }

	return StatusType::SUCCESS;
}

StatusType olympics_t::remove_newest_player(int teamId)
{
    // invalid input
    if (teamId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }

    // check if the team with the given id exists
    output_t<Team*> out = teamsTable->find(teamId);
    if (out.status() != StatusType::SUCCESS)
    {
        return StatusType::FAILURE;
    }

    // the team we need to remove the player from
    Team* teamToRemovePlayer = out.ans();

    // remove the player from the team
    StatusType status = teamToRemovePlayer->removeNewestPlayer();
    if (status != StatusType::SUCCESS)
    {
        return status;
    }
	return StatusType::SUCCESS;
}

output_t<int> olympics_t::play_match(int teamId1, int teamId2)
{
    if (teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2)
    {
        return StatusType::INVALID_INPUT;
    }
    output_t<Team*> outTeam1 = teamsTable->find(teamId1);
    output_t<Team*> outTeam2 = teamsTable->find(teamId2);
    if (outTeam1.status() != StatusType::SUCCESS || outTeam2.status() != StatusType::SUCCESS)
    {
        return StatusType::FAILURE;
    }
    Team* team1 = outTeam1.ans();
    Team* team2 = outTeam2.ans();
    if (team1->getNumOfPlayers() == 0 || team2->getNumOfPlayers() == 0)
    {
        return StatusType::FAILURE;
    }
    int team1Strength = team1->getStrength().ans() * team1->getNumOfPlayers();
    int team2Strength = team2->getStrength().ans() * team2->getNumOfPlayers();

    if (team1Strength > team2Strength)
    {
        // TODO: increase win count for team1
        return teamId1;
    }
    else if (team1Strength < team2Strength)
    {
        // TODO: increase win count for team2
        return teamId2;
    }
    else
    {
        int idOfTeamWithLowerId = team1->getId() > team2->getId() ? teamId2 : teamId1;
        // TODO: increase win count for idOfTeamWithLowerId
        return idOfTeamWithLowerId;
    }
}

output_t<int> olympics_t::num_wins_for_team(int teamId)
{
    if (teamId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }

    auto team = this->teamsTable->find(teamId);
    if (team.status() != StatusType::SUCCESS)
    {
        return team.status();
    }
    return this->teamsTree->calc_extra_in_path(Pair<int,int>(team.ans()->getStrength().ans(), team.ans()->getId()));
}

output_t<int> olympics_t::get_highest_ranked_team()
{
	if (this->teamsTable->isEmpty())
    {
        return -1;
    }
    if (this->teamsTree->get_size() == 0)
    {
        return 0;
    }
    return output_t<int>(this->teamsTree->get_maxSubtreeRank());
}

StatusType olympics_t::unite_teams(int teamId1, int teamId2)
{
    // invalid input
    if (teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2)
    {
        return StatusType::INVALID_INPUT;
    }
    output_t<Team*> outTeam1 = teamsTable->find(teamId1);
    if (outTeam1.status() != StatusType::SUCCESS)
    {
        return StatusType::FAILURE;
    }

    output_t<Team*> outTeam2 = teamsTable->find(teamId2);
    if (outTeam2.status() != StatusType::SUCCESS)
    {
        return StatusType::FAILURE;
    }
    Team* team1 = outTeam1.ans();
    Team* team2 = outTeam2.ans();

    team1->uniteTeams(team2);
    StatusType status =  teamsTable->remove(teamId2);
    if (status != StatusType::SUCCESS)
    {
        return status;
    }
    status = teamsTree->remove(teamId2);
    if (status != StatusType::SUCCESS)
    {
        return status;
    }
    numOfTeams--;

    return StatusType::SUCCESS;
}

output_t<int> olympics_t::play_tournament(int lowPower, int highPower)
{
    // TODO: Your code goes here
    static int i = 0;
    return (i++==0) ? 11 : 2;
}
