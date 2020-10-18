#ifndef NNUE_H
#define NNUE_H

#include <stdbool.h>
#include <stdalign.h>

#include "misc.h"

#ifdef __cplusplus
#   define EXTERNC extern "C"
#else
#   define EXTERNC
#endif
#if defined (_WIN32)
#   define _CDECL __cdecl
#ifdef DLL_EXPORT
#   define DLLExport EXTERNC __declspec(dllexport)
#else
#   define DLLExport EXTERNC __declspec(dllimport)
#endif
#else
#   define _CDECL
#   define DLLExport EXTERNC
#endif

/*pieces*/
enum colors {
    white,black
};
enum chessmen {
    blank,king,queen,rook,bishop,knight,pawn
};
const int pic_tab[14] = {
    blank,king,queen,rook,bishop,knight,pawn,
    king,queen,rook,bishop,knight,pawn,blank
};

#define PIECE(x)         (pic_tab[x])
#define COMBINE(c,x)     ((x) + (c) * 6) 

/*nnue data*/
#if 0
typedef struct DirtyPiece {
  int dirtyNum;
  int pc[3];
  int from[3];
  int to[3];
} DirtyPiece;
#endif

typedef struct {
  alignas(64) int16_t accumulation[2][256];
  bool computedAccumulation;
} Accumulator;

/*position*/
typedef struct Position {
  int player;
  int* pieces;
  int* squares;
  Accumulator accumulator;
#if 0
  DirtyPiece dirtyPiece;
#endif
} Position;

int nnue_evaluate_pos(Position* pos);

/**
* Load NNUE file
*/

#ifdef __cplusplus
extern "C" {
#endif

void nnue_init(
  const char * evalFile             /** Path to NNUE file */
);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C" {
#endif

/**
* Evaluate on FEN string
*/
int nnue_evaluate_fen(
  const char* fen                   /** FEN string to probe evaluation for */
);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C" {
#endif
/**
* Evaluation subroutine suitable for chess engines.
* -------------------------------------------------
* Piece codes are
*     wking=1, wqueen=2, wrook=3, wbishop= 4, wknight= 5, wpawn= 6,
*     bking=7, bqueen=8, brook=9, bbishop=10, bknight=11, bpawn=12,
* Square are
*     A1=0, B1=1 ... H8=63
* Input format:
*     piece[0] is white king, square[0] is its location
*     piece[1] is black king, square[1] is its location
*     ..
*     piece[x], square[x] can be in any order
*     ..
*     piece[n+1] is set to 0 to represent end of array
*/
int nnue_evaluate(
  int player,                       /** Side to move */
  int* pieces,                      /** Array of pieces */
  int* squares                      /** Corresponding array of squares the piece stand on */
);
#ifdef __cplusplus
}
#endif

#endif
