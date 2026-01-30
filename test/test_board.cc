// test_board.cc
#include <catch2/catch_test_macros.hpp>
#include "board.h"
#include "exceptions.h"
#include <array>

TEST_CASE("Board Initialization Invalid FEN strings", "[UnitTest][BoardInitialization][FEN]") {
    static const std::array<std::pair<std::string, std::string>, 22> tests = {
        std::make_pair(std::string("rnbqkbnr/pppppppp/8/8/8/PPPPPPPP/RNBQBNR w KQkq - 0 1"),     std::string("Too few ranks specified")),
        std::make_pair(std::string("rnbqkbnr/pppppppp/8/8/8/8/8/PPPPPPPP/RNBQBNR w KQkq - 0 1"), std::string("Too many ranks specified")),
        std::make_pair(std::string("rnbqkbnr/pppppppp/8/9/9/PPPPPPPppP/RNBQBNR w KQkq - 0 1"),   std::string("Too many files specified, last rank")),
        std::make_pair(std::string("rnbqkbnr/pppppppp/8/9/9/PPPPPPPppP/RNBQBNRr w KQkq - 0 1"),  std::string("Too many files specified")),
        std::make_pair(std::string("rnbqkbnr/pppp/8/8/5/PPPPPPPP/2B3 w KQkq - 0 1"),             std::string("Too few files specified")),
        std::make_pair(std::string("rnbqkbnr//8/8/8/PPPPPPPP/RNBQBNR w KQkq - 0 1"),             std::string("No piece specified on file")),
        std::make_pair(std::string("rnblkbnr/pppppppp/8/8/8/1p6/PrrPPjPP/RNBQWNR w KQkq - 0 1"), std::string("Invalid pieces")),
        std::make_pair(std::string("rnbqkbnr/pppppppp/8/8/8/PPPPPPPP/RNBQBNR KQkq - 0 1"),       std::string("No turn char")),
        std::make_pair(std::string("rnbqkbnr/pppppppp/8/8/8/PPPPPPPP/RNBQBNR wwb KQkq - 0 1"),   std::string("Too many turn chars")),
        std::make_pair(std::string("rnbqkbnr/pppppppp/8/8/8/PPPPPPPP/RNBQBNR t KQkq - 0 1"),     std::string("Invalid turn char")),
        std::make_pair(std::string("rnbqkbnr/pppppppp/8/8/8/PPPPPPPP/RNBQBNR w c - 0 1"),        std::string("Invalid castling char")),
        std::make_pair(std::string("rnbqkbnr/pppppppp/8/8/8/PPPPPPPP/RNBQBNR w KQ- - 0 1"),      std::string("Invalid castling chars with -")),
        std::make_pair(std::string("rnbqkbnr/pppppppp/8/8/8/PPPPPPPP/RNBQBNR w 0 1"),            std::string("No castling chars")),
        std::make_pair(std::string("rnbqkbnr/pppppppp/8/8/8/PPPPPPPP/RNBQBNR w KQkqK - 0 1"),    std::string("Too many castling chars")),
        std::make_pair(std::string("rnbqkbnr/pppppppp/8/8/8/PPPPPPPP/RNBQBNR w KQkq z3 0 1"),    std::string("Wrong en passant file")),
        std::make_pair(std::string("rnbqkbnr/pppppppp/8/8/8/PPPPPPPP/RNBQBNR w - e9 0 1"),       std::string("Wrong en passant rank")),
        std::make_pair(std::string("rnbqkbnr/pppppppp/8/8/8/PPPPPPPP/RNBQBNR w - e28z 0 1"),     std::string("Too many chars en passant")),
        std::make_pair(std::string("rnbqkbnr/pppppppp/8/8/8/PPPPPPPP/RNBQBNR w - a 0 1"),        std::string("Too few chars en passant")),
        std::make_pair(std::string("rnbqkbnr/pppppppp/8/8/8/PPPPPPPP/RNBQBNR w KQkq - plm gcc"), std::string("Invalid halfmove/fullmove")),
        std::make_pair(std::string("8/8/8/8/8/8/8/8 w - -"),                                     std::string("Too few fields specified")),
        std::make_pair(std::string("8/8/8/8/8/8/8/8 w K - 0 1 10"),                              std::string("Too many fields specified")),
        std::make_pair(std::string("bnbnn/153/nn22 ffff as/2.1//"),                              std::string("Junk FEN string")),     
    };

    for (const auto& [fen, description] : tests) {
        SECTION(fen) {
            INFO("Description: " << description);
            REQUIRE_THROWS_AS(lightknight::Board(fen), lightknight::exceptions::FENException);
        }
    }
}

void CheckBoard(const lightknight::Board& actual, const lightknight::Board& expected) {
    std::array<std::string, lightknight::kNumPieces> bitboard_names = {
        "White Pawn Bitboard", "White Knight Bitboard", "White Bishop Bitboard", "White Rook Bitboard",
        "White Queen Bitboard", "White King Bitboard", "Black Pawn Bitboard", "Black Knight Bitboard",
        "Black Bishop Bitboard", "Black Rook Bitboard", "Black Queen Bitboard", "Black King Bitboard",
        "Empty Squares Bitboard"
    };
    for (std::size_t idx=0; idx < lightknight::kNumPieces; ++idx) {
        SECTION(bitboard_names[idx]) {
            INFO("Expected: 0x" << std::hex << expected.piece_bitboards[idx]);
            INFO("Actual: 0x" << std::hex << actual.piece_bitboards[idx]);
            CHECK(actual.piece_bitboards[idx] == expected.piece_bitboards[idx]);
        }
    }

    SECTION("Turn") {
        CHECK(actual.turn == expected.turn);
    }

    SECTION("Castling Rights") {
        CHECK(actual.castling == expected.castling);
    }

    SECTION("En Passant Square") {
        CHECK(actual.en_passant == expected.en_passant);
    }

    SECTION("Halfmove clock") {
        CHECK(actual.halfmoves == expected.halfmoves);
    }

    SECTION("Fullmove clock") {
        CHECK(actual.fullmoves == expected.fullmoves);
    }
}

TEST_CASE("Board Initialization Valid FEN String", "[UnitTest][BoardInitialization][FEN]") {
    static const std::array<std::pair<std::string, lightknight::Board>, 3> tests = {
        std::make_pair(
            std::string("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"),
            lightknight::Board::FromRaw(
                {
                    0x000000000000FF00ULL,
                    0x0000000000000042ULL,
                    0x0000000000000024ULL,
                    0x0000000000000081ULL,
                    0x0000000000000008ULL,
                    0x0000000000000010ULL,
                    0x00FF000000000000ULL,
                    0x4200000000000000ULL,
                    0x2400000000000000ULL,
                    0x8100000000000000ULL,
                    0x0800000000000000ULL,
                    0x1000000000000000ULL,
                    0x0000FFFFFFFF0000ULL
                },
                lightknight::Color::kWhite,
                0b1111,
                -1,
                0,
                1)),
        std::make_pair(
            std::string("r3k2r/pp1q1pp1/2p1pn2/4B2p/2BP4/8/PPP1QPP1/2KR3R b kq - 1 16"),
            lightknight::Board::FromRaw(
                {
                    0x0000000008006700ULL,
                    0x0000000000000000ULL,
                    0x0000001004000000ULL,
                    0x0000000000000088ULL,
                    0x0000000000001000ULL,
                    0x0000000000000004ULL,
                    0x0063148000000000ULL,
                    0x0000200000000000ULL,
                    0x0000000000000000ULL,
                    0x8100000000000000ULL,
                    0x0008000000000000ULL,
                    0x1000000000000000ULL,
                    0x6e94cb6ff3ff8873ULL

                },
                lightknight::Color::kBlack,
                0b1100,
                -1,
                1,
                16)),
        std::make_pair(
            std::string("6k1/5pbp/6p1/2pNp3/P1P1P3/5P2/1P3KPP/8 b - a3 0 30"),
            lightknight::Board::FromRaw(
                {
                    0x000000001520c200ULL,
                    0x0000000800000000ULL,
                    0x0000000000000000ULL,
                    0x0000000000000000ULL,
                    0x0000000000000000ULL,
                    0x0000000000002000ULL,
                    0x00a0401400000000ULL,
                    0x0000000000000000ULL,
                    0x0040000000000000ULL,
                    0x0000000000000000ULL,
                    0x0000000000000000ULL,
                    0x4000000000000000ULL,
                    0xbf1fbfe3eadf1dffULL
                },
                lightknight::Color::kBlack,
                0b0000,
                16,
                0,
                30))
    };

    for (auto& test : tests) {
        const std::string& fen_str = test.first;
        const lightknight::Board& expected_board = test.second;
        lightknight::Board board;
            
        SECTION(fen_str) {    
            REQUIRE_NOTHROW(board.FromFEN(fen_str));
            CheckBoard(board, expected_board);
        }
    }
}