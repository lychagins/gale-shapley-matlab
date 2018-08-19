clear mex;

% We will place the binaries there; add this folder to the search path
addpath('../build');

% Standard Gale-Shapley algorithm
mex('gale_shapley.cpp', 'gale_shapley_core.cpp', '-outdir', '../build');