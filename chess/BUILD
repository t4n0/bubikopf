cc_library(
    name = "chess",
    srcs = [
        "source/state.cpp",
    ],
    hdrs = [
        "include/evaluate.h",
        "include/state.h",
    ],
    copts = ["-std=c++17"],
    include_prefix = "chess",
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = ["//alpha_beta"],
)

cc_test(
    name = "chess_unit_tests",
    srcs = [
        "test/evaluate_tests.cpp",
        "test/state_unit_tests.cpp",
    ],
    copts = ["-std=c++17"],
    visibility = ["//visibility:public"],
    deps = [
        ":chess",
        "@googletest//:gtest_main",
    ],
)
