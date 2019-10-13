cc_library(
    name = "game_tree",
    hdrs = [
        "include/evaluation.h",
        "include/minimax.h",
    ],
    copts = ["-std=c++17"],
    include_prefix = "game_tree",
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
)

cc_test(
    name = "game_tree_unit_tests",
    srcs = [
        "test/evaluation_unit_tests.cpp",
        "test/minimax_abstract_examples.h",
        "test/minimax_unit_tests.cpp",
    ],
    copts = ["-std=c++17"],
    visibility = ["//visibility:public"],
    deps = [
        ":game_tree",
        "@googletest//:gtest_main",
    ],
)
