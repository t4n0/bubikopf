load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_test(
    name = "test",
    srcs = ["evaluate_unit_tests.cpp"],
    deps = [
        "//evaluate",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "mock",
    hdrs = ["evaluate_mock.h"],
    visibility = ["//visibility:public"],
)
