# BBC (Bit Board Chess) + Stockfish NNUE!
 - UCI chess engine by Code Monkey King<br>
 - written for didactic purposes<br>
 - covered in 91 YouTube video series

# Play BBC online
http://maksimkorzh.pythonanywhere.com/

# Latest release
https://github.com/maksimKorzh/bbc/releases/tag/1.3

# GUI
https://github.com/maksimKorzh/uci-gui

# YouTube tutorials
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/QUNP-UjujBM/0.jpg)](https://www.youtube.com/watch?v=QUNP-UjujBM&list=PLmN0neTso3Jxh8ZIylk74JpwfiWNI76Cs)

# Latest CCRL rating is 2096 ELO rating
 - BBC 1.3 + SF NNUE (is currently getting tested)
 - BBC 1.2 not tested
 - BBC 1.1 2096 ELO https://www.computerchess.org.uk/ccrl/404/cgi/engine_details.cgi?print=Details&each_game=1&eng=BBC%201.1%2064-bit#BBC_1_1_64-bit
 - BBC 1.0 1957 ELO https://www.computerchess.org.uk/ccrl/404/cgi/engine_details.cgi?print=Details&each_game=1&eng=BBC%201.0%2064-bit#BBC_1_0_64-bit

# Testing
If your engine is stronger than 2500 ELO there's no much sense in playing matches<br>
versus BBC for at the moment it would most likely lose all the games.<br>
On the other hand testing versus engines of 2000 - 2300 ELO is welcomed!

# Test matches
https://github.com/maksimKorzh/bbc/tree/master/pgn

# Features
 - own online GUI
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
 - PURE Stockfish NNUE evaluation + 50 move rule penalty
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
   - Brendan J. Norman for creating logo for BBC
   - Daniel Shawul for providing NNUE probe library
  <br>
  And all the subscribers watching my videos and<br>
  motivating me to keep the development!<br>

# Support the project
  https://www.patreon.com/code_monkey_king
  
  
  
  
  
