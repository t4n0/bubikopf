cc_library(
    name = "chess",
    srcs = [
        #"populate.cpp",
        "state.cpp",
    ],
    hdrs = [
        "evaluate.h",
        #"populate.h",
        "state.h",
    ],
    visibility = ["//visibility:public"],
    deps = ["//alpha_beta"],
)

cc_test(
    name = "unit_tests",
    srcs = [
        "test/evaluate_tests.cpp",
        #"test/populate_unit_tests.cpp",
        "test/state_unit_tests.cpp",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":chess",
        "@googletest//:gtest_main",
    ],
)
