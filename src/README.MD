# Sources
 - "bbc.c" is a current development state version of BBC chess engine
 - "bbc_1.0_version.c" is a very first release of BBC

# BBC 1.1a
 - temporary removed all the eval by PST
 - added tapered evaluation
 - added PST from PeSTO engine by Ronald Friederich
 - empty PV bug fixed
 - timing bugs fixed
 - id author/name by fixed

# BBC 1.0
This is the very first release of BBC

 - bitboard board representation
 - pre-calculated attack tables
 - magic bitboards for sliding pieces
 - encoding moves as integers
 - copy/make approach for making moves
 - negamax search with alpha beta pruning
 - PV/killer/history move ordering
 - iterative deepening
 - PVS (Principle Variation Search)
 - LMR (Late Move Reduction)
 - NMP (Null Move Pruning)
 - Transposition table (20MB fixed size)
 - Material/PST (Positional Square Tables) evaluation
 - Double/isolated/passed pawns evaluation
 - Bishop & Queen mobility evaluation
 - Very basic king safety
 - UCI protocol
