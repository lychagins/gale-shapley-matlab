% This script tests the Gale-Shapley matching routine

clear;
num_proposers = 500;
num_reviewers = 1000;

rng(0, 'v5normal');

% Generate utilities, make sure there are no ties
reviewer_utils = rand(num_reviewers, num_proposers);
[~, reviewer_utils] = sort(reviewer_utils);
idx = (1:num_reviewers)';
reviewer_utils = idx(reviewer_utils);

% Generate reviewer rankings
proposer_pref = rand(num_reviewers, num_proposers);
[~, proposer_pref] = sort(proposer_pref);

% Note that the MEX code uses C indexing!
proposer_pref = uint64(proposer_pref - 1);

act_engagements = gale_shapley(proposer_pref, reviewer_utils);

% Type test
assert(isa(act_engagements, 'integer'), 'Non-integer engagements vector.');

% Size test
assert(all(size(act_engagements) == [num_reviewers, 1]), ...
    'Incorrect engagement vector size.');

% Content test
exp_engagements = csvread('resources/gs_engagements.csv');
assert(all(act_engagements == exp_engagements));