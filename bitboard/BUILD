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
        "shift.cpp",
        "shift.h",
        "squares.h",
    ],
    hdrs = [
        "move_generation.h",
    ],
    visibility = [
        "//:__pkg__",
        "//evaluate:__pkg__",
        "//search:__pkg__",
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
