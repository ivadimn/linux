all: lib 

lib: solution.h solution.c
	gcc -shared solution.c -fPIC -o libsolution.so 

clean:
	-rm libsolution.so 2>/dev/null


