# bubikopf
Chess engine. Implements alpha/beta search.

## Todo

- Test for checkmate
- Is king in check?
- Is king still in check after moves?
- Set all pawns to not capturable via en_passant
- Set rook and king castling to false after move
- Decrement evaluation upwards when propagating a checkmate backwards
- Move function to choose a branch (rest of tree shall be discarded)
- Use ToCoor / ToIdx Conversion functions only in constexpr
- Draw due to insufficient material
- Draw due to 50 move rule
- Pawn promotion
- Point to the same via simple ref. Avoid mallocs.
