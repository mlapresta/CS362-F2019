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

randomtestresuls: randomtestcard1.c dominion.o rngs.o interface.o
	echo "Result for running dominion random tests:" > randomtestresult.out
	echo "randomtest1.c (Baron):" >> randomtestresult.out
	gcc -o randomtestcard1 -g randomtestcard1.c dominion.c rngs.c interface.c $(CFLAGS)
	./randomtestcard1 >> randomtestresult.out
	echo "\n\n##############################################################" >> randomtestresult.out
	echo "randomtest1 coverage:" >> randomtestresult.out
	echo "##############################################################" >> randomtestresult.out
	gcov dominion.c -f -b >>randomtestresult.out
	echo "randomtest2.c (Minion):" >> randomtestresult.out
	gcc -o randomtestcard2 -g randomtestcard2.c dominion.c rngs.c interface.c $(CFLAGS)
	./randomtestcard2 >> randomtestresult.out
	echo "\n\n##############################################################" >> randomtestresult.out
	echo "randomtest2 coverage:" >> randomtestresult.out
	echo "##############################################################" >> randomtestresult.out
	gcov dominion.c -f -b >>randomtestresult.out
	echo "randomtest3.c (Tribute):" >> randomtestresult.out
	gcc -o randomtestcard3 -g randomtestcard3.c dominion.c rngs.c interface.c $(CFLAGS)
	./randomtestcard3 >> randomtestresult.out
	echo "\n\n##############################################################" >> randomtestresult.out
	echo "randomtest3 coverage:" >> randomtestresult.out
	echo "##############################################################" >> randomtestresult.out
	gcov dominion.c -f -b >>randomtestresult.out

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player randomtestresuls

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe randomtestcard1 randomtestcard2 randomtestcard3
