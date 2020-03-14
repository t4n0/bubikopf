cc_library(
    name = "board",
    srcs = [
        "piece.cpp",
        "state.cpp",
    ],
    hdrs = [
        "i_square.h",
        "piece.h",
        "player.h",
        "state.h",
        "utilities.h",
    ],
    visibility = ["//search:__pkg__"],
)

cc_test(
    name = "unit_tests",
    srcs = [
        "test/board_generator_unit_tests.cpp",
        "test/pieces_unit_tests.cpp",
        "test/player_unit_tests.cpp",
        "test/state_unit_tests.cpp",
    ],
    deps = [
        ":board",
        "@googletest//:gtest_main",
    ],
)
