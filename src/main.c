#include "../include/voting.h"
#include <stdio.h>

int main()
{
    init_system();
    int choice;

    while (1)
    {
        printf("\n==============================");
        printf("\n   SONU VOTING SYSTEM MAIN");
        printf("\n==============================");
        printf("\n1. Admin: Add Position");
        printf("\n2. Admin: Register Candidate");
        printf("\n3. Voter: Register to Vote");
        printf("\n4. Voter: Cast Ballot");
        printf("\n5. Results: View Tally");
        printf("\n6. Exit");
        printf("\nSelection: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addPosition();
            break;
        case 2:
            registerCandidate();
            break;
        case 3:
            registerVoter();
            break;
        case 4:
            castVote();
            break;
        case 5:
            announceResults();
            break;
        case 6:
            return 0;
        default:
            printf("Invalid choice!\n");
        }
    }
    cleanup_system();
    return 0;
}