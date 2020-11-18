load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
    name = "mock",
    hdrs = [
        "minimax_mock.h",
        "move_generation_mock.h",
    ],
    visibility = ["//search/benchmark:__pkg__"],
    deps = ["//evaluate/test:mock"],
)

cc_test(
    name = "test",
    srcs = ["smoke_tests.cpp"],
    deps = [
        "//bitboard",
        "//search/test:mock",
        "@googletest//:gtest_main",
    ],
)
