myname:
	cc -g -o myname.p homework-one.c

mylseek:
	cc -std=c11 -g -o mylseek.o mylseek.c
mychown:
	cc -std=c11 -g -o mychown.o mychown.c

mystat:
	cc -std=c11 -g -o mystat.o mystat.c

myhier:
	cc -std=c11 -g -o myhier.o myhierachy.c

test:
	touch "foo"
	touch "bar"
	./mystat.o foo
	./mystat.o bar
	rm foo
	rm bar

clean:
	rm $(RM) *.o *~ "foo" "bar"


