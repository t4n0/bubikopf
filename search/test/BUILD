load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
    name = "mock",
    hdrs = ["move_generation_mock.h"],
    visibility = ["//search/benchmark:__pkg__"],
)

cc_test(
    name = "smoke",
    srcs = ["smoke_tests.cpp"],
    deps = [
        "//bitboard",
        "//evaluate/test:mock",
        "//search",
        "//search/test:mock",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "test",
    srcs = ["find_best_move_test.cpp"],
    deps = [
        "//hardware",
        "//search",
        "@googletest//:gtest_main",
    ],
)
