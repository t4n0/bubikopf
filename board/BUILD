cc_library(
    name = "board",
    srcs = [
        "board.cpp",
        "pieces.cpp",
        "state.cpp",
    ],
    hdrs = [
        "board.h",
        "i_square.h",
        "pieces.h",
        "player.h",
        "state.h",
    ],
    visibility = [
        "//:__pkg__",
        "//evaluate:__pkg__",
        "//play:__pkg__",
        "//search:__pkg__",
    ],
)

cc_test(
    name = "unit_tests",
    srcs = [
        "test/board_unit_tests.cpp",
        "test/pieces_unit_tests.cpp",
        "test/player_unit_tests.cpp",
        "test/state_unit_tests.cpp",
    ],
    visibility = [
        "//:__pkg__",
    ],
    deps = [
        ":board",
        "@googletest//:gtest_main",
    ],
)
