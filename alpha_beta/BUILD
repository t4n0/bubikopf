cc_library(
    name = "alpha_beta",
    hdrs = [
        "evaluation.h",
        "minimax.h",
    ],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "unit_tests",
    srcs = [
        "test/evaluation_unit_tests.cpp",
        "test/minimax_abstract_examples.h",
        "test/minimax_unit_tests.cpp",
    ],
    deps = [
        ":alpha_beta",
        "@googletest//:gtest_main",
    ],
)
