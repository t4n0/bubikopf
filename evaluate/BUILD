cc_library(
    name = "evaluate",
    hdrs = ["evaluate.h"],
    visibility = [
        "//:__pkg__",
        "//play:__pkg__",
        "//search:__pkg__",
    ],
    deps = [
        "//bitboard",
        "//board",
    ],
)

cc_test(
    name = "unit_tests",
    srcs = ["test/evaluate_unit_tests.cpp"],
    visibility = ["//:__pkg__"],
    deps = [
        ":evaluate",
        "@googletest//:gtest_main",
    ],
)
