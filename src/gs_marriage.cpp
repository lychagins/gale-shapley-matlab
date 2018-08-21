// This is a MEX interface wrapper for the Gale-Shapley algorithm

#include "gs_marriage.h"
#include "mex.h"

void mexFunction(int nlhs, mxArray *plhs[], 
	int nrhs, const mxArray *prhs[])
{
	double *reviewer_utils;
	uint64_t *proposer_pref, *engagements;
	size_t n_proposers, n_reviewers;
	
	proposer_pref = (uint64_t *)mxGetData(prhs[0]);
	reviewer_utils = mxGetPr(prhs[1]);
	
	n_proposers = mxGetN(prhs[0]);
	n_reviewers = mxGetM(prhs[0]);

	plhs[0] = mxCreateNumericMatrix(n_reviewers, 1, mxUINT64_CLASS, mxREAL);
	engagements = (uint64_t *)mxGetData(plhs[0]);
	
	gs_marriage(proposer_pref, reviewer_utils, 
            n_proposers, n_reviewers, engagements);
	
}