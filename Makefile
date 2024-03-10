solution: main.o lex_an.o make_tree.o run_tree.o delet_func.o
	gcc main.o lex_an.o make_tree.o run_tree.o delet_func.c -o solution -O2 -ftrapv -fsanitize=undefined -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -std=gnu11 -lm -g

main.o: main.c main.h
	gcc main.c -c

lex_an.o: lex_an.c lex_an.h
	gcc lex_an.c -c

make_tree.o: make_tree.c make_tree.h
	gcc make_tree.c -c

run_tree.o: run_tree.c run_tree.h
	gcc run_tree.c -c

delet_func.o: delet_func.c delet_func.h
	gcc delet_func.c -c

clean:
	rm -rf *.o
