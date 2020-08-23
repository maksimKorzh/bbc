/**********************************\
 ==================================
  
                BBC
                
 ==================================
          
       bitboard chess engine
       
                by
                
         Code Monkey King
 
 ==================================
\**********************************/

// include system headers
#include <stdio.h>
#include <string.h>
#ifdef WIN64
#include "windows.h"
#else
#include "sys/time.h"
#include "sys/select.h"
#include "string.h"
#endif

// FEN dedug positions
#define empty_board "8/8/8/8/8/8/8/8 w - - "
#define start_position "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "
#define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
#define killer_position "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define cmk_position "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 "

// square encoding
enum {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1, no_sq
};

// piece encoding
enum { P, N, B, R, Q, K, p, n, b, r, q, k };

// convert board square indexes to coordinates
char *square_to_coords[] = {
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
};

// capture flags
enum capture_flags {all_moves, only_captures};

// castling binary representation
//
//  bin  dec
// 0001    1  white king can castle to the king side
// 0010    2  white king can castle to the queen side
// 0100    4  black king can castle to the king side
// 1000    8  black king can castle to the queen side
//
// examples
// 1111       both sides an castle both directions
// 1001       black king => queen side
//            white king => king side

// castling writes
enum castling { KC = 1, QC = 2, kc = 4, qc = 8 };

// sides to move
enum sides { white, black };

// rook & bishop flags
enum { rook, bishop };

// ascii pieces
char ascii_pieces[] = "PNBRQKpnbrqk";

// unicode pieces
char *unicode_pieces[] = {"♙", "♘", "♗", "♖", "♕", "♔", "♟︎", "♞", "♝", "♜", "♛", "♚"};

// encode ascii pieces
int char_pieces[] = {
    ['P'] = P,
    ['N'] = N,
    ['B'] = B,
    ['R'] = R,
    ['Q'] = Q,
    ['K'] = K,
    ['p'] = p,
    ['n'] = n,
    ['b'] = b,
    ['r'] = r,
    ['q'] = q,
    ['k'] = k,
};

// decode promoted pieces
int promoted_pieces[] = {
    [Q] = 'q',
    [R] = 'r',
    [B] = 'b',
    [N] = 'n',
    [q] = 'q',
    [r] = 'r',
    [b] = 'b',
    [n] = 'n',
};

// material scrore

/*
    ♙ =   100   = ♙
    ♘ =   300   = ♙ * 3
    ♗ =   350   = ♙ * 3 + ♙ * 0.5
    ♖ =   500   = ♙ * 5
    ♕ =   1000  = ♙ * 10
    ♔ =   10000 = ♙ * 100
    
*/

int material_score[13] = {
      0,      // empty square score
    100,      // white pawn score
    300,      // white knight scrore
    350,      // white bishop score
    500,      // white rook score
   1000,      // white queen score
  10000,      // white king score
   -100,      // black pawn score
   -300,      // black knight scrore
   -350,      // black bishop score
   -500,      // black rook score
  -1000,      // black queen score
 -10000,      // black king score
    
};

// castling rights

/*
                             castle   move     in      in
                              right    map     binary  decimal

        white king  moved:     1111 & 1100  =  1100    12
  white king's rook moved:     1111 & 1110  =  1110    14
 white queen's rook moved:     1111 & 1101  =  1101    13
     
         black king moved:     1111 & 0011  =  1011    3
  black king's rook moved:     1111 & 1011  =  1011    11
 black queen's rook moved:     1111 & 0111  =  0111    7

*/

int castling_rights[128] = {
     7, 15, 15, 15,  3, 15, 15, 11,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    13, 15, 15, 15, 12, 15, 15, 14
};

// pawn positional score
const int pawn_score[128] = 
{
    90,  90,  90,  90,  90,  90,  90,  90,
    30,  30,  30,  40,  40,  30,  30,  30,
    20,  20,  20,  30,  30,  30,  20,  20,
    10,  10,  10,  20,  20,  10,  10,  10,
     5,   5,  10,  20,  20,   5,   5,   5,
     0,   0,   0,   5,   5,   0,   0,   0,
     0,   0,   0, -10, -10,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0
};

// knight positional score
const int knight_score[128] = 
{
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,  10,  10,   0,   0,  -5,
    -5,   5,  20,  20,  20,  20,   5,  -5,
    -5,  10,  20,  30,  30,  20,  10,  -5,
    -5,  10,  20,  30,  30,  20,  10,  -5,
    -5,   5,  20,  10,  10,  20,   5,  -5,
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5, -10,   0,   0,   0,   0, -10,  -5
};

// bishop positional score
const int bishop_score[128] = 
{
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,  10,  10,   0,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,  10,   0,   0,   0,   0,  10,   0,
     0,  30,   0,   0,   0,   0,  30,   0,
     0,   0, -10,   0,   0, -10,   0,   0

};

// rook positional score
const int rook_score[128] =
{
    50,  50,  50,  50,  50,  50,  50,  50,
    50,  50,  50,  50,  50,  50,  50,  50,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,   0,  20,  20,   0,   0,   0

};

// king positional score
const int king_score[128] = 
{
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   5,   5,   5,   5,   0,   0,
     0,   5,   5,  10,  10,   5,   5,   0,
     0,   5,  10,  20,  20,  10,   5,   0,
     0,   5,  10,  20,  20,  10,   5,   0,
     0,   0,   5,  10,  10,   5,   0,   0,
     0,   5,   5,  -5,  -5,   0,   5,   0,
     0,   0,   5,   0, -15,   0,  10,   0

};

// mirror positional score tables for opposite side
const int mirror_score[128] =
{
	a1, b1, c1, d1, e1, f1, g1, h1,
	a2, b2, c2, d2, e2, f2, g2, h2,
	a3, b3, c3, d3, e3, f3, g3, h3,
	a4, b4, c4, d4, e4, f4, g4, h4,
	a5, b5, c5, d5, e5, f5, g5, h5,
	a6, b6, c6, d6, e6, f6, g6, h6,
	a7, b7, c7, d7, e7, f7, g7, h7,
	a8, b8, c8, d8, e8, f8, g8, h8
};

/*
    Move formatting
    
    0000 0000 0000 0000 0111 1111       source square
    0000 0000 0011 1111 1000 0000       target square
    0000 0011 1100 0000 0000 0000       promoted piece
    0000 0100 0000 0000 0000 0000       capture flag
    0000 1000 0000 0000 0000 0000       double pawn flag
    0001 0000 0000 0000 0000 0000       enpassant flag
    0010 0000 0000 0000 0000 0000       castling

*/

// encode move
#define encode_move(source, target, piece, capture, pawn, enpassant, castling) \
(                          \
    (source) |             \
    (target << 7) |        \
    (piece << 14) |        \
    (capture << 18) |      \
    (pawn << 19) |         \
    (enpassant << 20) |    \
    (castling << 21)       \
)

// decode move's source square
#define get_move_source(move) (move & 0x7f)

// decode move's target square
#define get_move_target(move) ((move >> 7) & 0x7f)

// decode move's promoted piece
#define get_move_piece(move) ((move >> 14) & 0xf)

// decode move's capture flag
#define get_move_capture(move) ((move >> 18) & 0x1)

// decode move's double pawn push flag
#define get_move_pawn(move) ((move >> 19) & 0x1)

// decode move's enpassant flag
#define get_move_enpassant(move) ((move >> 20) & 0x1)

// decode move's castling flag
#define get_move_castling(move) ((move >> 21) & 0x1)


/**********************************\
 ==================================
 
            Chess board
 
 ==================================
\**********************************/

// define bitboard data type
#define U64 unsigned long long

// bitboards for all pieces (empty square & 12 pieces)
U64 bitboards[13];

// occupancy bitboards (all white & black pieces)
U64 occupancy[2];

// side to move | enpassant square | castling right
int side, enpassant, castle;

// kings' squares
int king_square[2];


/**********************************\
 ==================================
 
          Bit manipulations
 
 ==================================
\**********************************/

// bits manipulations
#define get_bit(bitboard, square) (bitboard & (1ULL << square))
#define set_bit(bitboard, square) (bitboard |= (1ULL << square))
#define pop_bit(bitboard, square) (get_bit(bitboard, square) ? (bitboard ^= (1ULL << square)) : 0)

// count bits (Brian Kernighan's way)
int count_bits(U64 bitboard) {
  // bit count
  int count = 0;
  
  // pop bits untill bitboard is empty
  while (bitboard)
  {
      // increment count
      count++;

      // consecutively reset least significant 1st bit
      bitboard &= bitboard - 1;
  }
  
  // return bit count
  return count;
}

// get index of LS1B in bitboard
int get_ls1b_index(U64 bitboard) {
    // make sure bitboard is not empty
    if (bitboard != 0)
        // convert trailing zeros before LS1B to ones and count them
        return count_bits((bitboard & -bitboard) - 1);
    
    // otherwise
    else
        // return illegal index
        return -1;
}


/**********************************\
 ==================================
 
           Input & Output
 
 ==================================
\**********************************/

// print bitboard
void print_bitboard(U64 bitboard)
{
    printf("\n");
    
    // loop over board ranks
    for (int rank = 0; rank < 8; rank++)
    {
        // loop over board files
        for (int file = 0; file < 8; file++)
        {
            // init board square
            int square = rank * 8 + file;
            
            // print ranks
            if (!file)
                printf("  %d ", 8 - rank);
            
            // print bit indexed by board square
            printf(" %d", get_bit(bitboard, square) ? 1 : 0);
        }
        
        printf("\n");
    }
    
    // print files
    printf("\n     a b c d e f g h\n\n");
    
    // print bitboard as decimal
    printf("     bitboard: %llud\n\n", bitboard);
}

// print chess board
void print_board()
{
    printf("\n");
    
    // loop over board ranks
    for (int rank = 0; rank < 8; rank++)
    {
        // loop over board files
        for (int file = 0; file < 8; file++)
        {
            // init board square
            int square = rank * 8 + file;
            
            // print ranks
            if (!file)
                printf("  %d ", 8 - rank);
            
            // init piece
            int piece = -1;
            
            // loop over bitboards
            for (int bb_piece = P; bb_piece <= k; bb_piece++)
            {
                // if piece is on bitboard
                if (get_bit(bitboards[bb_piece], square))
                {
                    // init piece
                    piece = bb_piece;
                    break;
                }
            }
            
            #ifdef WIN64
                // print ASCII pieces on windows
                printf(" %c", (piece == -1) ? '.' : ascii_pieces[piece]);
            #else
                // print UNICODE pieces on linux
                printf(" %s", (piece == -1) ? "." : unicode_pieces[piece]);
            #endif
        }
        
        printf("\n");
    }
    
    // print files
    printf("\n     a b c d e f g h\n\n");
    
    // print board stats
    printf("    Side:     %s\n", (side == white) ? "white": "black");
    printf("    Castling:  %c%c%c%c\n", (castle & KC) ? 'K' : '-', 
                                        (castle & QC) ? 'Q' : '-',
                                        (castle & kc) ? 'k' : '-',
                                        (castle & qc) ? 'q' : '-');
    printf("    Enpassant:   %s\n", (enpassant == no_sq)? "no" : square_to_coords[enpassant]);
    printf("    King square: %s\n\n", square_to_coords[king_square[side]]);
}

// parse FEN
void parse_fen(char *fen)
{
    // reset bitboards
    memset(bitboards, 0ULL, sizeof(bitboards));
    
    // loop over board ranks
    for (int rank = 0; rank < 8; rank++)
    {
        // loop over board files
        for (int file = 0; file < 8; file++)
        {
            // init square
            int square = rank * 8 + file;
        
            // match pieces
            if ((*fen >= 'a' && *fen <= 'z') || (*fen >= 'A' && *fen <= 'Z'))
            {
                // set up kings' squares
                if (*fen == 'K')
                    king_square[white] = square;
                
                else if (*fen == 'k')
                    king_square[black] = square;
                
                // init piece type
                int piece = char_pieces[*fen];

                // set the piece on board
                set_bit(bitboards[piece], square);
                
                // increment FEN pointer
                *fen++;
            }
            
            // match empty squares
            if (*fen >= '0' && *fen <= '9')
            {
                // calculate offset
                int offset = *fen - '0';
                
                // init piece
                int piece = -1;
                
                // loop over bitboards
                for (int bb_piece = P; bb_piece <= k; bb_piece++)
                {
                    // if piece is on bitboard
                    if (get_bit(bitboards[bb_piece], square))
                    {
                        // init piece
                        piece = bb_piece;
                        break;
                    }
                }
                
                // if no piece on current square
                if (piece == -1)
                    // decrement file
                    file--;
                
                // skip empty squares
                file += offset;
                
                // increment FEN pointer
                *fen++;
            }
            
            // match end of rank
            if (*fen == '/')
                // increment FEN pointer
                *fen++;
        }
    }
    
    // go to side parsing
    *fen++;
    
    // parse side to move
    side = (*fen == 'w') ? white : black;
    
    // go to castling rights parsing
    fen += 2;
    
    // parse castling rights
    while (*fen != ' ')
    {
        switch(*fen)
        {
            case 'K': castle |= KC; break;
            case 'Q': castle |= QC; break;
            case 'k': castle |= kc; break;
            case 'q': castle |= qc; break;
            case '-': break;
        }
        
        // increment pointer
        *fen++;
    }
    
    // got to empassant square
    *fen++;
    
    // parse empassant square
    if (*fen != '-')
    {
        // parse enpassant square's file & rank
        int file = fen[0] - 'a';
        int rank = 8 - (fen[1] - '0');
        
        // set up enpassant square
        enpassant = rank * 8 + file;
    }
    
    else
        enpassant = no_sq;   
}


/**********************************\
 ==================================
 
        Leaper piece attacks
 
 ==================================
\**********************************/

/* 
     not A file          not H file         not HG files      not AB files
      bitboard            bitboard            bitboard          bitboard

 8  0 1 1 1 1 1 1 1    1 1 1 1 1 1 1 0    1 1 1 1 1 1 0 0    0 0 1 1 1 1 1 1
 7  0 1 1 1 1 1 1 1    1 1 1 1 1 1 1 0    1 1 1 1 1 1 0 0    0 0 1 1 1 1 1 1
 6  0 1 1 1 1 1 1 1    1 1 1 1 1 1 1 0    1 1 1 1 1 1 0 0    0 0 1 1 1 1 1 1
 5  0 1 1 1 1 1 1 1    1 1 1 1 1 1 1 0    1 1 1 1 1 1 0 0    0 0 1 1 1 1 1 1
 4  0 1 1 1 1 1 1 1    1 1 1 1 1 1 1 0    1 1 1 1 1 1 0 0    0 0 1 1 1 1 1 1
 3  0 1 1 1 1 1 1 1    1 1 1 1 1 1 1 0    1 1 1 1 1 1 0 0    0 0 1 1 1 1 1 1
 2  0 1 1 1 1 1 1 1    1 1 1 1 1 1 1 0    1 1 1 1 1 1 0 0    0 0 1 1 1 1 1 1
 1  0 1 1 1 1 1 1 1    1 1 1 1 1 1 1 0    1 1 1 1 1 1 0 0    0 0 1 1 1 1 1 1
    
    a b c d e f g h    a b c d e f g h    a b c d e f g h    a b c d e f g h

*/

// A file mask
U64 not_a_file = 18374403900871474942ULL;

// H file mask
U64 not_h_file = 9187201950435737471ULL;

// HG files mask
U64 not_hg_file = 4557430888798830399ULL;

// AB files mask
U64 not_ab_file = 18229723555195321596ULL;

// pre-calculated pawn attacks [side][square]
U64 pawn_attacks[2][64];

// pre-calculated knight attacks [square]
U64 knight_attacks[64];

// pre-calculated king attacks [square]
U64 king_attacks[64];

// mask pawn attacks
U64 mask_pawn_attacks(int side, int square)
{
    // init piece bitboard
    U64 bitboard = 0ULL;
    
    // init attack bitboard
    U64 attacks = 0ULL;
    
    // set bit on piece bitboard at current square
    set_bit(bitboard, square);
    
    // white to move
    if (!side)
    {
        // generate attacks
        if ((bitboard >> 7) & not_a_file) attacks |= (bitboard >> 7);
        if ((bitboard >> 9 ) & not_h_file) attacks |= (bitboard >> 9);
    }
    
    // black to move
    else
    {
        // generate attacks
        if ((bitboard << 7) & not_h_file) attacks |= (bitboard << 7);
        if ((bitboard << 9 ) & not_a_file) attacks |= (bitboard << 9);
    }
    
    // return attack mask
    return attacks;
}

// mask knight attacks
U64 mask_knight_attacks(int square)
{
    // init piece bitboard
    U64 bitboard = 0ULL;
    
    // init attack bitboard
    U64 attacks = 0ULL;
    
    // set bit on piece bitboard at current square
    set_bit(bitboard, square);

    // generate attacks
    if ((bitboard >> 15) & not_a_file) attacks |= (bitboard >> 15);
    if ((bitboard >> 17) & not_h_file) attacks |= (bitboard >> 17);
    if ((bitboard >> 10) & not_hg_file) attacks |= (bitboard >> 10);
    if ((bitboard >> 6) & not_ab_file) attacks |= (bitboard >> 6);
    if ((bitboard << 15) & not_h_file) attacks |= (bitboard << 15);
    if ((bitboard << 17) & not_a_file) attacks |= (bitboard << 17);
    if ((bitboard << 10) & not_ab_file) attacks |= (bitboard << 10);
    if ((bitboard << 6) & not_hg_file) attacks |= (bitboard << 6);
    
    // return attack mask
    return attacks;
}

// mask king attacks
U64 mask_king_attacks(int square)
{
    // init piece bitboard
    U64 bitboard = 0ULL;
    
    // init attack bitboard
    U64 attacks = 0ULL;
    
    // set bit on piece bitboard at current square
    set_bit(bitboard, square);
    
    // generate attacks
    if (bitboard >> 8) attacks |= (bitboard >> 8);
    if ((bitboard >> 7) & not_a_file) attacks |= (bitboard >> 7);
    if ((bitboard >> 9) & not_h_file) attacks |= (bitboard >> 9);
    if ((bitboard >> 1) & not_h_file) attacks |= (bitboard >> 1);
    if (bitboard << 8) attacks |= (bitboard << 8);
    if ((bitboard << 7) & not_h_file) attacks |= (bitboard << 7);
    if ((bitboard << 9) & not_a_file) attacks |= (bitboard << 9);
    if ((bitboard << 1) & not_a_file) attacks |= (bitboard << 1);

    // return attack mask
    return attacks;
}

// init leaper pieces attacks
void init_leapers_attacks()
{
    // loop over bitboard indices
    for (int square = 0; square < 64; square++)
    {
        // init pawn attacks
        pawn_attacks[white][square] = mask_pawn_attacks(white, square);
        pawn_attacks[black][square] = mask_pawn_attacks(black, square);
        knight_attacks[square] = mask_knight_attacks(square);
        king_attacks[square] = mask_king_attacks(square);
    }
}


/**********************************\
 ==================================
 
        Slider piece attacks
 
 ==================================
\**********************************/

// masks
U64 bishop_masks[64];
U64 rook_masks[64];

// attacks
U64 bishop_attacks[64][512];
U64 rook_attacks[64][4096];

// rook rellevant occupancy bits
int rook_rellevant_bits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};

// bishop rellevant occupancy bits
int bishop_rellevant_bits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
};

const U64 rook_magics[64] = {
    0xa8002c000108020ULL,
    0x6c00049b0002001ULL,
    0x100200010090040ULL,
    0x2480041000800801ULL,
    0x280028004000800ULL,
    0x900410008040022ULL,
    0x280020001001080ULL,
    0x2880002041000080ULL,
    0xa000800080400034ULL,
    0x4808020004000ULL,
    0x2290802004801000ULL,
    0x411000d00100020ULL,
    0x402800800040080ULL,
    0xb000401004208ULL,
    0x2409000100040200ULL,
    0x1002100004082ULL,
    0x22878001e24000ULL,
    0x1090810021004010ULL,
    0x801030040200012ULL,
    0x500808008001000ULL,
    0xa08018014000880ULL,
    0x8000808004000200ULL,
    0x201008080010200ULL,
    0x801020000441091ULL,
    0x800080204005ULL,
    0x1040200040100048ULL,
    0x120200402082ULL,
    0xd14880480100080ULL,
    0x12040280080080ULL,
    0x100040080020080ULL,
    0x9020010080800200ULL,
    0x813241200148449ULL,
    0x491604001800080ULL,
    0x100401000402001ULL,
    0x4820010021001040ULL,
    0x400402202000812ULL,
    0x209009005000802ULL,
    0x810800601800400ULL,
    0x4301083214000150ULL,
    0x204026458e001401ULL,
    0x40204000808000ULL,
    0x8001008040010020ULL,
    0x8410820820420010ULL,
    0x1003001000090020ULL,
    0x804040008008080ULL,
    0x12000810020004ULL,
    0x1000100200040208ULL,
    0x430000a044020001ULL,
    0x280009023410300ULL,
    0xe0100040002240ULL,
    0x200100401700ULL,
    0x2244100408008080ULL,
    0x8000400801980ULL,
    0x2000810040200ULL,
    0x8010100228810400ULL,
    0x2000009044210200ULL,
    0x4080008040102101ULL,
    0x40002080411d01ULL,
    0x2005524060000901ULL,
    0x502001008400422ULL,
    0x489a000810200402ULL,
    0x1004400080a13ULL,
    0x4000011008020084ULL,
    0x26002114058042ULL,
};

const U64 bishop_magics[64] = {
    0x89a1121896040240ULL,
    0x2004844802002010ULL,
    0x2068080051921000ULL,
    0x62880a0220200808ULL,
    0x4042004000000ULL,
    0x100822020200011ULL,
    0xc00444222012000aULL,
    0x28808801216001ULL,
    0x400492088408100ULL,
    0x201c401040c0084ULL,
    0x840800910a0010ULL,
    0x82080240060ULL,
    0x2000840504006000ULL,
    0x30010c4108405004ULL,
    0x1008005410080802ULL,
    0x8144042209100900ULL,
    0x208081020014400ULL,
    0x4800201208ca00ULL,
    0xf18140408012008ULL,
    0x1004002802102001ULL,
    0x841000820080811ULL,
    0x40200200a42008ULL,
    0x800054042000ULL,
    0x88010400410c9000ULL,
    0x520040470104290ULL,
    0x1004040051500081ULL,
    0x2002081833080021ULL,
    0x400c00c010142ULL,
    0x941408200c002000ULL,
    0x658810000806011ULL,
    0x188071040440a00ULL,
    0x4800404002011c00ULL,
    0x104442040404200ULL,
    0x511080202091021ULL,
    0x4022401120400ULL,
    0x80c0040400080120ULL,
    0x8040010040820802ULL,
    0x480810700020090ULL,
    0x102008e00040242ULL,
    0x809005202050100ULL,
    0x8002024220104080ULL,
    0x431008804142000ULL,
    0x19001802081400ULL,
    0x200014208040080ULL,
    0x3308082008200100ULL,
    0x41010500040c020ULL,
    0x4012020c04210308ULL,
    0x208220a202004080ULL,
    0x111040120082000ULL,
    0x6803040141280a00ULL,
    0x2101004202410000ULL,
    0x8200000041108022ULL,
    0x21082088000ULL,
    0x2410204010040ULL,
    0x40100400809000ULL,
    0x822088220820214ULL,
    0x40808090012004ULL,
    0x910224040218c9ULL,
    0x402814422015008ULL,
    0x90014004842410ULL,
    0x1000042304105ULL,
    0x10008830412a00ULL,
    0x2520081090008908ULL,
    0x40102000a0a60140ULL,
};

// mask bishop attacks
U64 mask_bishop_attacks(int square)
{
    // attack bitboard
    U64 attacks = 0;
    
    // init files & ranks
    int f, r;
    
    // init target files & ranks
    int tr = square / 8;
    int tf = square % 8;
    
    // generate attacks
    for (r = tr + 1, f = tf + 1; r <= 6 && f <= 6; r++, f++) attacks |= (1ULL << (r * 8 + f));
    for (r = tr + 1, f = tf - 1; r <= 6 && f >= 1; r++, f--) attacks |= (1ULL << (r * 8 + f));
    for (r = tr - 1, f = tf + 1; r >= 1 && f <= 6; r--, f++) attacks |= (1ULL << (r * 8 + f));
    for (r = tr - 1, f = tf - 1; r >= 1 && f >= 1; r--, f--) attacks |= (1ULL << (r * 8 + f));
    
    // return attack map for bishop on a given square
    return attacks;
}

// mask rook attacks
U64 mask_rook_attacks(int square)
{
    // attacks bitboard
    U64 attacks = 0ULL;
    
    // init files & ranks
    int f, r;
    
    // init target files & ranks
    int tr = square / 8;
    int tf = square % 8;
    
    // generate attacks
    for (r = tr + 1; r <= 6; r++) attacks |= (1ULL << (r * 8 + tf));
    for (r = tr - 1; r >= 1; r--) attacks |= (1ULL << (r * 8 + tf));
    for (f = tf + 1; f <= 6; f++) attacks |= (1ULL << (tr * 8 + f));
    for (f = tf - 1; f >= 1; f--) attacks |= (1ULL << (tr * 8 + f));
    
    // return attack map for bishop on a given square
    return attacks;
}

// bishop attacks
U64 bishop_attacks_on_the_fly(int square, U64 block)
{
    // attack bitboard
    U64 attacks = 0;
    
    // init files & ranks
    int f, r;
    
    // init target files & ranks
    int tr = square / 8;
    int tf = square % 8;
    
    // generate attacks
    for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++)
    {
        attacks |= (1ULL << (r * 8 + f));
        if (block & (1ULL << (r * 8 + f))) break;
    }
    
    for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--)
    {
        attacks |= (1ULL << (r * 8 + f));
        if (block & (1ULL << (r * 8 + f))) break;
    }
    
    for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++)
    {
        attacks |= (1ULL << (r * 8 + f));
        if (block & (1ULL << (r * 8 + f))) break;
    }
    
    for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--)
    {
        attacks |= (1ULL << (r * 8 + f));
        if (block & (1ULL << (r * 8 + f))) break;
    }
    
    // return attack map for bishop on a given square
    return attacks;
}

// rook attacks
U64 rook_attacks_on_the_fly(int square, U64 block)
{
    // attacks bitboard
    U64 attacks = 0ULL;
    
    // init files & ranks
    int f, r;
    
    // init target files & ranks
    int tr = square / 8;
    int tf = square % 8;
    
    // generate attacks
    for (r = tr + 1; r <= 7; r++)
    {
        attacks |= (1ULL << (r * 8 + tf));
        if (block & (1ULL << (r * 8 + tf))) break;
    }
    
    for (r = tr - 1; r >= 0; r--)
    {
        attacks |= (1ULL << (r * 8 + tf));
        if (block & (1ULL << (r * 8 + tf))) break;
    }
    
    for (f = tf + 1; f <= 7; f++)
    {
        attacks |= (1ULL << (tr * 8 + f));
        if (block & (1ULL << (tr * 8 + f))) break;
    }
    
    for (f = tf - 1; f >= 0; f--)
    {
        attacks |= (1ULL << (tr * 8 + f));
        if (block & (1ULL << (tr * 8 + f))) break;
    }
    
    // return attack map for bishop on a given square
    return attacks;
}

// set occupancy
U64 set_occupancy(int square, int bit_count, U64 attack_mask) {
    
    // init occupancy bitboard
	U64 occupancy = 0ULL;
	
	// loop over bits
	for (int count = 0; count < bit_count; count++)
	{
		// get index (square) of LS1B of attack mask
		int bit_index = get_ls1b_index(attack_mask);
		pop_bit(attack_mask, bit_index);
		
		if (square & (1 << count)) 
			occupancy |= (1ULL << bit_index);
	}
	
	// return occupancy map
	return occupancy;
}

// init slider pieces attacks
void init_sliders_attacks(int is_bishop)
{
    // loop over 64 board squares
    for (int square = 0; square < 64; square++)
    {
        // init bishop & rook masks
        bishop_masks[square] = mask_bishop_attacks(square);
        rook_masks[square] = mask_rook_attacks(square);
        
        // init current mask
        U64 mask = is_bishop ? mask_bishop_attacks(square) : mask_rook_attacks(square);
        
        // count attack mask bits
        int bit_count = count_bits(mask);
        
        // occupancy variations count
        int occupancy_variations = 1 << bit_count;
        
        // loop over occupancy variations
        for (int count = 0; count < occupancy_variations; count++)
        {
            // bishop
            if (is_bishop)
            {
                // init occupancies, magic index & attacks
                U64 occupancy = set_occupancy(count, bit_count, mask);
                U64 magic_index = occupancy * bishop_magics[square] >> 64 - bishop_rellevant_bits[square];
                bishop_attacks[square][magic_index] = bishop_attacks_on_the_fly(square, occupancy);                
            }
            
            // rook
            else
            {
                // init occupancies, magic index & attacks
                U64 occupancy = set_occupancy(count, bit_count, mask);
                U64 magic_index = occupancy * rook_magics[square] >> 64 - rook_rellevant_bits[square];
                rook_attacks[square][magic_index] = rook_attacks_on_the_fly(square, occupancy);                
            }
        }
    }
}


// lookup bishop attacks 
U64 get_bishop_attacks(int square, U64 occupancy) {
	
	// calculate magic index
	occupancy &= bishop_masks[square];
	occupancy *=  bishop_magics[square];
	occupancy >>= 64 - bishop_rellevant_bits[square];
	
	// return rellevant attacks
	return bishop_attacks[square][occupancy];
	
}

// lookup rook attacks 
U64 get_rook_attacks(int square, U64 occupancy) {
	
	// calculate magic index
	occupancy &= rook_masks[square];
	occupancy *=  rook_magics[square];
	occupancy >>= 64 - rook_rellevant_bits[square];
	
	// return rellevant attacks
	return rook_attacks[square][occupancy];
	
}

int main()
{
    // init attack tables
    init_leapers_attacks();
    init_sliders_attacks(bishop);
    init_sliders_attacks(rook);
    
    U64 occ = 0ULL;
    set_bit(occ, d5);
    set_bit(occ, b4);
    set_bit(occ, d7);
    set_bit(occ, d8);
    set_bit(occ, d2);
    set_bit(occ, g4);
    
    print_bitboard(get_rook_attacks(d4, occ));
    
    return 0;
}









