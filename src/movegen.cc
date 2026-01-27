#include "movegen.h"
#include "types.h"
#include <iostream>

namespace lightknight::movegen {
    uint64_t BishopAttackBitboard(Square sq, uint64_t blockers_bitboard) {
        uint64_t relevant_blockers = blockers_bitboard & kBishopRelevantOccupancy[static_cast<int>(sq)];
        uint64_t magics_square_hash = (relevant_blockers * kBishopMagics[static_cast<int>(sq)]) >> kBishopMagicShift;
        uint64_t magics_hash = static_cast<uint64_t>(sq) * (1ULL << (64 - kBishopMagicShift)) + magics_square_hash;
        
        /*
        std::cout << "Square: " << static_cast<int>(sq) << std::endl;
        std::cout << "Blockers: " << std::endl;
        PrintBitboard(blockers_bitboard);
        std::cout << "Relevant Occupancy Mask: " << std::endl;
        PrintBitboard(kBishopRelevantOccupancy[static_cast<int>(sq)]);
        std::cout << "Relevant Blockers" << std::endl;
        PrintBitboard(relevant_blockers);
        std::cout << "Attackers:" << std::endl;
        */

        return kBishopAttacks[magics_hash];
    }

    uint64_t RookAttackBitboard(Square sq, uint64_t blockers_bitboard) {
        uint64_t relevant_blockers = blockers_bitboard & kRookRelevantOccupancy[static_cast<int>(sq)];
        uint64_t magics_square_hash = (relevant_blockers * kRookMagics[static_cast<int>(sq)]) >> kRookMagicShift;
        uint64_t magics_hash = static_cast<uint64_t>(sq) * (1ULL << (64 - kRookMagicShift)) + magics_square_hash;
        
        /*
        std::cout << "Square: " << static_cast<int>(sq) << std::endl;
        std::cout << "Blockers: " << std::endl;
        PrintBitboard(blockers_bitboard);
        std::cout << "Relevant Occupancy Mask: " << std::endl;
        PrintBitboard(kRookRelevantOccupancy[static_cast<int>(sq)]);
        std::cout << "Relevant Blockers" << std::endl;
        PrintBitboard(relevant_blockers);
        */
        return kRookAttacks[magics_hash];
    }

    uint64_t QueenAttackBitboard(Square sq, uint64_t blockers_bitboard) {
        return BishopAttackBitboard(sq, blockers_bitboard) | RookAttackBitboard(sq, blockers_bitboard);
    }
} // namespace lightknight::movegen