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
    name = "find_best_move",
    srcs = ["find_best_move_benchmark.cpp"],
    deps = [
        "//bitboard",
        "//evaluate",
        "//search:find_best_move",
        "@googlebenchmark//:benchmark",
    ],
)

cc_binary(
    name = "traverse_all_leaves",
    srcs = ["traverse_all_leaves_benchmark.cpp"],
    deps = [
        "//bitboard",
        "//search:traverse_all_leaves",
        "@googlebenchmark//:benchmark",
    ],
)
