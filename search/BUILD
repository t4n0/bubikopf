cc_library(
    name = "search",
    srcs = [
        "node.cpp",
        "populate.cpp",
    ],
    hdrs = [
        "evaluate.h",
        "evaluation.h",
        "minimax.h",
        "node.h",
        "populate.h",
    ],
    visibility = [
        "//:__pkg__",
        "//play:__pkg__",
    ],
    deps = ["//board"],
)

cc_test(
    name = "unit_tests",
    srcs = [
        "test/evaluate_mock.h",
        "test/evaluate_unit_tests.cpp",
        "test/evaluation_unit_tests.cpp",
        "test/minimax_unit_tests.cpp",
        "test/node_unit_tests.cpp",
        "test/populate_unit_tests.cpp",
        "test/puzzle_tests.cpp",
    ],
    visibility = ["//:__pkg__"],
    deps = [
        ":search",
        "@googletest//:gtest_main",
    ],
)
