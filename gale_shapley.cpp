#include "gale_shapley_core.h"
#include "mex.h"

void mexFunction(int nlhs, mxArray *plhs[], 
	int nrhs, const mxArray *prhs[])
{
	double *reviewerUtils;
	uint64_t *proposerPref, *engagements;
	size_t M, W;
	
	proposerPref = (uint64_t *)mxGetData(prhs[0]);
	reviewerUtils = mxGetPr(prhs[1]);
	
	/* Number of proposers (men) */
	M = mxGetN(prhs[0]);
	/* Number of reviewers (women) */
	W = mxGetM(prhs[0]);

	plhs[0] = mxCreateNumericMatrix(W, 1, mxUINT64_CLASS, mxREAL);
	engagements = (uint64_t *)mxGetData(plhs[0]);
	
	cpp_galeshapley(proposerPref, reviewerUtils, M, W, engagements);
	
}