all:
	gcc -Wall -Lquadrupel/ movieinfo.c -o movieinfo -lquadrupel -lavformat -lavcodec

install: all
	cp movieinfo /usr/bin/
