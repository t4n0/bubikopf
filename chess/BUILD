cc_library(
    name = "chess",
    srcs = ["source/state.cpp"],
    hdrs = [
        "include/basic_types.h",
        "include/state.h",
    ],
    copts = ["-std=c++17"],
    include_prefix = "chess",
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = [
        "//alpha_beta",
    ],
)

cc_library(
    name = "board_test_helpers",
    hdrs = ["test/board_test_helpers.h"],
    copts = ["-std=c++17"],
    include_prefix = "chess",
    strip_include_prefix = "test",
    visibility = ["//visibility:public"],
    deps = [
        ":chess",
        "@googletest//:gtest",
    ],
)

cc_test(
    name = "chess_unit_tests",
    srcs = [
        "test/basic_types_unit_tests.cpp",
        "test/state_unit_tests.cpp",
    ],
    copts = ["-std=c++17"],
    visibility = ["//visibility:public"],
    deps = [
        ":board_test_helpers",
        ":chess",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "minimax",
    srcs = ["source/populate.cpp"],
    hdrs = [
        "include/evaluate.h",
        "include/populate.h",
    ],
    copts = ["-std=c++17"],
    include_prefix = "chess",
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = [
        ":chess",
        "//alpha_beta",
    ],
)

cc_test(
    name = "minimax_tests",
    srcs = [
        "test/evaluate_tests.cpp",
        "test/populate_unit_tests.cpp",
    ],
    copts = ["-std=c++17"],
    visibility = ["//visibility:public"],
    deps = [
        ":board_test_helpers",
        ":minimax",
        "@googletest//:gtest_main",
    ],
)
