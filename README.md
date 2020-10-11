# BBC (Bit Board Chess)
 - UCI chess engine by Code Monkey King<br>
 - written for didactic purposes and<br>
 - covered in 82 YouTube video series

# YouTube tutorials
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/QUNP-UjujBM/0.jpg)](https://www.youtube.com/watch?v=QUNP-UjujBM&list=PLmN0neTso3Jxh8ZIylk74JpwfiWNI76Cs)

# CCRL 1957 ELO rating
 - BBC 1.2 (is currently getting tested)
 - BBC 1.0 https://www.computerchess.org.uk/ccrl/404/cgi/engine_details.cgi?print=Details&each_game=1&eng=BBC%201.0%2064-bit#BBC_1_0_64-bit

# Testing
If your engine is stronger than 2500 ELO there's no much sense in playing matches<br>
versus BBC for at the moment it would most likely lose all the games.<br>
On the other hand testing versus engines of 2000 - 2300 ELO is welcomed!

# Test matches
https://github.com/maksimKorzh/bbc/tree/master/pgn

# Features
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
 - Transposition table (up to 128MB)
 - Material/PST (Positional Square Tables) evaluation
 - basic pawn structure/mobility/king safety evaluation
 - Tapered evaluation
 - UCI protocol

# Credits
  
  This project would never be completed<br>
  without the help of Talk Chess forum<br>
  community members who has been helping me<br>
  during the development process significantly<br>
  I would like to thank to:<br>
  <br>
   - HaraldLuessen & Pedro Castro Elgarresta for neverending hints and technical support
   - Sven Schüle for explaining how mating scores works in TT (in great details!)
   - Jay Warendorff for testing development versions of BBC<br>
   - Marcel Vanthoor for teaching me linear algebra and general inspirations
   - Ronald Friederich for providing PeSTO evaluation tables and sharing his ideas
   - Gabor Szots from CCRL for testing & rating BBC
   - Chris Rea for testing and fixing bugs
  <br>
  And all the subscribers watching my videos and<br>
  motivating me to keep the development!<br>

# Support the project
  https://www.patreon.com/code_monkey_king
  
  
  
  
  
