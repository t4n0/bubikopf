load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
    name = "bitboard",
    srcs = [
        "basic_type_declarations.h",
        "board.h",
        "move.h",
        "move_generation.h",
        "move_list.h",
        "move_lookup_table.h",
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
    visibility = [
        "//evaluate:__pkg__",
        "//search:__subpackages__",
    ],
    deps = ["//hardware"],
)

cc_test(
    name = "unit_tests",
    srcs = [
        "test/board_unit_tests.cpp",
        "test/move_generation_unit_tests.cpp",
        "test/move_unit_tests.cpp",
        "test/position_unit_tests.cpp",
        "test/set_up_from_fen_unit_tests.cpp",
        "test/shift_unit_tests.cpp",
        "test/squares_unit_tests.cpp",
    ],
    visibility = ["//:__pkg__"],
    deps = [
        ":bitboard",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
