/* NNUE wrapping functions */

// include headers
#include "./nnue/nnue.h"
#include "nnue_eval.h"

// init NNUE
void init_nnue(char *filename)
{
    // call NNUE probe lib function
    nnue_init(filename);
}

// get NNUE score directly
int evaluate_nnue(int player, int *pieces, int *squares)
{
    // call NNUE probe lib function
    nnue_evaluate(player, pieces, squares);
}

// det NNUE score from FEN input
int evaluate_fen_nnue(char *fen)
{
    // call NNUE probe lib function
    nnue_evaluate_fen(fen);
}
