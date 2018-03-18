all: 
	if [ "${CODESHIP}" = "TRUE" -o "${CIRCLECI}" = "true" -o ] ; then make -f Makefile1 ; else ./run_docker ; fi

test:
	echo "tests disabled"
	#./test1 && ./test2

check:
	./compiling

