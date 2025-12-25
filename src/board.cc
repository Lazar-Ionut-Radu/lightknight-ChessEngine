#include "board.h"
#include "exceptions.h"

#include <array>
#include <string>
#include <cstdint>
#include <regex>
#include <iostream>
#include <unordered_map>
#include <algorithm>

namespace lightknight::engine {
    void Board::FromFEN(const std::string& fen) {
        // Validate the format of a fen string
        std::regex fen_regex(
            R"(^(?:([PNBRQKpnbrqk1-8/]+)\s)"
            R"(([bw])\s)"
            R"((-|[KQkq]{1,4})\s)"
            R"((-|[a-h][36])\s)"
            R"((\d+)\s)"
            R"((\d+)$))"
        );

        std::smatch match;
        if (!std::regex_match(fen, match, fen_regex)) {
            throw lightknight::exceptions::FENException("Invalid FEN string");
        }

        // Extract each section
        std::string placement_str  = match[1];
        std::string turn_str       = match[2];
        std::string castling_str   = match[3];
        std::string en_passant_str = match[4];
        std::string halfmove_str   = match[5];
        std::string fullmove_str   = match[6];

        // Setup the pieces on the board.
        static const std::unordered_map<char, lightknight::engine::Pieces> char_to_piece = {
            {'P', lightknight::engine::Pieces::kWhitePawn}, {'p', lightknight::engine::Pieces::kBlackPawn},
            {'N', lightknight::engine::Pieces::kWhiteKnight}, {'n', lightknight::engine::Pieces::kBlackKnight},
            {'B', lightknight::engine::Pieces::kWhiteBishop}, {'b', lightknight::engine::Pieces::kBlackBishop},
            {'R', lightknight::engine::Pieces::kWhiteRook}, {'r', lightknight::engine::Pieces::kBlackRook},
            {'Q', lightknight::engine::Pieces::kWhiteQueen}, {'q', lightknight::engine::Pieces::kBlackQueen},
            {'K', lightknight::engine::Pieces::kWhiteKing}, {'k', lightknight::engine::Pieces::kBlackKing},
        };
        int file = 0, rank = 7;
        for (int i = 0; i < placement_str.size(); ++i) {
            // Go on the next rank
            if (placement_str[i] == '/') {
                file = 0;
                rank--;

                if (rank < 0)
                    throw lightknight::exceptions::FENException("Invalid FEN string ops");
            }
            else {
                // Empty spaces
                if (placement_str[i] < 'A') {
                    for (int num = 1; num <= (int)(placement_str[i] - '0'); ++num) {
                        this->bitboards[static_cast<int>(lightknight::engine::Pieces::kEmpty)] |= 1ULL << (8*rank + file);
                        file++;

                        if (file > 8)
                            throw lightknight::exceptions::FENException("Invalid FEN string ops");
                    }
                }
                // Pieces
                else {
                    this->bitboards[static_cast<int>(char_to_piece.at(placement_str[i]))] |= 1ULL << (8*rank + file);
                    file++;

                    if (rank > 8)
                        throw lightknight::exceptions::FENException("Invalid FEN string ops");
                }
            }
        }
        if (rank > 0)
            throw lightknight::exceptions::FENException("Invalid FEN string ops");

        // Setup the turn
        if (turn_str[0] == 'w')
            this->turn = lightknight::engine::Colors::kWhite;
        else
            this->turn = lightknight::engine::Colors::kBlack;
        
        // Setup the castling rights
        if (castling_str[0] != '-') {
            for (char c : castling_str) {
                switch (c) {
                    case 'K':
                        this->castling |= static_cast<uint8_t>(lightknight::engine::Castles::kWhiteKingSide);
                        break;
                    case 'Q':
                        this->castling |= static_cast<uint8_t>(lightknight::engine::Castles::kWhiteQueenSide);
                        break;
                    case 'k':
                        this->castling |= static_cast<uint8_t>(lightknight::engine::Castles::kBlackKingSide);
                        break;
                    case 'q':
                        this->castling |= static_cast<uint8_t>(lightknight::engine::Castles::kBlackQueenSide);
                        break;
                }
            }
        }

        // Setup the en passant square
        if (en_passant_str[0] == '-')
            this->en_passant = 0ULL;
        else {
            int file = (int)(en_passant_str[0] - 'a');
            int rank = (int)(en_passant_str[1] - '1');
            this->en_passant = 1ULL << (8*rank+file);
        }

        // Setup the halfmove & fullmove clocks.
        this->halfmoves = this->fullmoves = 0;

        if (halfmove_str[0] != '-')
            for (int i = 0; i < halfmove_str.size(); ++i)
                this->halfmoves = 10 * this->halfmoves + (int)(halfmove_str[i] - '0');
                
        if (fullmove_str[0] != '-')
            for (int i = 0; i < fullmove_str.size(); ++i)
                this->fullmoves = 10 * this->fullmoves + (int)(fullmove_str[i] - '0');
    }

    Board Board::FromRaw(
        const std::array<uint64_t, kNumPieces>& bitboards,
        Colors turn,
        uint8_t castling,
        int en_passant_square,
        int halfmoves,
        int fullmoves
    ) {
        Board board;
        board.bitboards = bitboards;
        board.turn = turn;
        board.castling = castling;
        board.en_passant = (en_passant_square < 0) ? 0ULL : (1ULL << en_passant_square);
        board.halfmoves = halfmoves;
        board.fullmoves = fullmoves;

        return board;
    }

    Board::Board(const std::string& fen) : Board() {
        this->FromFEN(fen);
    }

    Board::Board() {
        this->bitboards = {0ULL};
        this->castling = 0;
        this->en_passant = 0ULL;
        this->halfmoves = 0;
        this->fullmoves = 1;
        this->turn = lightknight::engine::Colors::kWhite;
    }
} // namespace lightknight::engine
