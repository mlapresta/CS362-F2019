CFLAGS= -Wall -fpic -coverage -lm -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/

testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

unittestresults: unittest1.c unittest2.c dominion.o rngs.o interface.o
	echo "Result for running dominion unit tests:" > unittestresult.out
	echo "unittest1.c (Baron):" >> unittestresult.out
	gcc -o unittest1 -g unittest1.c dominion.c rngs.c interface.c $(CFLAGS)
	./unittest1 >> unittestresult.out
	echo "\n\n##############################################################" >> unittestresult.out
	echo "test1 coverage:" >> unittestresult.out
	echo "##############################################################" >> unittestresult.out
	gcov dominion.c -f -b >>unittestresult.out
	echo "unittest2.c (Minion):" >> unittestresult.out
	gcc -o unittest2 -g unittest2.c dominion.c rngs.c interface.c $(CFLAGS)
	./unittest2 >> unittestresult.out
	echo "\n\n##############################################################" >> unittestresult.out
	echo "test2 coverage:" >> unittestresult.out
	echo "##############################################################" >> unittestresult.out
	gcov dominion.c -f -b >>unittestresult.out
	echo "unittest3.c (Ambassador):" >> unittestresult.out
	gcc -o unittest3 -g unittest3.c dominion.c rngs.c interface.c $(CFLAGS)
	./unittest3 >> unittestresult.out
	echo "\n\n##############################################################" >> unittestresult.out
	echo "test3 coverage:" >> unittestresult.out
	echo "##############################################################" >> unittestresult.out
	gcov dominion.c -f -b >>unittestresult.out
	echo "unittest4.c (Tribute):" >> unittestresult.out
	gcc -o unittest4 -g unittest4.c dominion.c rngs.c interface.c $(CFLAGS)
	./unittest4 >> unittestresult.out
	echo "\n\n##############################################################" >> unittestresult.out
	echo "test4 coverage:" >> unittestresult.out
	echo "##############################################################" >> unittestresult.out
	gcov dominion.c -f -b >>unittestresult.out
	echo "unittest5.c (Mine):" >> unittestresult.out
	gcc -o unittest5 -g unittest5.c dominion.c rngs.c interface.c $(CFLAGS)
	./unittest5 >> unittestresult.out
	echo "\n\n##############################################################" >> unittestresult.out
	echo "test5 coverage:" >> unittestresult.out
	echo "##############################################################" >> unittestresult.out
	gcov dominion.c -f -b >>unittestresult.out

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player unittestresults

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe unittest1 unittest1.exe
