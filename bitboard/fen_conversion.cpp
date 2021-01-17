#include "bitboard/fen_conversion.h"

#include "bitboard/board.h"
#include "bitboard/squares.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>

namespace Chess
{

namespace
{
int ConvertToInteger(const char charater)
{
    return static_cast<int>(charater - '0');
}
bool IsNumberOfEmptySquares(const int candidate_number)
{
    return (candidate_number >= 1) && (candidate_number <= 8);
}
}  // namespace

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
            const int empty_squares = ConvertToInteger(symbol);
            if (IsNumberOfEmptySquares(empty_squares))
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
                        side = kBlackBoard;
                        piece_kind = kPawn;
                        break;
                    case 'r':
                        side = kBlackBoard;
                        piece_kind = kRook;
                        break;
                    case 'n':
                        side = kBlackBoard;
                        piece_kind = kKnight;
                        break;
                    case 'b':
                        side = kBlackBoard;
                        piece_kind = kBishop;
                        break;
                    case 'q':
                        side = kBlackBoard;
                        piece_kind = kQueen;
                        break;
                    case 'k':
                        side = kBlackBoard;
                        piece_kind = kKing;
                        break;
                    case 'P':
                        side = kWhiteBoard;
                        piece_kind = kPawn;
                        break;
                    case 'R':
                        side = kWhiteBoard;
                        piece_kind = kRook;
                        break;
                    case 'N':
                        side = kWhiteBoard;
                        piece_kind = kKnight;
                        break;
                    case 'B':
                        side = kWhiteBoard;
                        piece_kind = kBishop;
                        break;
                    case 'Q':
                        side = kWhiteBoard;
                        piece_kind = kQueen;
                        break;
                    case 'K':
                        side = kWhiteBoard;
                        piece_kind = kKing;
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
            position.attacking_side_ = kWhiteBoard;
            position.defending_side_ = kBlackBoard;
            break;
        case 'b':
            position.white_to_move_ = false;
            position.attacking_side_ = kBlackBoard;
            position.defending_side_ = kWhiteBoard;
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
                    position[kExtrasBoard] |= kCastlingBlackKingside;
                    break;
                case 'q':
                    position[kExtrasBoard] |= kCastlingBlackQueenside;
                    break;
                case 'K':
                    position[kExtrasBoard] |= kCastlingWhiteKingside;
                    break;
                case 'Q':
                    position[kExtrasBoard] |= kCastlingWhiteQueenside;
                    break;
                default:
                    throw std::runtime_error{"FEN contains invalid castling token."};
            }
        }
    }

    const std::string en_passant = tokens.at(kFenTokenEnPassant);
    if (en_passant != "-")
    {
        const auto location_it = std::find(kSquareLabels.begin(), kSquareLabels.end(), en_passant);
        if (location_it == kSquareLabels.end())
        {
            throw std::runtime_error{"FEN contains invalid en passant token."};
        }

        const Bitboard en_passant_bits = std::distance(kSquareLabels.begin(), location_it);
        position[kExtrasBoard] |= en_passant_bits << kBoardShiftEnPassant;
    }

    const Bitboard static_plies = std::stoi(tokens.at(kFenTokenStaticPlies));
    position[kExtrasBoard] |= static_plies << kBoardShiftStaticPlies;

    const Bitboard full_moves_count = std::stoi(tokens.at(kFenTokenMoves));
    const Bitboard additional_ply_if_black_has_not_played_yet = tokens.at(kFenTokenSide) == "b";
    const Bitboard total_plies = (full_moves_count - 1) * 2 + additional_ply_if_black_has_not_played_yet;
    position[kExtrasBoard] |= total_plies << kBoardShiftTotalPlies;

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
        const Bitmove white_piece = position.GetPieceKind(kWhiteBoard, square);
        const Bitmove black_piece = position.GetPieceKind(kBlackBoard, square);
        if (white_piece)
        {
            if (empty_squares)
            {
                pieces += std::to_string(empty_squares);
                empty_squares = 0;
            }
            pieces += kPieceLabelsFenWhite.at(white_piece);
        }
        else if (black_piece)
        {
            if (empty_squares)
            {
                pieces += std::to_string(empty_squares);
                empty_squares = 0;
            }
            pieces += kPieceLabels.at(black_piece);
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
    if (position[kExtrasBoard] & kCastlingWhiteKingside)
    {
        castling += "K";
    }
    if (position[kExtrasBoard] & kCastlingWhiteQueenside)
    {
        castling += "Q";
    }
    if (position[kExtrasBoard] & kCastlingBlackKingside)
    {
        castling += "k";
    }
    if (position[kExtrasBoard] & kCastlingBlackQueenside)
    {
        castling += "q";
    }
    const bool at_least_one_side_can_castle = position[kExtrasBoard] & kBoardMaskCastling;
    if (!at_least_one_side_can_castle)
    {
        castling += "-";
    }

    const auto en_passant_square_bits = (position[kExtrasBoard] & kBoardMaskEnPassant) >> kBoardShiftEnPassant;
    const auto en_passant = en_passant_square_bits ? kSquareLabels.at(en_passant_square_bits) : "-";

    const auto static_plies = std::to_string(position.GetStaticPlies());

    const auto full_moves = std::to_string(1 + (position.GetTotalPlies() / 2));

    return pieces + " " + side + " " + castling + " " + en_passant + " " + static_plies + " " + full_moves;
}

std::vector<std::string> TokenizeFen(const std::string& fen)
{
    std::istringstream iss{fen};
    return {std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};
}

void PrettyPrintFen(const std::string& fen)
{
    std::cout << fen << '\n';

    const auto tokens = TokenizeFen(fen);
    for (const char symbol : tokens.at(kFenTokenPieces))
    {
        const int empty_squares = ConvertToInteger(symbol);
        if (IsNumberOfEmptySquares(empty_squares))
        {
            for (int count = 0; count < empty_squares; count++)
            {
                std::cout << "- ";
            }
        }
        else if (symbol == '/')
        {
            std::cout << '\n';
        }
        else  // is piece symbol
        {
            std::cout << symbol << ' ';
        }
    }
    std::cout << '\n' << std::endl;
}

}  // namespace Chess