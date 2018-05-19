rx: rx.c
	gcc -O3 rx.c -lm -orx
clang: rx.c
	clang -O3 rx.c -lm -orx
clean:
	rm rx

