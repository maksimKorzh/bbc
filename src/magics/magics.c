#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define U64 unsigned long long

// set/get/pop bits
#define get_bit(bitboard, square) (bitboard & (1ULL << square))
#define pop_bit(bitboard, square) (get_bit(bitboard, square) ? (bitboard ^= (1ULL << square)) : 0)

typedef unsigned long long uint64;

uint64 random_uint64() {
  uint64 u1, u2, u3, u4;
  u1 = (uint64)(random()) & 0xFFFF; u2 = (uint64)(random()) & 0xFFFF;
  u3 = (uint64)(random()) & 0xFFFF; u4 = (uint64)(random()) & 0xFFFF;
  return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}

uint64 random_uint64_fewbits() {
  return random_uint64() & random_uint64() & random_uint64();
}

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
	
	return occupancy;
	  
}

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

// find magic number
uint64 find_magic(int square, int rellevant_bits, int bishop) {
    // define occupancies array
    U64 occupancies[4096];

    // define attacks array
    U64 attacks[4096];

    // define used indicies array
    U64 used_attacks[4096];

    // define magic number candidate
    U64 magic;
    
    // mask piece attack
    U64 mask_attack = bishop ? mask_bishop_attacks(square) : mask_rook_attacks(square);
    
    // count number of bits in attack mask
    int bit_count = count_bits(mask_attack);

    // occupancy variations
    int occupancy_variations = 1 << bit_count;

    // loop over the number of occupancy variations
    for(int count = 0; count < occupancy_variations; count++) {
        // init occupancies
        occupancies[count] = set_occupancy(count, bit_count, mask_attack);
        
        // init attacks
        attacks[count] = bishop ? bishop_attacks_on_the_fly(square, occupancies[count]) :
                              rook_attacks_on_the_fly(square, occupancies[count]);
    }

    // test magic numbers
    for(int random_count = 0; random_count < 100000000; random_count++)
    {
        // init magic number candidate
        magic = random_uint64_fewbits();

        // skip testing magic number if inappropriate
        if(count_bits((mask_attack * magic) & 0xFF00000000000000ULL) < 6) continue;

        // reset used attacks array
        memset(used_attacks, 0ULL, sizeof(used_attacks));
        
        // init count & fail flag
        int count, fail;
        
        // occupancy variation
        int occupancy_variation = 1 << bit_count;
        
        // test magic index
        for (count = 0, fail = 0; !fail && count < occupancy_variation; count++) {
          // generate magic index
          int magic_index = (int)((occupancies[count] * magic) >> (64 - rellevant_bits));
          
          // init used attacks on empty index
          if(used_attacks[magic_index] == 0ULL) used_attacks[magic_index] = attacks[count];
          
          // otherwise fail on  collision
          else if(used_attacks[magic_index] != attacks[count]) fail = 1;
        }
        
        // return magic if it works
        if(!fail) return magic;
    }
    
    // on fail
    printf("***Failed***\n");
    return 0ULL;
}

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

int main() {

  printf("const U64 rook_magics[64] = {\n");
  
  // loop over 64 board squares
  for(int square = 0; square < 64; square++)
      printf("    0x%llxULL,\n", find_magic(square, rook_rellevant_bits[square], 0));
  
  printf("};\n\nconst U64 bishop_magics[64] = {\n");
  
  // loop over 64 board squares
  for(int square = 0; square < 64; square++)
      printf("    0x%llxULL,\n", find_magic(square, bishop_rellevant_bits[square], 1));
  
  printf("};\n\n");

  return 0;
}
