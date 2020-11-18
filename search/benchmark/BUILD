load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "perft",
    srcs = ["perft.cpp"],
    deps = [
        "//bitboard",
        "//evaluate/test:mock",
        "//search",
        "//search/test:mock",
    ],
)

cc_binary(
    name = "benchmark",
    srcs = ["benchmark.cpp"],
    deps = [
        "//bitboard",
        "//evaluate/test:mock",
        "//search",
        "//search/test:mock",
        "@googlebenchmark//:benchmark",
    ],
)
