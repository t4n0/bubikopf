load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "test",
    srcs = [
        "evaluate_unit_tests.cpp",
        "evaluation_unit_tests.cpp",
        "player_unit_tests.cpp",
    ],
    visibility = ["//:__pkg__"],
    deps = [
        "//evaluate",
        "@googletest//:gtest_main",
    ],
)
