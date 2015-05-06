myname:
	cc -g -o myname.o homework1.c

mylseek:
	cc -std=c11 -g -o mylseek.o mylseek.c
mychown:
	cc -std=c11 -g -o mychown.o mychown.c

mystat:
	cc -std=c11 -g -o mystat.o mystat.c

myhier:
	cc -std=c11 -g  myhierarchy.h
	cc -std=c11 -g -o myhier.o myhierarchy.c
findusers:
	cc -std=c11 -g -o findusers.o findusers.c
supplementGID:
	cc -std=c11 -g -o sGID.o supplementGID.c
homework6:
	cc -Wdiv-by-zero -o hw6.o homework6.c
homework7:
	cc -o hw7.o homework7.c
homework8:
	gcc -pthread -o hw8.o homework8.c
threads:
	gcc -pthread -g -o threads.o threads.c
daemon:
	gcc -pthread -g -o timed.o daemon.c

dohomework:
	make homework6
test:
	echo emptytests

clean:
	$(RM) *.o *~ *.gch "foo" "bar"


