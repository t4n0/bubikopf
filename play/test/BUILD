load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "test",
    srcs = ["uci_utilities_unit_tests.cpp"],
    visibility = ["//:__pkg__"],
    deps = [
        "//play:uci_utilities",
        "@googletest//:gtest_main",
    ],
)
