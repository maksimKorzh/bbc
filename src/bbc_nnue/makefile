all:
	gcc -Ofast -fomit-frame-pointer bbc.c nnue_eval.c ./nnue/nnue.cpp ./nnue/misc.cpp -o bbc -DUSE_SSE41 -msse4.1 -DUSE_SSSE3 -mssse3 -DUSE_SSE2 -msse2 -DUSE_SSE -msse
	x86_64-w64-mingw32-gcc -Ofast -fomit-frame-pointer bbc.c nnue_eval.c ./nnue/nnue.cpp ./nnue/misc.cpp -o bbc.exe -DUSE_SSE41 -msse4.1 -DUSE_SSSE3 -mssse3 -DUSE_SSE2 -msse2 -DUSE_SSE -msse

debug:
	gcc bbc.c nnue_eval.c ./nnue/nnue.cpp ./nnue/misc.cpp -o bbc
	x86_64-w64-mingw32-gcc bbc.c nnue_eval.c ./nnue/nnue.cpp ./nnue/misc.cpp -o bbc.exe
