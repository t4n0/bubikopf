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
        "//board",
        "//evaluate",
    ],
)

cc_test(
    name = "unit_tests",
    srcs = [
        "minimax_unit_tests.cpp",
        "node_unit_tests.cpp",
        "populate_unit_tests.cpp",
        "puzzle_tests.cpp",
        "smoke_tests.cpp",
    ],
    visibility = ["//:__pkg__"],
    deps = [
        ":mock",
        "//search",
        "@googletest//:gtest_main",
    ],
)
