# BBC 1.3
 - added Stockfish NNUE

# BBC 1.2
 - forfeit on time with 1sec increment bug fix
 - restored and improved commented evaluation features
 - slightly stronger than version 1.1

# BBC 1.1
 - added dynamic hash memory allocation
 - added tapered evaluation
 - added PST from PeSTO engine by Ronald Friederich
 - temporary removed all the eval by PST
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
