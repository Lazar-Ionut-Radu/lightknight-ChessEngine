#ifndef LIGHTKNIGHT_TYPES_H
#define LIGHTKNIGHT_TYPES_H

#include <cstdint>
#include <cstddef>

namespace lightknight {
    const size_t kNumPieces = 13;
    enum class Pieces : uint8_t {
        kWhitePawn = 0,
        kWhiteKnight,
        kWhiteBishop,
        kWhiteRook,
        kWhiteQueen,
        kWhiteKing,
        kBlackPawn,
        kBlackKnight,
        kBlackBishop,
        kBlackRook,
        kBlackQueen,
        kBlackKing,
        kEmpty
    };

    const size_t kNumColors = 2;
    enum class Colors : uint8_t {
        kWhite = 0,
        kBlack
    };

    const size_t kNumCastles = 4;
    enum class Castles : uint8_t {
        kWhiteQueenSide = 1 << 0,
        kWhiteKingSide = 1 << 1,
        kBlackQueenSide = 1 << 2,
        kBlackKingSide = 1 << 3
    };

    // File / rank bitboard masks.
    static constexpr uint64_t kFileA = 0x0101010101010101ULL;
    static constexpr uint64_t kFileB = kFileA << 1;
    static constexpr uint64_t kFileC = kFileA << 2;
    static constexpr uint64_t kFileD = kFileA << 3;
    static constexpr uint64_t kFileE = kFileA << 4;
    static constexpr uint64_t kFileF = kFileA << 5;
    static constexpr uint64_t kFileG = kFileA << 6;
    static constexpr uint64_t kFileH = kFileA << 7;
    static constexpr uint64_t kRank1 = 0x00000000000000ffULL;
    static constexpr uint64_t kRank2 = kRank1 << 8;
    static constexpr uint64_t kRank3 = kRank1 << 16;
    static constexpr uint64_t kRank4 = kRank1 << 24;
    static constexpr uint64_t kRank5 = kRank1 << 32;
    static constexpr uint64_t kRank6 = kRank1 << 40;
    static constexpr uint64_t kRank7 = kRank1 << 48;
    static constexpr uint64_t kRank8 = kRank1 << 56;

    const size_t kNumSquares = 64;
    enum class Square : uint8_t {
        A1, B1, C1, D1, E1, F1, G1, H1,
        A2, B2, C2, D2, E2, F2, G2, H2,
        A3, B3, C3, D3, E3, F3, G3, H3,
        A4, B4, C4, D4, E4, F4, G4, H4,
        A5, B5, C5, D5, E5, F5, G5, H5,
        A6, B6, C6, D6, E6, F6, G6, H6,
        A7, B7, C7, D7, E7, F7, G7, H7,
        A8, B8, C8, D8, E8, F8, G8, H8,
    };

    // Directional moving, handles edges correctly.
    inline constexpr uint64_t North(uint64_t bitboard) {return bitboard << 8;}
    inline constexpr uint64_t South(uint64_t bitboard) {return bitboard >> 8;}
    inline constexpr uint64_t East(uint64_t bitboard) {return (bitboard & ~kFileH) << 1;}
    inline constexpr uint64_t West(uint64_t bitboard) {return (bitboard & ~kFileA) >> 1;}
    inline constexpr uint64_t NorthEast(uint64_t bitboard) {return North(East(bitboard));}
    inline constexpr uint64_t SouthEast(uint64_t bitboard) {return South(East(bitboard));}
    inline constexpr uint64_t SouthWest(uint64_t bitboard) {return South(West(bitboard));}
    inline constexpr uint64_t NorthWest(uint64_t bitboard) {return North(West(bitboard));}

    // For log2 of power of 2.
    static const uint64_t kDeBruijeMagic = 0x03F79D71B4CB0A89ULL;
    static const std::array<uint8_t, 64> kDeBruije = {
         0,  1, 48,  2, 57, 49, 28,  3,
        61, 58, 50, 42, 38, 29, 17,  4,
        62, 55, 59, 36, 53, 51, 43, 22,
        45, 39, 33, 30, 24, 18, 12,  5,
        63, 47, 56, 27, 60, 41, 37, 16,
        54, 35, 52, 21, 44, 32, 23, 11,
        46, 26, 40, 15, 34, 20, 31, 10,
        25, 14, 19, 9, 13,  8,  7,  6
    };
    inline uint64_t LSB(uint64_t bitboard) {return bitboard & -bitboard;}

    inline size_t SetBitsCount(uint64_t bitboard) {
        size_t count = 0;
        while (bitboard) {
            bitboard &= bitboard - 1;
            ++count;
        }
        return count;
    }

    // Expects the bitboard to only have one set bit, to be a power of 2.
    inline Square BitboardToSquare(uint64_t bitboard){
        return static_cast<Square>(kDeBruije[(bitboard * kDeBruijeMagic) >> 58]);
    }

    inline Square LSBSquare(uint64_t bitboard) {return BitboardToSquare(LSB(bitboard));};
    inline constexpr uint64_t SquareToBitboard(Square square) { return 1ULL << static_cast<uint8_t>(square);}
    inline constexpr int Rank(Square square) {return static_cast<int>(square) / 8;}
    inline constexpr int File(Square square) {return static_cast<int>(square) % 8;}
}; // namespace lightknight

#endif //LIGHTKNIGHT_TYPES_H