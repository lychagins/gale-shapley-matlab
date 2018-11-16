// This code is a revised version of the matchingR package authored by 
// Jan Tilly and Nick Janetos (https://github.com/jtilly/matchingR).
#include <cstdint>
#include <cstdlib>

#ifndef gs_marriage_h
#define gs_marriage_h

// Marriage market, basic version
void gs_marriage(uint64_t *proposer_pref, double *reviewer_utils,
		size_t n_proposers, size_t n_reviewers, 
		uint64_t *engagements);

// Marriage market. Proposers have unacceptable candidates.
void gs_marriage(uint64_t *proposer_pref, double *reviewer_utils,
		size_t n_proposers, size_t n_reviewers, size_t *n_acceptable,
		uint64_t *engagements);

// Core algorithm
void gs_marriage_core(uint64_t *proposer_pref, double *reviewer_utils,
		size_t n_proposers, size_t n_reviewers, uint64_t **last_proposal,
		uint64_t *engagements);

#endif