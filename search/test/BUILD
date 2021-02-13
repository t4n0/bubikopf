load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "smoke",
    srcs = ["smoke_test.cpp"],
    deps = [
        "//bitboard",
        "//search:traverse_all_leaves",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "test",
    srcs = [
        "find_best_move_test.cpp",
        "material_difference_comparison_unit_test.cpp",
        "principal_variation_test.cpp",
        "traverse_all_leaves_unit_test.cpp",
    ],
    deps = [
        "//evaluate",
        "//hardware",
        "//search:find_best_move",
        "//search:traverse_all_leaves",
        "@googletest//:gtest_main",
    ],
)
