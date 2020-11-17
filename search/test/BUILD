load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
    name = "mock",
    hdrs = [
        "evaluate_mock.h",
        "minimax_mock.h",
        "move_generation_mock.h",
    ],
    visibility = ["//search/benchmark:__pkg__"],
    deps = [
        "//evaluate",
    ],
)

cc_test(
    name = "unit_tests",
    srcs = [
        "smoke_tests.cpp",
    ],
    deps = [
        ":mock",
        "@googletest//:gtest_main",
    ],
)
