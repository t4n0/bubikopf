cc_library(
    name = "bitboard",
    srcs = [
        "basic_type_declarations.h",
        "move.h",
        "pieces.h",
        "position.cpp",
        "position.h",
    ],
    hdrs = [
        "move_generation.h",
    ],
    visibility = [
        "//:__pkg__",
        "//evaluate:__pkg__",
        "//search:__pkg__",
    ],
)

cc_test(
    name = "unit_tests",
    srcs = [
        "test/move_unit_tests.cpp",
        "test/position_unit_tests.cpp",
    ],
    visibility = ["//:__pkg__"],
    deps = [
        ":bitboard",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
