# bubikopf
A chess engine. Implements alpha/beta search.

## Todos

### Missing Features
- Test for checkmate
- Is king in check?
- Is king still in check after move?
- Set rook and king castling to false after King/Rook move
- Castling
- Decrement evaluation upwards when propagating a checkmate backwards
- Use ToCoor / ToIdx Conversion functions only in constexpr
- Draw due to insufficient material
- Draw due to 50 move rule
- Draw due to threefold repitition


### Optimizations
- remove "turn" member from State an substitute with plies
- remove "Player" variant from Evaluation and substitute with int max value

### Bugs
- local repositories
- bazel test //... not working because of compile commands
- local change to simplejson