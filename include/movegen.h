#ifndef LIGHTKNIGHT_MOVEGEN_H
#define LIGHTKNIGHT_MOVEGEN_H

#include "types.h"
#include <array>

namespace lightknight::movegen {
    constexpr std::array<uint64_t, kNumSquares> PrecomputeKnightAttacks() {
        std::array<uint64_t, kNumSquares> knight_attacks = {0};

        for (uint8_t i = 0; i < kNumSquares; i++) {
            uint64_t bb = SquareToBitboard(static_cast<Square>(i));

            uint64_t attacks = 0ULL;
            attacks |= North(NorthWest(bb));
            attacks |= North(NorthEast(bb));
            attacks |= East(NorthEast(bb));
            attacks |= East(SouthEast(bb));
            attacks |= South(SouthEast(bb));
            attacks |= South(SouthWest(bb));
            attacks |= West(SouthWest(bb));
            attacks |= West(NorthWest(bb));

            knight_attacks[i] = attacks;
        }

        return knight_attacks;
    }
    inline constexpr std::array<uint64_t, kNumSquares> kKnightAttacks = PrecomputeKnightAttacks();

} // namespace lightknight::movegen

#endif // LIGHTKNIGHT_MOVEGEN_H