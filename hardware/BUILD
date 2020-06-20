cc_library(
    name = "hardware",
    hdrs = [
        "population_count.h",
        "trailing_zeros_count.h",
    ],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "unit_tests",
    srcs = ["test/trailing_zeros_count_unit_tests.cpp"],
    deps = [
        ":hardware",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
