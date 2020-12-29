load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "bitboard",
    srcs = [
        "basic_type_declarations.h",
        "board.h",
        "fen_conversion.cpp",
        "move.h",
        "move_list.h",
        "pieces.h",
        "position.cpp",
        "position.h",
        "shift.cpp",
        "shift.h",
        "squares.h",
    ],
    hdrs = [
        "fen_conversion.h",
        "generate_moves.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//bitboard/lookup_table",
        "//hardware",
    ],
)
