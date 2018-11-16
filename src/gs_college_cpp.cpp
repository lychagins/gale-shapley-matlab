#include <queue>
#include "gs_college.h"

using namespace std;

//=========================================================================
// College admissions problem, most general formulation
//-------------------------------------------------------------------------

// College-proposing deferred acceptance. Compared to the marriage problem, 
// this allows proposers to be matched with more than one reviewer.
// A typical trick is to treat college seats as independent proposers, but 
// this is very inefficient for large markets: the utility and the 
// preference matrices become too big to be kept in the memory.


void gs_college_opt(uint64_t *college_pref, 
		uint64_t *college_type, uint64_t *quota, double *student_utils,
		double *min_util,
		size_t n_colleges, size_t n_students, 
		uint64_t *n_acceptable, uint64_t *placement)
{
	
	// n_colleges -- number of proposers (colleges), 
	// n_students -- number of reviewers (students).
	
	// college_pref -- pointer to a column-major array of full college
	// preferences over students. Students index takes values between 0
	// and (n_reviewers - 1). Column = college type, row = order of preference.
	
	// college_type -- colleges of the same type have the same preferences 
	// over students. If placement is based on centralized exam scores, 
	// using college types leads to substantial memory savings.
	
	// quota -- admission quota
	
	// student_utils -- pointer to a column-major array of reviewer utility.
	// Column = proposer, row = reviewer. Elements are arbitrary doubles.
	
	// placement -- a pointer to a pre-allocated array of n_students elements.
	// This array is used to return the final list of student placements. 
	// Row = student, value = matched proposer.
	
	
	// Number of vacant seats
	uint64_t *seats_vacant;
	// Pointers to keep track of proposals made/to be made.
	uint64_t **next_proposal, **last_proposal;
	uint64_t p, r, poached_from;
	double *utility;
	// Queue of bachelors
	queue<uint64_t> vacant;
	
	seats_vacant = (uint64_t *)malloc(n_colleges*sizeof(uint64_t));
	next_proposal = (uint64_t **)malloc(n_colleges*sizeof(uint64_t *));
	last_proposal = (uint64_t **)malloc(n_colleges*sizeof(uint64_t *));
	utility = (double *)malloc(n_students*sizeof(double));
	
	for(p=0; p < n_colleges; p++) {
		
		// proposals = n_reviewers for proposers without a preliminary match
		// Every proposer starts out as a bachelor
		seats_vacant[p] = quota[p];
		vacant.push(p);
		// Every proposer starts from the top of his preference list and stops
		// at the bottom (there are no unacceptable matches).
		next_proposal[p] = college_pref + college_type[p]*n_students;
		last_proposal[p] = next_proposal[p] + n_acceptable[p] - 1;
	}
	
	for(r=0; r < n_students; r++) {
		// engagements = n_proposers for reviewers without a preliminary match
		utility[r] = min_util[r];
		placement[r] = n_colleges;
	}
	
	// Loop until there are no more proposals to be made
	
	while (!vacant.empty())
	{
		
		// Get the index of the next college
		p = vacant.front();
		
		while((next_proposal[p] <= last_proposal[p]) && (seats_vacant[p] > 0)) {
			
			// Get the next proposal, move the pointer towards the end of the
			// preference list
			r = *next_proposal[p];
			next_proposal[p]++;

			if(student_utils[p*n_students + r] > utility[r])
			{
				
				// If r is already matched, poach r
				if(placement[r] < n_colleges){
					
					poached_from = placement[r];
					if(seats_vacant[poached_from] == 0) {
						vacant.push(poached_from);
					}
					seats_vacant[poached_from]++;
				}
				
				// p and r form a match
				placement[r] = p;
				utility[r] = student_utils[p*n_students + r];
				seats_vacant[p]--;
				
			}
			
		}
		
		// Remove proposer from queue
		vacant.pop();
	}
	
	free(seats_vacant);
	free(next_proposal);
	free(last_proposal);
	free(utility);
}

void gs_college_opt_wgt(uint64_t *college_pref, 
		uint64_t *college_type, uint64_t *quota, double *student_utils,
		double *min_util, uint64_t *weight,
		size_t n_colleges, size_t n_students, size_t m_students, uint64_t *n_acceptable, 
		uint64_t *placement, double *utility, uint64_t *seats_vacant)
{
	
	// n_colleges -- number of proposers (colleges), 
	// n_students -- number of reviewers (students).
	
	// college_pref -- pointer to a column-major array of full college
	// preferences over students. Students index takes values between 0
	// and (n_reviewers - 1). Column = college type, row = order of preference.
	
	// college_type -- colleges of the same type have the same preferences 
	// over students. If placement is based on centralized exam scores, 
	// using college types leads to substantial memory savings.
	
	// quota -- admission quota
	
	// student_utils -- pointer to a column-major array of reviewer utility.
	// Column = proposer, row = reviewer. Elements are arbitrary doubles.
	
	// weight -- weight of the student.
	// m_students -- sum of weights
	
	// placement -- a pointer to a pre-allocated array of n_students elements.
	// This array is used to return the final list of student placements. 
	// Row = student, value = matched proposer.
	// seats_vacant -- number of vacant seats
	
	// Pointers to keep track of proposals made/to be made.
	uint64_t **next_proposal, **last_proposal, *wgt_counter;
	uint64_t *last_proposal_p, *next_proposal_p, wgt_counter_p, seats_vacant_p;
	uint64_t p, r, q, poached_from, *p2q_map;
	double min_util_r, u_prop, *cur_ptr, *next_ptr;
	// Queue of bachelors
	queue<uint64_t> vacant;
	
	next_proposal = (uint64_t **)malloc(n_colleges*sizeof(uint64_t *));
	last_proposal = (uint64_t **)malloc(n_colleges*sizeof(uint64_t *));
	wgt_counter = (uint64_t *)malloc(n_colleges*sizeof(uint64_t));
	p2q_map = (uint64_t *)malloc(n_students*sizeof(uint64_t));
	
	for(p=0; p < n_colleges; p++) {
		
		// proposals = n_reviewers for proposers without a preliminary match
		// Every proposer starts out as a bachelor
		seats_vacant[p] = quota[p];
		vacant.push(p);
		// Every proposer starts from the top of his preference list and stops
		// at the bottom (there are no unacceptable matches).
		next_proposal[p] = college_pref + college_type[p]*n_students;
		last_proposal[p] = next_proposal[p] + n_acceptable[p] - 1;
		wgt_counter[p] = weight[*next_proposal[p]];
	}
	
	cur_ptr = utility;
	for(r=0; r < n_students; r++) {
		
		min_util_r = min_util[r];
		next_ptr = cur_ptr + weight[r];
		while (cur_ptr < next_ptr) {
			*cur_ptr++ = min_util_r;
		}
		
	}
	
	for(q=0; q < m_students; q++) {
		// engagements = n_proposers for reviewers without a preliminary match
		placement[q] = n_colleges;
	}
	
	p2q_map[0] = weight[0];
	for(r=1; r < n_students; r++) {
		p2q_map[r] = p2q_map[r-1] + weight[r];
	}
	
	// Loop until there are no more proposals to be made
	
	while (!vacant.empty())
	{
		
		// Get the index of the next college
		p = vacant.front();

		last_proposal_p = last_proposal[p];
		next_proposal_p = next_proposal[p];
		seats_vacant_p = seats_vacant[p];
		
		if((next_proposal_p <= last_proposal_p) && (seats_vacant_p > 0)) {
			
			wgt_counter_p = wgt_counter[p];
			
			while(true) {
				// Get the next proposal, move the pointer towards the end of the
				// preference list
				
				// R is the student class
				r = *next_proposal_p;
				// Q is the student instance
				q = p2q_map[r] - wgt_counter_p;
				u_prop = student_utils[p*n_students + r];
				
				if(u_prop > utility[q])
				{
					
					// If q is already matched, poach q
					if(placement[q] < n_colleges){
						poached_from = placement[q];
						if(seats_vacant[poached_from] == 0) {
							vacant.push(poached_from);
						}
						seats_vacant[poached_from]++;
					}
					
					// p and r form a match
					placement[q] = p;
					utility[q] = u_prop;
					seats_vacant_p--;
					if(seats_vacant_p == 0)
						break;
					
				}
				
				if (wgt_counter_p > 1) {
					wgt_counter_p--;
				} else {
					next_proposal_p++;
					if(next_proposal_p > last_proposal_p)
						break;
					wgt_counter_p = weight[*next_proposal_p];
				}
			}
			
			seats_vacant[p] = seats_vacant_p;
			next_proposal[p] = next_proposal_p;
			wgt_counter[p] = wgt_counter_p;
		}
		
		// Remove proposer from the queue
		vacant.pop();
	}
	
	free(next_proposal);
	free(last_proposal);
	free(wgt_counter);
	free(p2q_map);
}


