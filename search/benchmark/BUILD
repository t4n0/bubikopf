load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "perft",
    srcs = ["perft.cpp"],
    deps = [
        "//bitboard",
        "//search:traverse_all_leaves",
    ],
)

cc_binary(
    name = "benchmark",
    srcs = ["benchmark.cpp"],
    deps = [
        "//bitboard",
        "//evaluate",
        "//search:find_best_move",
        "//search:traverse_all_leaves",
        "@googlebenchmark//:benchmark",
    ],
)
