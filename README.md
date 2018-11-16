# Gale-Shapley algorithm for large markets (Matlab and C++)
Fast implementation of Gale-Shapley deferred acceptance algorithm.

## Usage ##
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
