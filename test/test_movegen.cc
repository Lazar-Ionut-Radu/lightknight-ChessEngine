// test_movegen.cc
#include <catch2/catch_test_macros.hpp>
#include "movegen.h"
#include "types.h"
#include <array>
#include <tuple>

TEST_CASE("Sliding Pieces Relevant Occupacy (for magic bitboards generation)", "[UnitTest][MoveGeneration][MagicBitboards]") {
    static const std::array<std::pair<lightknight::Square, uint64_t>, 10> bishop_tests = {
        std::make_pair(lightknight::Square::A1, 0x40201008040200ULL),
        std::make_pair(lightknight::Square::H8, 0x40201008040200ULL),
        std::make_pair(lightknight::Square::A8, 0x2040810204000ULL),
        std::make_pair(lightknight::Square::H1, 0x2040810204000ULL),
        std::make_pair(lightknight::Square::D1, 0x40221400ULL),
        std::make_pair(lightknight::Square::H2, 0x4081020400000ULL),
        std::make_pair(lightknight::Square::F8, 0x50080402000000ULL),
        std::make_pair(lightknight::Square::C6, 0xa000a10204000ULL),
        std::make_pair(lightknight::Square::E5, 0x44280028440200ULL),
        std::make_pair(lightknight::Square::B7, 0x40810204000ULL)
    };

    static const std::array<std::pair<lightknight::Square, uint64_t>, 10> rook_tests = {
        std::make_pair(lightknight::Square::A1, 0x101010101017eULL),
        std::make_pair(lightknight::Square::H1, 0x8080808080807eULL),
        std::make_pair(lightknight::Square::H8, 0x7e80808080808000ULL),
        std::make_pair(lightknight::Square::H1, 0x8080808080807eULL),
        std::make_pair(lightknight::Square::A5, 0x1017e01010100ULL),
        std::make_pair(lightknight::Square::G1, 0x4040404040403eULL),
        std::make_pair(lightknight::Square::G7, 0x3e404040404000ULL),
        std::make_pair(lightknight::Square::E4, 0x1010106e101000ULL),
        std::make_pair(lightknight::Square::D2, 0x8080808087600ULL),
        std::make_pair(lightknight::Square::C5, 0x4047a04040400ULL)
    };

    SECTION("Bishop") {
        for (auto& [sq, bb] : bishop_tests) {
            CHECK(lightknight::movegen::kBishopRelevantOccupancy[static_cast<int>(sq)] == bb);
        }
    }

    SECTION("Rook") {
        for (auto& [sq, bb] : rook_tests) {
            CHECK(lightknight::movegen::kRookRelevantOccupancy[static_cast<int>(sq)] == bb);
        }
    }
}

TEST_CASE("Precomputed Attack Bitboards", "[UnitTest][MoveGeneration][MagicBitboards]") {
    SECTION("Knight") {
        static const std::array<std::pair<lightknight::Square, uint64_t>, 8> knight_tests = {
            std::make_pair(lightknight::Square::A1, 0x20400ULL),
            std::make_pair(lightknight::Square::B7, 0x800080500000000ULL),
            std::make_pair(lightknight::Square::G1, 0xa01000ULL),
            std::make_pair(lightknight::Square::G3, 0xa0100010a0ULL),
            std::make_pair(lightknight::Square::F6, 0x5088008850000000ULL),
            std::make_pair(lightknight::Square::D5, 0x14220022140000ULL),
            std::make_pair(lightknight::Square::E8, 0x44280000000000ULL),
            std::make_pair(lightknight::Square::A2, 0x2040004ULL)
        };

        for (auto& [sq, bb] : knight_tests) {
            CHECK(lightknight::movegen::kKnightAttacks[static_cast<int>(sq)] == bb);
        }
    }

    SECTION("Bishop") {
        static const std::array<std::tuple<lightknight::Square, uint64_t, uint64_t>, 13> bishop_tests = {
            std::make_tuple(lightknight::Square::A1, 0ULL, 0x8040201008040200ULL),
            std::make_tuple(lightknight::Square::E5, 0ULL, 0x8244280028448201ULL),
            std::make_tuple(lightknight::Square::G7, 0ULL, 0xa000a01008040201ULL),
            std::make_tuple(lightknight::Square::F1, 0ULL, 0x10204885000ULL),
            std::make_tuple(lightknight::Square::D6, 0ULL, 0x2214001422418000ULL),
            std::make_tuple(lightknight::Square::D4, 0x430100003000008ULL, 0x8041221400142241ULL),
            std::make_tuple(lightknight::Square::C1, 0x91404040440ULL, 0x804020110a00ULL),
            std::make_tuple(lightknight::Square::H1, 0xf80020000100bf7fULL, 0x102040810204000ULL),
            std::make_tuple(lightknight::Square::D2, 0x404000040000ULL, 0x4020140014ULL),
            std::make_tuple(lightknight::Square::E1, 0x40000ULL, 0x80442800ULL),
            std::make_tuple(lightknight::Square::E5, 0x280028000000ULL, 0x280028000000ULL),
            std::make_tuple(lightknight::Square::E5, 0x8200000000008001ULL, 0x8244280028448201ULL),
            std::make_tuple(lightknight::Square::C5, 0x24040002207ULL, 0x20100a000a112000ULL)
        };

        for (auto& [sq, blockers, bb] : bishop_tests) {
            CHECK(lightknight::movegen::BishopAttackBitboard(sq, blockers) == bb);
        }

    }

    SECTION("Rook") {
        static const std::array<std::tuple<lightknight::Square, uint64_t, uint64_t>, 14> rook_tests = {
            std::make_tuple(lightknight::Square::A1, 0ULL, 0x1010101010101feULL),
            std::make_tuple(lightknight::Square::B4, 0ULL, 0x2020202fd020202ULL),
            std::make_tuple(lightknight::Square::G1, 0ULL, 0x40404040404040bfULL),
            std::make_tuple(lightknight::Square::H2, 0ULL, 0x8080808080807f80ULL),
            std::make_tuple(lightknight::Square::E7, 0x40307030303ULL, 0x10ef101010101010ULL),
            std::make_tuple(lightknight::Square::H8, 0x40307030303ULL, 0x7f80808080808080ULL),
            std::make_tuple(lightknight::Square::F8, 0x1800400000001ULL, 0xdf20202020202020ULL),
            std::make_tuple(lightknight::Square::B3, 0x200200200ULL, 0x2023d0200ULL),
            std::make_tuple(lightknight::Square::H1, 0x80080080001cULL, 0x808070ULL),
            std::make_tuple(lightknight::Square::C5, 0x4023300000000ULL, 0x4041a04040404ULL),
            std::make_tuple(lightknight::Square::E2, 0x10202000ULL, 0x10102f10ULL),
            std::make_tuple(lightknight::Square::F1, 0x2004ULL, 0x20dcULL),
            std::make_tuple(lightknight::Square::B1, 0x205ULL, 0x205ULL),
            std::make_tuple(lightknight::Square::A8, 0x201000000000000ULL, 0x201000000000000ULL)
        };
        
        for (auto& [sq, blockers, bb] : rook_tests) {
            CHECK(lightknight::movegen::RookAttackBitboard(sq, blockers) == bb);
        }
    }

    SECTION("Queen") {
        static const std::array<std::tuple<lightknight::Square, uint64_t, uint64_t>, 13> queen_tests = {
            std::make_tuple(lightknight::Square::A1, 0ULL, 0x81412111090503feULL),
            std::make_tuple(lightknight::Square::D1, 0ULL, 0x8080888492a1cf7ULL),
            std::make_tuple(lightknight::Square::G7, 0ULL, 0xe0bfe05048444241ULL),
            std::make_tuple(lightknight::Square::D5, 0ULL, 0x492a1cf71c2a4988ULL),
            std::make_tuple(lightknight::Square::B4, 0x6183c0a10ULL, 0x70d070200ULL),
            std::make_tuple(lightknight::Square::A8, 0x20202020301c0700ULL, 0x3e03050911010100ULL),
            std::make_tuple(lightknight::Square::F8, 0x440082000000000ULL, 0xdc70282000000000ULL),
            std::make_tuple(lightknight::Square::D3, 0x440082000021c00ULL, 0x92a1cf61c00ULL),
            std::make_tuple(lightknight::Square::F3, 0x200000200000ULL, 0x10224a870df70a8ULL),
            std::make_tuple(lightknight::Square::G7, 0x40000000000000ULL, 0xe0bfe05048444241ULL),
            std::make_tuple(lightknight::Square::F5, 0x1000010000030088ULL, 0x24a870df70a82422ULL),
            std::make_tuple(lightknight::Square::H6, 0x1f3f800000000703ULL, 0xa0c07fc0a0908884ULL),
            std::make_tuple(lightknight::Square::H1, 0x7e7d7b776f5f3f00ULL, 0x8182848890a0c07fULL),
        };

        for (auto& [sq, blockers, bb] : queen_tests) {
            CHECK(lightknight::movegen::QueenAttackBitboard(sq, blockers) == bb);
        }
    }

    SECTION("King") {
        static const std::array<std::pair<lightknight::Square, uint64_t>, 6> king_tests = {
            std::make_pair(lightknight::Square::H1, 0xc040ULL),
            std::make_pair(lightknight::Square::H3, 0xc040c000ULL),
            std::make_pair(lightknight::Square::C6, 0xe0a0e00000000ULL),
            std::make_pair(lightknight::Square::G8, 0xa0e0000000000000ULL),
            std::make_pair(lightknight::Square::F3, 0x70507000ULL),
            std::make_pair(lightknight::Square::A8, 0x203000000000000ULL),
        };

        for (auto& [sq, bb] : king_tests) {
            CHECK(lightknight::movegen::kKingAttacks[static_cast<int>(sq)] == bb);
        }
    }
}