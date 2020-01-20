cc_library(
    name = "chess",
    srcs = [
        "pieces.cpp",
        "populate.cpp",
        "state.cpp",
    ],
    hdrs = [
        "evaluate.h",
        "board_generator.h",
        "i_square.h",
        "pieces.h",
        "populate.h",
        "state.h",
    ],
    visibility = ["//visibility:public"],
    deps = ["//alpha_beta"],
)

cc_test(
    name = "unit_tests",
    srcs = [
        "test/evaluate_unit_tests.cpp",
        "test/board_generator_unit_tests.cpp",
        "test/pieces_unit_tests.cpp",
        "test/populate_unit_tests.cpp",
        "test/state_unit_tests.cpp",
    ],
    deps = [
        ":chess",
        "@googletest//:gtest_main",
    ],
)

cc_binary(
    name = "play",
    srcs = ["main.cpp"],
    deps = [":chess"],
)
