#include <stdint.h>

#ifndef gs_college_h
#define gs_college_h

// College-optimal DA, optimized for the case when some colleges have common 
// rankings
	
void gs_college_opt(uint64_t *college_pref, 
	uint64_t *college_type, uint64_t *quota, double *student_utils,
	double *min_util,
	size_t n_colleges, size_t n_students, 
	uint64_t *n_acceptable, uint64_t *placement);

#endif