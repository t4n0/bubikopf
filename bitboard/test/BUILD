load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "test",
    srcs = [
        "board_unit_tests.cpp",
        "move_unit_tests.cpp",
        "position_unit_tests.cpp",
        "set_up_from_fen_unit_tests.cpp",
        "shift_unit_tests.cpp",
        "squares_unit_tests.cpp",
    ],
    deps = [
        "//bitboard",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
