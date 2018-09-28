// This is a MEX interface wrapper for college-optimal DA algorithm

#include "gs_college.h"
#include "mex.h"

void mexFunction(int nlhs, mxArray *plhs[], 
	int nrhs, const mxArray *prhs[])
{
	/*
	* PRHS[0] -- preference rankings of colleges over students
	* PRHS[1] -- number of acceptable students, by program. Only first 
	*            n_acceptable[i] students are eligible for admission at program i.
	* PRHS[2] -- order used to rank students, by college
	* PRHS[3] -- quota, by college
	* PRHS[4] -- full utility matrix, for each student and college
	* PRHS[5] -- "reservation price"; utility of not being matched, by student
	* PRHS[6] (optional) -- student weight
	*/
	
	double *student_utils, *min_util;
	uint64_t *college_pref, *college_type, *quota, *n_acceptable, *placement, *weight;
	size_t n_colleges, n_students, m_students, r;
	
	college_pref = (uint64_t *)mxGetData(prhs[0]);
	n_acceptable = (uint64_t *)mxGetData(prhs[1]);
	
	college_type = (uint64_t *)mxGetData(prhs[2]);
	quota = (uint64_t *)mxGetData(prhs[3]);
	
	student_utils = mxGetPr(prhs[4]);
	min_util = mxGetPr(prhs[5]);
	
	n_colleges = mxGetN(prhs[4]);
	n_students = mxGetM(prhs[4]);
	
	if(nrhs>6) {
		
		weight = (uint64_t *)mxGetData(prhs[6]);
		
		// Mass of students = sum of student weights
		m_students = 0;
		for (r=0; r<n_students; r++) {
			m_students += weight[r];
		}
		
		plhs[0] = mxCreateNumericMatrix(m_students, 1, mxUINT64_CLASS, mxREAL);
		placement = (uint64_t *)mxGetData(plhs[0]);
		
		gs_college_opt_wgt(college_pref, college_type, quota,
			student_utils, min_util, weight,
			n_colleges, n_students, m_students, n_acceptable, placement);
		
	} else {
		
		plhs[0] = mxCreateNumericMatrix(n_students, 1, mxUINT64_CLASS, mxREAL);
		placement = (uint64_t *)mxGetData(plhs[0]);
		
		gs_college_opt(college_pref, college_type, quota,
			student_utils, min_util, n_colleges, n_students, n_acceptable, 
			placement);
		
	}
}