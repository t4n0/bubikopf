cc_library(
    name = "alpha_beta",
    hdrs = [
        "include/evaluation.h",
        "include/minimax.h",
    ],
    copts = ["-std=c++17"],
    include_prefix = "alpha_beta",
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
)

cc_test(
    name = "alpha_beta_unit_tests",
    srcs = [
        "test/evaluation_unit_tests.cpp",
        "test/minimax_abstract_examples.h",
        "test/minimax_unit_tests.cpp",
    ],
    copts = ["-std=c++17"],
    visibility = ["//visibility:public"],
    deps = [
        ":alpha_beta",
        "@googletest//:gtest_main",
    ],
)
