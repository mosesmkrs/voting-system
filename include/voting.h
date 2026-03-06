#ifndef VOTING_H
#define VOTING_H

#include <stdio.h>
#include <pthread.h>

#define VOTER_FILE "data/voters.dat"
#define CANDIDATE_FILE "data/candidates.dat"
#define POSITION_FILE "data/positions.dat"
#define VOTES_LOG_FILE "data/votes_log.dat"

extern pthread_mutex_t file_mutex;



typedef struct
{
    char studentID[20];
    int posID;
} VoteRecord;

typedef struct
{
    int id;
    char title[50]; // e.g., "President", "Secretary General"
} Position;

typedef struct
{
    int id;
    int posID; // Links to Position.id
    char name[50];
    int voteCount;
} Candidate;

typedef struct
{
    char studentID[20];
    int hasVoted;
} Voter;

void init_system();
void cleanup_system();
void addPosition();
void registerCandidate();
void registerVoter();
void castVote();
void announceResults();

#endif