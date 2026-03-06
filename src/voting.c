#include "../include/voting.h"
#include <string.h>
#include <stdlib.h>

pthread_mutex_t file_mutex;

void init_system() { pthread_mutex_init(&file_mutex, NULL); }
void cleanup_system() { pthread_mutex_destroy(&file_mutex); }

void addPosition()
{
    FILE *fp = fopen(POSITION_FILE, "ab+");
    if (!fp)
        return;
    fseek(fp, 0, SEEK_END);
    Position p;
    p.id = (ftell(fp) / sizeof(Position)) + 1;
    printf("Enter Position Title (e.g. President): ");
    scanf(" %[^\n]s", p.title);
    fwrite(&p, sizeof(Position), 1, fp);
    fclose(fp);
    printf("Position added with ID: %d\n", p.id);
}

void registerCandidate()
{
    FILE *pfp = fopen(POSITION_FILE, "rb");
    if (!pfp)
    {
        printf("Add positions first!\n");
        return;
    }

    Position p;
    printf("\nAvailable Positions:\n");
    while (fread(&p, sizeof(Position), 1, pfp))
        printf("%d. %s\n", p.id, p.title);
    fclose(pfp);

    Candidate c;
    printf("Enter Position ID to vie for: ");
    scanf("%d", &c.posID);
    printf("Enter Candidate Name: ");
    scanf(" %[^\n]s", c.name);

    FILE *cfp = fopen(CANDIDATE_FILE, "ab+");
    fseek(cfp, 0, SEEK_END);
    c.id = (ftell(cfp) / sizeof(Candidate)) + 1;
    c.voteCount = 0;
    fwrite(&c, sizeof(Candidate), 1, cfp);
    fclose(cfp);
}

void registerVoter()
{
    FILE *fp = fopen(VOTER_FILE, "ab");
    if (!fp)
        return;
    Voter v;
    printf("Enter Student ID: ");
    scanf("%s", v.studentID);
    v.hasVoted = 0;
    fwrite(&v, sizeof(Voter), 1, fp);
    fclose(fp);
    printf("Voter registered successfully.\n");
}

// Helper function to check if this student already voted for this specific position
int hasAlreadyVotedInRace(char *sid, int pid)
{
    FILE *fp = fopen(VOTES_LOG_FILE, "rb");
    if (!fp)
        return 0;

    VoteRecord vr;
    while (fread(&vr, sizeof(VoteRecord), 1, fp))
    {
        if (strcmp(vr.studentID, sid) == 0 && vr.posID == pid)
        {
            fclose(fp);
            return 1; // Already voted in this specific race
        }
    }
    fclose(fp);
    return 0;
}

void castVote()
{
    char sid[20];
    printf("Enter Student ID: ");
    scanf("%s", sid);

    // 1. Basic Voter Verification
    FILE *vfp = fopen(VOTER_FILE, "rb+");
    if (!vfp)
        return;
    Voter v;
    int foundV = 0;
    long vPos;
    while (fread(&v, sizeof(Voter), 1, vfp))
    {
        if (strcmp(v.studentID, sid) == 0)
        {
            foundV = 1;
            vPos = ftell(vfp) - (long)sizeof(Voter);
            break;
        }
    }
    fclose(vfp); // Closing for now to avoid locking issues during the loop

    if (!foundV)
    {
        printf("Not registered.\n");
        return;
    }

    int sessionActive = 1;
    while (sessionActive)
    {
        // 2. List Positions
        FILE *pfp = fopen(POSITION_FILE, "rb");
        if (!pfp)
            break;
        printf("\n--- SELECT A RACE ---\n");
        Position p;
        while (fread(&p, sizeof(Position), 1, pfp))
            printf("%d. %s\n", p.id, p.title);
        printf("0. Exit Session\n");
        fclose(pfp);

        int selectedPos;
        printf("Choice: ");
        scanf("%d", &selectedPos);
        if (selectedPos == 0)
            break;

        // 3. THE BUG FIX: Check if already voted in THIS race
        if (hasAlreadyVotedInRace(sid, selectedPos))
        {
            printf("\n!!! ERROR: You have already cast a vote for this position !!!\n");
            continue;
        }

        // 4. List Candidates for selectedPos
        FILE *cfp = fopen(CANDIDATE_FILE, "rb");
        Candidate c;
        int count = 0;
        printf("\n--- CANDIDATES ---\n");
        while (fread(&c, sizeof(Candidate), 1, cfp))
        {
            if (c.posID == selectedPos)
            {
                printf("ID %d: %s\n", c.id, c.name);
                count++;
            }
        }
        fclose(cfp);

        if (count == 0)
        {
            printf("No candidates.\n");
            continue;
        }

        int choiceID;
        printf("Enter Candidate ID: ");
        scanf("%d", &choiceID);

        // 5. Update Candidate and LOG the vote
        pthread_mutex_lock(&file_mutex);
        FILE *upfp = fopen(CANDIDATE_FILE, "rb+");
        int success = 0;
        while (fread(&c, sizeof(Candidate), 1, upfp))
        {
            if (c.id == choiceID && c.posID == selectedPos)
            {
                c.voteCount++;
                fseek(upfp, -(long)sizeof(Candidate), SEEK_CUR);
                fwrite(&c, sizeof(Candidate), 1, upfp);
                success = 1;
                break;
            }
        }
        fclose(upfp);

        if (success)
        {
            // Log this specific vote so they can't do it again
            FILE *lfp = fopen(VOTES_LOG_FILE, "ab");
            VoteRecord vr;
            strcpy(vr.studentID, sid);
            vr.posID = selectedPos;
            fwrite(&vr, sizeof(VoteRecord), 1, lfp);
            fclose(lfp);
            printf("Vote cast successfully!\n");
        }
        pthread_mutex_unlock(&file_mutex);

        printf("\nVote in another race? (1:Yes, 0:No): ");
        int cont;
        scanf("%d", &cont);
        if (cont == 0)
            sessionActive = 0;
    }
}

void announceResults()
{
    FILE *pfp = fopen(POSITION_FILE, "rb");
    if (!pfp)
    {
        printf("No positions found to tally.\n");
        return;
    }

    Position p;
    // Iterate through each race/position
    while (fread(&p, sizeof(Position), 1, pfp))
    {
        printf("\n============================================");
        printf("\n>> RESULTS FOR: %s <<", p.title);
        printf("\n============================================\n");

        // 1. Load candidates of THIS position into a temporary array
        FILE *cfp = fopen(CANDIDATE_FILE, "rb");
        if (!cfp)
            continue;

        Candidate list[100]; // Assuming max 100 candidates per race
        int count = 0;
        Candidate c;

        while (fread(&c, sizeof(Candidate), 1, cfp))
        {
            if (c.posID == p.id)
            {
                list[count] = c;
                count++;
            }
        }
        fclose(cfp);

        if (count == 0)
        {
            printf("No candidates were registered for this race.\n");
            continue;
        }

        // 2. Sort the array (Bubble Sort) by voteCount descending
        for (int i = 0; i < count - 1; i++)
        {
            for (int j = 0; j < count - i - 1; j++)
            {
                if (list[j].voteCount < list[j + 1].voteCount)
                {
                    Candidate temp = list[j];
                    list[j] = list[j + 1];
                    list[j + 1] = temp;
                }
            }
        }

        // 3. Display Sorted Results with Tie Detection
        int winningVotes = list[0].voteCount;
        int tieCount = 0;

        // Count how many people have the top score
        for (int i = 0; i < count; i++)
        {
            if (list[i].voteCount == winningVotes && winningVotes > 0)
            {
                tieCount++;
            }
        }

        for (int i = 0; i < count; i++)
        {
            if (winningVotes > 0 && list[i].voteCount == winningVotes)
            {
                if (tieCount > 1)
                {
                    printf("TIE -> %-20s | Votes: %d\n", list[i].name, list[i].voteCount);
                }
                else
                {
                    printf("WINNER -> %-20s | Votes: %d\n", list[i].name, list[i].voteCount);
                }
            }
            else
            {
                printf("          %-20s | Votes: %d\n", list[i].name, list[i].voteCount);
            }
        }

        if (tieCount > 1)
        {
            printf("\nNOTICE: A re-run or tie-breaker is required for %s.\n", p.title);
        }
    }
    fclose(pfp);
    printf("\n--- End of Official SONU Results ---\n");
}