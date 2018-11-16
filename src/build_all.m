clear mex;

% We will place the binaries there; add this folder to the search path
addpath('../build');

% Standard Gale-Shapley algorithm
mex gs_marriage.cpp gs_marriage_cpp.cpp -v -outdir ../build CXXFLAGS="\$CXXFLAGS -std=c++11";

mex gs_college_opt.cpp gs_college_cpp.cpp -v -outdir ../build CXXFLAGS="\$CXXFLAGS -std=c++11";