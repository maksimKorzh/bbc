all:
	gcc -Ofast bbc.c nnue_eval.c ./nnue/nnue.cpp ./nnue/misc.cpp -o bbc
	#x86_64-w64-mingw32-gcc -Ofast bbc.c nnue_eval.c ./nnue/nnue.cpp ./nnue/misc.cpp -lws2_32 -o bbc.exe

debug:
	gcc bbc.c nnue_eval.c ./nnue/nnue.cpp ./nnue/misc.cpp -o bbc
	#x86_64-w64-mingw32-gcc -lws2_32 bbc.c nnue_eval.c ./nnue/nnue.cpp ./nnue/misc.cpp -o bbc.exe
