all:
	gcc -Ofast bbc.c nnue_eval.c ./nnue/nnue.cpp ./nnue/misc.cpp -o bbc
	x86_64-w64-mingw32-gcc -Ofast bbc.c nnue_eval.c ./nnue/nnue.cpp ./nnue/misc.cpp -o bbc.exe

debug:
	gcc bbc.c nnue_eval.c ./nnue/nnue.cpp ./nnue/misc.cpp -o bbc
	x86_64-w64-mingw32-gcc bbc.c nnue_eval.c ./nnue/nnue.cpp ./nnue/misc.cpp -o bbc.exe
