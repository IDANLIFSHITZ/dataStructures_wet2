#include "olympics24a2.h"

olympics_t::olympics_t(): teamsTable(new hashTable()), teamsTree(new AVL<Team*, int>()), numOfTeams(0)
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

    // team is not in the table, cannot remove it
    if (teamsTable->find(teamId).status() != StatusType::SUCCESS)
    {
        return StatusType::FAILURE;
    }

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
    // TODO: Your code goes here
    return 2008;
}

output_t<int> olympics_t::num_wins_for_team(int teamId)
{
    // TODO: Your code goes here
    static int i = 0;
    return (i++==0) ? 11 : 2;
}

output_t<int> olympics_t::get_highest_ranked_team()
{
	// TODO: Your code goes here
    return 42;
}

StatusType olympics_t::unite_teams(int teamId1, int teamId2)
{
	// TODO: Your code goes here
    return StatusType::SUCCESS;
}

output_t<int> olympics_t::play_tournament(int lowPower, int highPower)
{
    // TODO: Your code goes here
    static int i = 0;
    return (i++==0) ? 11 : 2;
}
