#include <cs50.h>
#include <stdio.h>
#include <string.h>

// * Max number of candidates
#define MAX 9

// * preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// * locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// * Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// * Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// * Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool is_cycle(int winner, int loser);

int main(int argc, string argv[])
{
    // ? Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // ? Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // ? Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // ? Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ? ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // ? Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }
        record_preferences(ranks);
        printf("\n");
    }
// // ! check 2d-array --------------------------------------------------------------------------------------------------------------------------------
//     printf("Check 2D array:\n");
//     for (int i = 0; i < candidate_count; i++)
//     {
//         for (int j = 0; j < candidate_count; j++)
//         {
//             printf("%d ", preferences[i][j]);
//         }
//         printf("\n");
//     }
//     printf("\n");
// // ! --------------------------------------------------------------------------------------------------------------------------------

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// ? Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
// ? look for a candidate called name
    for (int i = 0; i < candidate_count; i++)
    {
// ? if candidate found, update ranks and return true. ranks[i] is the voter's ith preference
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
// ? if no candidate found, don't update any ranks and return false
    return false;
}

// ? Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // ? update the preferences array based on the current voter's ranks
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// ? Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // ? add each pair of candidates to pairs array if one candidate is preferred over the other
    // ? update global variable pair_count to be the total number of pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] != 0 && (preferences[i][j] > preferences[j][i]))
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[j][i] != 0 && (preferences[i][j] < preferences[j][i]))
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
// // ! check pairs --------------------------------------------------------------------------------------------------------------------------------
//     printf("Check pairs before sorting:\n");
//     for (int i = 0; i < pair_count; i++)
//     {
//         printf("pair %i: winner = %i | loser = %i | strength = %i\n", i, pairs[i].winner, pairs[i].loser, preferences[pairs[i].winner][pairs[i].loser]);
//     }
//     printf("\n");
// // ! --------------------------------------------------------------------------------------------------------------------------------------------
    return;
}

// ? Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    pair temp;
    for (int i = 0; i < pair_count; i++)
    {
        for (int j = i + 1; j < pair_count; j++)
        {
            if (preferences[pairs[j].winner][pairs[j].loser] > preferences[pairs[i].winner][pairs[i].loser])
            {
                temp = pairs[i];
                pairs[i] = pairs[j];
                pairs[j] = temp;
            }
        }
    }
// // ! check pairs --------------------------------------------------------------------------------------------------------------------------------
//     printf("Check pairs after sorting:\n");
//     for (int i = 0; i < pair_count; i++)
//     {
//         printf("pair %i: winner = %i | loser = %i | strength = %i\n", i, pairs[i].winner, pairs[i].loser, preferences[pairs[i].winner][pairs[i].loser]);
//     }
//     printf("\n");
// // ! --------------------------------------------------------------------------------------------------------------------------------
    return;
}

bool is_cycle(int winner, int loser)
{
    if (winner == loser)
    {
        return true;
    }
    bool check = false;
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[loser][i])
        {
            check = is_cycle(winner, i);
        }
        if (check || i == candidate_count - 1)
        {
            return check;
        }
    }
    return false;
}

// ? Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
// TODO
// ? update locked to create the locked graph by adding all edges in decreasing order of victory strength, as long as there is no cycle
    for (int i = 0; i < pair_count; i++)
    {
        if (!is_cycle(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }

// // ! check locked graph --------------------------------------------------------------------------------------------------------------------------------
//     printf("Check locked graph pairs:\n");
//     for (int i = 0; i < candidate_count; i++)
//     {
//         for (int j = 0; j < candidate_count; j++)
//         {
//             printf("%d ", locked[i][j]);
//         }
//         printf("\n");
//     }
//     printf("\n");
// // ! --------------------------------------------------------------------------------------------------------------------------------
    return;
}

// ? Print the winner of the election
void print_winner(void)
{
// TODO
// ? print out the winner of the election, who will be the source of the graph.
// ? you may assume there will not be more than one source
    for (int i = 0; i < pair_count; i++)
    {
        bool is_winner = true;
        for (int j = 0; j < pair_count; j++)
        {
            if (locked[j][i])
            {
                is_winner = false;
            }
        }
        if (is_winner)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
}