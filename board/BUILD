load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
    name = "board",
    hdrs = [
        "evaluation.h",
        "player.h",
    ],
    visibility = [
        "//evaluate:__pkg__",
        "//play:__pkg__",
        "//search:__subpackages__",
    ],
)

cc_test(
    name = "unit_tests",
    srcs = [
        "test/evaluation_unit_tests.cpp",
        "test/player_unit_tests.cpp",
    ],
    visibility = [
    ],
    deps = [
        ":board",
        "@googletest//:gtest_main",
    ],
)
