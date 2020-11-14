load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")

cc_library(
    name = "search",
    srcs = [
        "node.cpp",
        "populate.cpp",
    ],
    hdrs = [
        "minimax.h",
        "node.h",
        "populate.h",
    ],
    visibility = [
        "//:__pkg__",
        "//play:__pkg__",
    ],
    deps = [
        "//bitboard",
        "//board",
        "//evaluate",
    ],
)

cc_test(
    name = "unit_tests",
    srcs = [
        "test/evaluate_mock.h",
        "test/minimax_mock.h",
        "test/minimax_unit_tests.cpp",
        "test/move_generation_mock.h",
        "test/node_unit_tests.cpp",
        "test/populate_unit_tests.cpp",
        "test/puzzle_tests.cpp",
        "test/smoke_tests.cpp",
    ],
    visibility = ["//:__pkg__"],
    deps = [
        ":search",
        "@googletest//:gtest_main",
    ],
)

cc_binary(
    name = "perft",
    srcs = [
        "test/evaluate_mock.h",
        "test/minimax_mock.h",
        "test/perft.cpp",
    ],
    deps = [":search"],
)
