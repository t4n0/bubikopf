#include "bitboard/fen_conversion.h"

#include "bitboard/board.h"
#include "bitboard/squares.h"

#include <algorithm>
#include <iterator>
#include <sstream>

namespace Chess
{

Position PositionFromFen(const std::string& fen)
{
    Position position{};

    const std::vector<std::string> tokens = TokenizeFen(fen);

    if (tokens.size() != 6)
    {
        throw std::runtime_error{"FEN contains invalid number of tokens."};
    }

    const std::string pieces = tokens.at(kFenTokenPieces);
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

    const char side_to_move = tokens.at(kFenTokenSide).front();
    switch (side_to_move)
    {
        case 'w':
            position.white_to_move_ = true;
            position.attacking_side_ = BOARD_IDX_WHITE;
            position.defending_side_ = BOARD_IDX_BLACK;
            break;
        case 'b':
            position.white_to_move_ = false;
            position.attacking_side_ = BOARD_IDX_BLACK;
            position.defending_side_ = BOARD_IDX_WHITE;
            break;
        default:
            throw std::runtime_error{"FEN contains invalid token for side to play."};
    }

    const std::string castling_rights = tokens.at(kFenTokenCastling);
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

    const std::string en_passant = tokens.at(kFenTokenEnPassant);
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

    const Bitboard static_plies = std::stoi(tokens.at(kFenTokenStaticPlies));
    position[BOARD_IDX_EXTRAS] |= static_plies;

    const Bitboard total_plies = std::stoi(tokens.at(kFenTokenMoves));

    position[BOARD_IDX_EXTRAS] |= total_plies << BOARD_SHIFT_MOVES;

    return position;
}

std::string FenFromPosition(const Position& position)
{
    std::string pieces{};
    int empty_squares = 0;
    int squares_covered_in_current_rank = 0;
    for (int idx = 63; idx >= 0; idx--)
    {
        const Bitboard square = Bitboard{1} << idx;
        const Bitmove white_piece = position.GetPieceKind(BOARD_IDX_WHITE, square);
        const Bitmove black_piece = position.GetPieceKind(BOARD_IDX_BLACK, square);
        if (white_piece)
        {
            if (empty_squares)
            {
                pieces += std::to_string(empty_squares);
                empty_squares = 0;
            }
            pieces += PIECE_LABEL_WHITE.at(white_piece);
        }
        else if (black_piece)
        {
            if (empty_squares)
            {
                pieces += std::to_string(empty_squares);
                empty_squares = 0;
            }
            pieces += PIECE_LABEL.at(black_piece);
        }
        else
        {
            empty_squares++;
        }

        squares_covered_in_current_rank++;
        if (squares_covered_in_current_rank == 8)
        {
            squares_covered_in_current_rank = 0;
            if (empty_squares)
            {
                pieces += std::to_string(empty_squares);
                empty_squares = 0;
            }
            if (idx)
            {
                pieces += "/";
            }
        }
    }

    const auto side = position.white_to_move_ ? "w" : "b";

    std::string castling{};
    if (position[BOARD_IDX_EXTRAS] & BOARD_VALUE_CASTLING_WHITE_KINGSIDE)
    {
        castling += "K";
    }
    if (position[BOARD_IDX_EXTRAS] & BOARD_VALUE_CASTLING_WHITE_QUEENSIDE)
    {
        castling += "Q";
    }
    if (position[BOARD_IDX_EXTRAS] & BOARD_VALUE_CASTLING_BLACK_KINGSIDE)
    {
        castling += "k";
    }
    if (position[BOARD_IDX_EXTRAS] & BOARD_VALUE_CASTLING_BLACK_QUEENSIDE)
    {
        castling += "q";
    }
    const bool at_least_one_side_can_castle = position[BOARD_IDX_EXTRAS] & BOARD_MASK_CASTLING;
    if (!at_least_one_side_can_castle)
    {
        castling += "-";
    }

    const auto en_passant_square_bits = (position[BOARD_IDX_EXTRAS] & BOARD_MASK_EN_PASSANT) >> BOARD_SHIFT_EN_PASSANT;
    const auto en_passant = en_passant_square_bits ? SQUARE_LABEL.at(en_passant_square_bits) : "-";

    const auto static_plies = std::to_string(position[BOARD_IDX_EXTRAS] & BOARD_MASK_STATIC_PLIES);

    const auto total_plies = std::to_string((position[BOARD_IDX_EXTRAS] & BOARD_MASK_MOVES) >> BOARD_SHIFT_MOVES);

    return pieces + " " + side + " " + castling + " " + en_passant + " " + static_plies + " " + total_plies;
}

std::vector<std::string> TokenizeFen(const std::string& fen)
{
    std::istringstream iss{fen};
    return {std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};
}

}  // namespace Chess