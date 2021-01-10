load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "bitboard",
    srcs = [
        "basic_type_declarations.h",
        "board.h",
        "fen_conversion.cpp",
        "move.cpp",
        "move.h",
        "move_stack.h",
        "pieces.h",
        "position.cpp",
        "position.h",
        "shift.h",
        "squares.h",
        "uci_conversion.cpp",
    ],
    hdrs = [
        "fen_conversion.h",
        "generate_moves.h",
        "uci_conversion.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//bitboard/lookup_table",
        "//hardware",
    ],
)
