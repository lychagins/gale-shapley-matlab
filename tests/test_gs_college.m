% This script tests the college-optimal deferred acceptance routine
clear;

% Our compiled MEX binaries live here
addpath('../build');

%--------------------------------------------------------------------------
% Trivial case: no unacceptable students, each college has one seat, # of
% ranking categories = # of colleges. Check if we get the same results
% as in the marriage market algorithm.
%--------------------------------------------------------------------------
num_colleges = 500;
num_students = 1000;

rng(0, 'v5normal');

% Generate utilities, make sure there are no ties
student_utils = rand(num_students, num_colleges);
[~, student_utils] = sort(student_utils);
idx = (1:num_students)';
student_utils = idx(student_utils);

% Generate reviewer rankings
college_pref = rand(num_students, num_colleges);
[~, college_pref] = sort(college_pref);

% Note that the MEX code uses C indexing!
college_pref = uint64(college_pref - 1);

n_acceptable = uint64(num_students*ones(num_colleges, 1));
college_type = uint64((1:num_colleges)' - 1);
quota = ones(num_colleges, 1, 'uint64');
weights = ones(num_students, 1, 'uint64');
util_outside = min(student_utils, [], 2) - 1;


act_engagements = gs_college_opt(college_pref, n_acceptable, ...
    college_type, quota, student_utils, util_outside);

act_engagements2 = gs_college_opt(college_pref, n_acceptable, ...
    college_type, quota, student_utils, util_outside, weights);

% Type test
assert(isa(act_engagements, 'integer'), 'Non-integer engagements vector.');

% Size test
assert(all(size(act_engagements) == [num_students, 1]), ...
    'Incorrect engagement vector size.');

% Content test
exp_engagements = csvread('resources/gs_engagements.csv');
assert(all(act_engagements == exp_engagements),...
    'Simulated placements do not match expected results');

% Weighted DA with unit weights is equivalent to the standard DA
assert(all(act_engagements == act_engagements2), ...
    'Weighted DA with unit weights is not equivalent to the standard DA');

disp('College-optimal Gale-Shapley: PASSED');