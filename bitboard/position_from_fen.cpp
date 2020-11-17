#include "bitboard/position_from_fen.h"

#include "bitboard/board.h"
#include "bitboard/squares.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

namespace Chess
{

PositionWithBitboards PositionFromFen(const std::string& fen)
{
    PositionWithBitboards position{};

    std::istringstream iss{fen};
    const std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
                                          std::istream_iterator<std::string>{}};

    if (tokens.size() != 6)
    {
        throw std::runtime_error{"FEN contains invalid number of tokens."};
    }

    const std::string pieces = tokens.at(0);
    Bitboard current_square = A8;
    for (const char symbol : pieces)
    {
        if (symbol != '/')
        {
            const int empty_squares = static_cast<int>(symbol - '0');
            const bool is_number_of_empty_squares = (empty_squares >= 1) && (empty_squares <= 8);
            if (is_number_of_empty_squares)
            {
                current_square >>= empty_squares;
            }
            else
            {
                std::size_t side{};
                std::size_t piece_kind{};

                switch (symbol)
                {
                    case 'p':
                        side = BOARD_IDX_BLACK;
                        piece_kind = PAWN;
                        break;
                    case 'r':
                        side = BOARD_IDX_BLACK;
                        piece_kind = ROOK;
                        break;
                    case 'n':
                        side = BOARD_IDX_BLACK;
                        piece_kind = KNIGHT;
                        break;
                    case 'b':
                        side = BOARD_IDX_BLACK;
                        piece_kind = BISHOP;
                        break;
                    case 'q':
                        side = BOARD_IDX_BLACK;
                        piece_kind = QUEEN;
                        break;
                    case 'k':
                        side = BOARD_IDX_BLACK;
                        piece_kind = KING;
                        break;
                    case 'P':
                        side = BOARD_IDX_WHITE;
                        piece_kind = PAWN;
                        break;
                    case 'R':
                        side = BOARD_IDX_WHITE;
                        piece_kind = ROOK;
                        break;
                    case 'N':
                        side = BOARD_IDX_WHITE;
                        piece_kind = KNIGHT;
                        break;
                    case 'B':
                        side = BOARD_IDX_WHITE;
                        piece_kind = BISHOP;
                        break;
                    case 'Q':
                        side = BOARD_IDX_WHITE;
                        piece_kind = QUEEN;
                        break;
                    case 'K':
                        side = BOARD_IDX_WHITE;
                        piece_kind = KING;
                        break;
                    default:
                        throw std::runtime_error{"FEN contains invalid piece symbol: " + symbol};
                }
                position[side] |= current_square;
                position[side + piece_kind] |= current_square;
                current_square >>= 1;
            }
        }
    }

    const char side_to_move = tokens.at(1).front();
    switch (side_to_move)
    {
        case 'w':
            position[BOARD_IDX_EXTRAS] |= BOARD_MASK_WHITE_TURN;
            break;
        case 'b':
            position[BOARD_IDX_EXTRAS] |= BOARD_MASK_BLACK_TURN;
            break;
        default:
            throw std::runtime_error{"FEN contains invalid token for side to play."};
    }

    const std::string castling_rights = tokens.at(2);
    if (castling_rights != "-")
    {
        for (const auto& castling_right : castling_rights)
        {
            switch (castling_right)
            {
                case 'k':
                    position[BOARD_IDX_EXTRAS] |= BOARD_VALUE_CASTLING_BLACK_KINGSIDE;
                    break;
                case 'q':
                    position[BOARD_IDX_EXTRAS] |= BOARD_VALUE_CASTLING_BLACK_QUEENSIDE;
                    break;
                case 'K':
                    position[BOARD_IDX_EXTRAS] |= BOARD_VALUE_CASTLING_WHITE_KINGSIDE;
                    break;
                case 'Q':
                    position[BOARD_IDX_EXTRAS] |= BOARD_VALUE_CASTLING_WHITE_QUEENSIDE;
                    break;
                default:
                    throw std::runtime_error{"FEN contains invalid castling token."};
            }
        }
    }

    const std::string en_passant = tokens.at(3);
    if (en_passant != "-")
    {
        const auto location_it = std::find(SQUARE_LABEL.begin(), SQUARE_LABEL.end(), en_passant);
        if (location_it == SQUARE_LABEL.end())
        {
            throw std::runtime_error{"FEN contains invalid en passant token."};
        }

        const int en_passant_bits = std::distance(SQUARE_LABEL.begin(), location_it);
        position[BOARD_IDX_EXTRAS] |= en_passant_bits << BOARD_SHIFT_EN_PASSANT;
    }

    const int static_plies = std::stoi(tokens.at(4));
    position[BOARD_IDX_EXTRAS] |= static_plies;

    return position;
}

}  // namespace Chess