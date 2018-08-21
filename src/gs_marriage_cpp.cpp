// This code is a revised version of the matchingR package authored by
// Jan Tilly and Nick Janetos (https://github.com/jtilly/matchingR).

#include <queue>
#include "gs_marriage.h"

using namespace std;

//=========================================================================
// Marriage problem
//-------------------------------------------------------------------------

// Core algorithm
void gs_marriage_core(uint64_t *proposer_pref, double *reviewer_utils,
        size_t n_proposers, size_t n_reviewers, uint64_t **last_proposal,
        uint64_t *engagements)
{
    
    // n_proposers -- number of proposers, n_reviewers -- number of reviewers
    
    // proposer_pref -- pointer to a column-major array of full proposer
    // preferences over reviewers. Reviewer index takes values between 0
    // and (n_reviewers - 1). Column = proposer, row = order of preference.
    
    // reviewer_utils -- pointer to a column-major array of reviewer utility.
    // Column = proposer, row = reviewer. Elements are arbitrary doubles.
    
    // engagements -- a pointer to a pre-allocated array of n_reviewers elements.
    // This array is use to return the final list of matches. Row = reviewer,
    // value = matched proposer.
    
    
    // List of active proposals
    uint64_t *proposals;
    // Pointers to keep track of proposals made/to be made.
    uint64_t **next_proposal;
    uint64_t p, r;
    // Queue of bachelors
    queue<uint64_t> bachelors;
    
    proposals = (uint64_t *)malloc(n_proposers*sizeof(uint64_t));
    next_proposal = (uint64_t **)malloc(n_proposers*sizeof(uint64_t *));
    
    for(p=0; p < n_proposers; p++) {
        
        // proposals = n_reviewers for proposers without a preliminary match
        // Every proposer starts out as a bachelor
        proposals[p] = n_reviewers;
        bachelors.push(p);
        // Every proposer starts from the top of his preference list and stops
        // at the bottom (there are no unacceptable matches).
        next_proposal[p] = proposer_pref + p*n_reviewers;
        
    }
    
    for(r=0; r < n_reviewers; r++) {
        // engagements = n_proposers for reviewers without a preliminary match
        engagements[r] = n_proposers;
    }
    
    // Loop until there are no more proposals to be made
    
    while (!bachelors.empty())
    {
        
        // Get the index of the proposer
        p = bachelors.front();
        
        while(next_proposal[p] <= last_proposal[p]) {
            
            // Get the next proposal, move the pointer towards the end of the
            // preference list
            r = *next_proposal[p];
            next_proposal[p]++;
            // Is r still unmatched?
            if(engagements[r]==n_proposers) {
                // If so, then form a match
                engagements[r] = p;
                proposals[p] = r;
                break;
                
            }
            
            if(reviewer_utils[p*n_reviewers + r] > reviewer_utils[engagements[r]*n_reviewers + r])
            {
                
                // r is already matched, let's see if r can be poached
                // r's previous partner becomes unmatched (`n_reviewers` means unmatched)
                proposals[engagements[r]] = n_reviewers;
                bachelors.push(engagements[r]);
                
                // p and r form a match
                engagements[r] = p;
                proposals[p] = r;
                break;
                
            }
            
        }
        
        // Remove proposer from bachelor queue
        bachelors.pop();
    }
    
    free(proposals);
    free(next_proposal);
    
}


// Basic version
void gs_marriage(uint64_t *proposer_pref, double *reviewer_utils, 
        size_t n_proposers, size_t n_reviewers, uint64_t *engagements)
{
    
    uint64_t p;
    // Pointers to keep track of proposals made/to be made.
    uint64_t **last_proposal =
            (uint64_t **)malloc(n_proposers*sizeof(uint64_t *));
    
    // Every proposer starts from the top of his preference list and stops
    // at the bottom (there are no unacceptable matches).
    for(p=0; p < n_proposers; p++) {
        last_proposal[p] = proposer_pref + p*n_reviewers + (n_reviewers - 1);
    }
    
    gs_marriage_core(proposer_pref, reviewer_utils, n_proposers,
            n_reviewers, last_proposal, engagements);
    
    free(last_proposal);
            
}


// Proposers can have unacceptable matches.
void gs_marriage(uint64_t *proposer_pref, double *reviewer_utils, 
        size_t n_proposers, size_t n_reviewers, size_t *n_acceptable,
        uint64_t *engagements)
{
    // n_acceptable[p] -- the number reviewers acceptable for a proposer p.
    // We also assume the following preference structure:
    // First n_acceptable[p] items in p's slice of proposer_pref are
    // acceptable.
    
    uint64_t p;
    // Pointers to keep track of proposals made/to be made.
    uint64_t **last_proposal =
            (uint64_t **)malloc(n_proposers*sizeof(uint64_t *));
    
    // Every proposer starts from the top of his preference list and stops
    // at the bottom (there are no unacceptable matches).
    for(p=0; p < n_proposers; p++) {
        last_proposal[p] = proposer_pref + p*n_reviewers + n_acceptable[p] - 1;
    }
    
    gs_marriage_core(proposer_pref, reviewer_utils, n_proposers,
            n_reviewers, last_proposal, engagements);
    
    free(last_proposal);
            
}
