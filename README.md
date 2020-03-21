# bubikopf
Chess engine. Implements alpha/beta search.

## Todo

- Test for checkmate
- Is king in check?
- Is king still in check after moves?
- Set rook and king castling to false after King/Rook move
- Castling
- Decrement evaluation upwards when propagating a checkmate backwards
- Use ToCoor / ToIdx Conversion functions only in constexpr
- Draw due to insufficient material
- Draw due to 50 move rule
- Draw due to threefold repitition
