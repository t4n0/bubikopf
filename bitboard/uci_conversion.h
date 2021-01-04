#ifndef BITBOARD_UCI_CONVERSION_H
#define BITBOARD_UCI_CONVERSION_H

#include "bitboard/basic_type_declarations.h"

#include <string>

namespace Chess
{

constexpr const char* const kUciNullMove = "0000";

std::string ToUciString(const Bitmove move);

}  // namespace Chess

#endif