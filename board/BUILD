cc_library(
    name = "board",
    srcs = [
        "board.cpp",
        "pieces.cpp",
        "position.cpp",
    ],
    hdrs = [
        "board.h",
        "i_square.h",
        "pieces.h",
        "player.h",
        "position.h",
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
        "test/position_unit_tests.cpp",
    ],
    visibility = [
        "//:__pkg__",
    ],
    deps = [
        ":board",
        "@googletest//:gtest_main",
    ],
)
