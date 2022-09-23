# bubikopf
UCI-compliant chess engine.

<img src="doc/bubikopf.jpg" width="300">

(The yellow sticker is for [catching bugs](https://www.amazon.com/s?k=fungus+gnat+sticky+trap&crid=1HBPT4CPWLJLS&sprefix=fungus+gnat+%2Caps%2C270&ref=nb_sb_ss_ts-doa-p_8_12).)

## specs
- written in C++20
- no dependencies to external libraries
- move generation visits about `25k nodes/ms` on an `Intel i7-6700HQ`
- iteratively deepening negamax search with alpha / beta pruning
- principal variation is tracked via triangular table
- rating not determined yet 


## missing features
- NNUE based static evaluation
- draw due to threefold repitition (via hash table)
- draw due to 50 move rule
- draw due to insufficient material

## linux build instructions
- install the [bazel](https://docs.bazel.build/versions/4.0.0/install.html) build system
- clone this repository to your machine
- build the engine binary:
```shell
cd <clone_dir>
bazel build //play:bubikopf
cp <clone_dir>/bazel-bin/play/bubikopf <path_where_you_want_the_binary>
```
(Tested on Ubuntu 20.04 with x86_64 CPU.)

## acknowledgements
For the development of bubikopf the following open source projects were used. A big thanks to the authors and contributors:
- [Mk-Chan/BBPerft](https://github.com/Mk-Chan/BBPerft) (reference implementation for debugging and benchmarking)
- [ShailChoksi/lichess-bot](https://github.com/ShailChoksi/lichess-bot) (bridge between engine binary and lichess server)

And _of course_ [lichess](https://github.com/lichess-org). Hands down the best chess plattform out there. [Go check it out](https://lichess.org/).
