load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "bitboard",
    srcs = [
        "basic_type_declarations.h",
        "board.h",
        "knight_lookup_table.h",
        "move.h",
        "move_generation.h",
        "move_list.h",
        "pawn_lookup_table.h",
        "pieces.h",
        "position.cpp",
        "position.h",
        "position_from_fen.cpp",
        "shift.cpp",
        "shift.h",
        "squares.h",
    ],
    hdrs = [
        "move_generation.h",
        "position_from_fen.h",
    ],
    visibility = ["//visibility:public"],
    deps = ["//hardware"],
)
