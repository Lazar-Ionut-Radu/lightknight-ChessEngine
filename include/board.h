#ifndef LIGHTKNIGHT_BOARD_H
#define LIGHTKNIGHT_BOARD_H

#include <cstdint>
#include <array>
#include <string>

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

    class Board {
    public:
        // Array of bitboards, one for each piece type, specifying their positions on the table.
        std::array<uint64_t, kNumPieces> bitboards;
        uint8_t castling;
        lightknight::Colors turn;
        // Specifies the target square (if any) an en passant capture can be done.
        uint64_t en_passant;
        // Counts the number of half moves (one side) since the last capture or pawn advancement.
        // Used for the 50 rule move.
        int halfmoves;
        // Counts the number of moves played in the game. Starts at 1 and is incremented after
        // black's move.
        int fullmoves;

        // Constructors
        Board();
        explicit Board(const std::string& fen);
        ~Board() = default;

        // Only checks if the fen string is valid, not if the resulting position is.
        void FromFEN(const std::string& fen);

        // Does not check the validity of the data, only for testing purposes.
        static Board FromRaw(
            const std::array<uint64_t, kNumPieces>& bitboards,
            Colors turn,
            uint8_t castling,
            int en_passant_square,
            int halfmoves,
            int fullmoves
        );

    };
} // namespace lightknight

#endif // LIGHTKNIGHT_BOARD_H
