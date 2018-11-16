all: ../build/gs_college_opt.mexa64 ../build/gs_marriage.mexa64
../build/gs_college_opt.mexa64 ../build/gs_marriage.mexa64: build_all.m
	matlab -r "build_all; exit" -nodesktop -nosplash

test: ../build/gs_college_opt.mexa64 ../build/gs_marriage.mexa64 ../tests/test_gs_college.m ../tests/test_gs_marriage.m
	cd ../tests/; \
		matlab -r "test_gs_college; test_gs_marriage; exit" -nodesktop -nojvm -nosplash

clean :
	cd ../build/; \
		rm -v *.mexa64
