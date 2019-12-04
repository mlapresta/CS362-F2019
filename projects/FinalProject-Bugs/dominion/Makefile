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

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

unittestresults: unittest*.c dominion.o rngs.o interface.o
	echo "Result for running dominion unit tests:" > unittestresults.out
	echo "\n\n##############################################################" >> unittestresults.out
	echo "bug 1 unittest:" >> unittestresults.out
	echo "##############################################################" >> unittestresults.out
	gcc -o unittest1 -g unittest1.c dominion.c rngs.c interface.c $(CFLAGS)
	./unittest1 >> unittestresults.out
	echo "\n\n##############################################################" >> unittestresults.out
	echo "test1 coverage:" >> unittestresults.out
	echo "##############################################################" >> unittestresults.out
	gcov dominion.c -f -b >>unittestresults.out
	echo "\n\n##############################################################" >> unittestresults.out
	echo "bug 2 unittest:" >> unittestresults.out
	echo "##############################################################" >> unittestresults.out
	gcc -o unittest2 -g unittest2.c dominion.c rngs.c interface.c $(CFLAGS)
	./unittest2 >> unittestresults.out
	echo "\n\n##############################################################" >> unittestresults.out
	echo "test2 coverage:" >> unittestresults.out
	echo "##############################################################" >> unittestresults.out
	gcov dominion.c -f -b >>unittestresults.out
	echo "\n\n##############################################################" >> unittestresults.out
	echo "bug 3 unittest:" >> unittestresults.out
	echo "##############################################################" >> unittestresults.out
	gcc -o unittest3 -g unittest3.c dominion.c rngs.c interface.c $(CFLAGS)
	./unittest3 >> unittestresults.out
	echo "\n\n##############################################################" >> unittestresults.out
	echo "test3 coverage:" >> unittestresults.out
	echo "##############################################################" >> unittestresults.out
	gcov dominion.c -f -b >>unittestresults.out
	echo "\n\n##############################################################" >> unittestresults.out
	echo "bug 4 unittest:" >> unittestresults.out
	echo "##############################################################" >> unittestresults.out
	gcc -o unittest4 -g unittest4.c dominion.c rngs.c interface.c $(CFLAGS)
	./unittest4 >> unittestresults.out
	echo "\n\n##############################################################" >> unittestresults.out
	echo "test4 coverage:" >> unittestresults.out
	echo "##############################################################" >> unittestresults.out
	gcov dominion.c -f -b >>unittestresults.out
	echo "\n\n##############################################################" >> unittestresults.out
	echo "bug 5 unittest:" >> unittestresults.out
	echo "##############################################################" >> unittestresults.out
	gcc -o unittest5 -g unittest5.c dominion.c rngs.c interface.c $(CFLAGS)
	./unittest5 >> unittestresults.out
	echo "\n\n##############################################################" >> unittestresults.out
	echo "test5 coverage:" >> unittestresults.out
	echo "##############################################################" >> unittestresults.out
	gcov dominion.c -f -b >>unittestresults.out
	echo "\n\n##############################################################" >> unittestresults.out
	echo "bug 6 unittest:" >> unittestresults.out
	echo "##############################################################" >> unittestresults.out
	gcc -o unittest6 -g unittest6.c dominion.c rngs.c interface.c $(CFLAGS)
	./unittest6 >> unittestresults.out
	echo "\n\n##############################################################" >> unittestresults.out
	echo "test6 coverage:" >> unittestresults.out
	echo "##############################################################" >> unittestresults.out
	gcov dominion.c -f -b >>unittestresults.out
	echo "\n\n##############################################################" >> unittestresults.out
	echo "bug 7 unittest:" >> unittestresults.out
	echo "##############################################################" >> unittestresults.out
	gcc -o unittest7 -g unittest7.c dominion.c rngs.c interface.c $(CFLAGS)
	./unittest7 >> unittestresults.out
	echo "\n\n##############################################################" >> unittestresults.out
	echo "test7 coverage:" >> unittestresults.out
	echo "##############################################################" >> unittestresults.out
	gcov dominion.c -f -b >>unittestresults.out
	echo "\n\n##############################################################" >> unittestresults.out
	echo "bug 8 unittest:" >> unittestresults.out
	echo "##############################################################" >> unittestresults.out
	gcc -o unittest8 -g unittest8.c dominion.c rngs.c interface.c $(CFLAGS)
	./unittest8 >> unittestresults.out
	echo "\n\n##############################################################" >> unittestresults.out
	echo "test8 coverage:" >> unittestresults.out
	echo "##############################################################" >> unittestresults.out
	gcov dominion.c -f -b >>unittestresults.out
	echo "\n\n##############################################################" >> unittestresults.out
	echo "bug 9 unittest:" >> unittestresults.out
	echo "##############################################################" >> unittestresults.out
	gcc -o unittest9 -g unittest9.c dominion.c rngs.c interface.c $(CFLAGS)
	./unittest9 >> unittestresults.out
	echo "\n\n##############################################################" >> unittestresults.out
	echo "test9 coverage:" >> unittestresults.out
	echo "##############################################################" >> unittestresults.out
	gcov dominion.c -f -b >>unittestresults.out
	echo "\n\n##############################################################" >> unittestresults.out
	echo "bug 10 unittest:" >> unittestresults.out
	echo "##############################################################" >> unittestresults.out
	gcc -o unittest10 -g unittest10.c dominion.c rngs.c interface.c $(CFLAGS)
	./unittest10 >> unittestresults.out
	echo "\n\n##############################################################" >> unittestresults.out
	echo "test10 coverage:" >> unittestresults.out
	echo "##############################################################" >> unittestresults.out
	gcov dominion.c -f -b >>unittestresults.out

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player unittestresults

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe unittest*.exe unittest1 unittest2 unittest3 unittest4 unittest5 unittest6 unittest7 unittest8 unittest9 unittest10
