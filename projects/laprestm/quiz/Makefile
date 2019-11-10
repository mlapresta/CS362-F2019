CFLAGS= -Wall -fpic -coverage -lm -std=c99

testme: testme.c
	gcc -o testme testme.c -g $(CFLAGS)
	-./testme

randomtestrun: testme
	echo "Coverage result for running random test:" > randomtestresult.out
	gcov testme.c -f -b >> randomtestresult.out

all: randomtestrun

clean:
	rm -f *.o testme *.gcov *.gcda *.gcno *.so *.out
