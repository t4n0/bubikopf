cc_library(
    name = "search",
    srcs = ["populate.cpp"],
    hdrs = [
        "evaluate.h",
        "evaluation.h",
        "minimax.h",
        "populate.h",
    ],
    visibility = ["//play:__pkg__"],
    deps = ["//board"],
)

cc_test(
    name = "unit_tests",
    srcs = [
        "test/evaluate_unit_tests.cpp",
        "test/evaluation_unit_tests.cpp",
        "test/minimax_abstract_examples.h",
        "test/minimax_unit_tests.cpp",
        "test/populate_unit_tests.cpp",
    ],
    deps = [
        ":search",
        "@googletest//:gtest_main",
    ],
)
