// This is a MEX interface wrapper for college-optimal DA algorithm

#include "gs_college.h"
#include "mex.h"

void mexFunction(int nlhs, mxArray *plhs[], 
	int nrhs, const mxArray *prhs[])
{
	double *student_utils;
	uint64_t *college_pref, *college_type, *quota, *n_acceptable, *placement;
	size_t n_colleges, n_students;
	
	college_pref = (uint64_t *)mxGetData(prhs[0]);
	n_acceptable = (uint64_t *)mxGetData(prhs[1]);
	
	college_type = (uint64_t *)mxGetData(prhs[2]);
	quota = (uint64_t *)mxGetData(prhs[3]);
	
	student_utils = mxGetPr(prhs[4]);
	
	n_colleges = mxGetN(prhs[4]);
	n_students = mxGetM(prhs[4]);

	plhs[0] = mxCreateNumericMatrix(n_students, 1, mxUINT64_CLASS, mxREAL);
	placement = (uint64_t *)mxGetData(plhs[0]);
	
	gs_college_opt(college_pref, college_type, quota,
		student_utils, n_colleges, n_students, n_acceptable, 
		placement);
	
}