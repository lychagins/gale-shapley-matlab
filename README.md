# Gale-Shapley college-optimal algorithm for large markets (Matlab and C++)

When applied to large markets, Gale-Shapley college-optimal algorithm is constrained by memory bottlenecks. This implementation is designed to reduce memory requirements in settings where many colleges share the same preference ranking over students and many students share identical preferences over colleges.

## Typical use case ##
Consider a college market with 5,000 college programs and 1,000,000 students. The deferred acceptance algorithm would require two inputs at the minimum: a matrix of college preferences over students (5 million of 4-byte integers) and a matrix of student utilities over colleges (same number of single-precision floats). Taken together, this requires approximately 37 Gigabytes of memory. This rules out most consumer-grade laptops and desktops, adds to memory access overhead and slows down parallel simulations of college placements even on high-end workstations and some multi-CPU servers.

In many college admission systems, however, colleges admit students based on a handful of exam scores. For instance, Turkish college programs used to screen students based on four scores in 2002-2003. Effectively, this means that all colleges in Turkey belonged to one of four distinct preference types and college programs of the same type ranked all students in the economy in the same way. As a result, one can encode college preferences in the above example using a 1,000,000x4 matrix instead of 1,000,000x5,000 one.

Another optimization is feasible if students can be partitioned into a small number of types such that all students of the same type have the same preferences and are located contiguously in all college preference rankings in the same order. For instance, suppose that 1,000,000 students in the example above belong to 100,000 of such types. Tracking preferences and rankings of student types rather than students would reduce the student dimension of the preference matrices by the factor of 10.

## Installation ##
This package requires a working copy of Matlab configured to process MEX code with a suitable C++ compiler.

### Linux ###
To compile the code, change folder to `src` and run
```
make all
```
After the code is finished running, the compiled binaries can be found in the `build` folder.
To run optional tests, type
```
make test
```
To clean up, type
```
make clean
```

### Windows ###
To compile the code, run `src/build_all.m` from the Matlab command prompt.
The tests can be run manually from the `tests` folder.

## Usage ##

`[placement, utility, seats_vacant] = gs_college_opt(col_pref, n_acceptable, col_type, quota, st_util, util_min)`
or
`[...] = gs_college_opt(col_pref, n_acceptable, col_type, quota, st_util, util_min, st_weight)`

### Arguments ###
`col_pref`: College preference rankings. Each column corresponds to a college type. Rows give the ranking order. Each element is a student type index. Type: **uint64**. 
_Example: student type `col_pref(i,j)` is i^th^ most preferred student type in the ranking of college type j._
`n_acceptable`: Student types ranked below `n_acceptable(i)` are not eligible to apply to college `i`. Type: **uint64**.
`col_type`: College type, by college. Type: **uint64**.
`quota`: Quota by college. Type: **uint64**.
`st_util`: Student utility matrix. Rows correspond to colleges, columns correspond to student types. Type: **double**.
`util_min`: Reservation utility by student type. Student type i will not apply to college j if `st_util(j,i) < util_min(i)`. Type: **double**.
`st_weight`: _Optional argument_. Number of students in each student type; a column vector. If not specified, defaults to the vector of ones. Type: **uint64**.

### Return values ###
`placement`: Placement outcome for each student. Note that the number of students is equal to the total sum of `st_weight`. Type: **uint64**.
`utility`: Placement utility for each student. Type: **double**.
`seats_vacant`: The number of college seats left vacant, by college. Type: **uint64**.

## Installation ##
This package requires a working copy of Matlab configured to process MEX code with a suitable C++ compiler.

### Linux ###
To compile the code, change folder to `src` and run
```
make all
```
After the code is finished running, the compiled binaries can be found in the `build` folder.
To run optional tests, type
```
make test
```
To clean up, type
```
make clean
```

### Windows ###
To compile the code, run `src/build_all.m` from the Matlab command prompt.
The tests can be run manually from the `tests` folder.

## Usage ##

`[placement, utility, seats_vacant] = gs_college_opt(col_pref, n_acceptable, col_type, quota, st_util, util_min)`
or
`[...] = gs_college_opt(col_pref, n_acceptable, col_type, quota, st_util, util_min, st_weight)`

### Arguments ###
`col_pref`: College preference rankings. Each column corresponds to a college type. Rows give the ranking order. Each element is a student type index. Type: **uint64**. 
_Example: student type `col_pref(i,j)` is i^th^ most preferred student type in the ranking of college type j._
`n_acceptable`: Student types ranked below `n_acceptable(i)` are not eligible to apply to college `i`. Type: **uint64**.
`col_type`: College type, by college. Type: **uint64**.
`quota`: Quota by college. Type: **uint64**.
`st_util`: Student utility matrix. Rows correspond to colleges, columns correspond to student types. Type: **double**.
`util_min`: Reservation utility by student type. Student type i will not apply to college j if `st_util(j,i) < util_min(i)`. Type: **double**.
`st_weight`: _Optional argument_. Number of students in each student type; a column vector. If not specified, defaults to the vector of ones. Type: **uint64**.

### Return values ###
`placement`: Placement outcome for each student. Note that the number of students is equal to the total sum of `st_weight`. Type: **uint64**.
`utility`: Placement utility for each student. Type: **double**.
`seats_vacant`: The number of college seats left vacant, by college. Type: **uint64**.