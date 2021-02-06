load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "abort_condition",
    hdrs = ["abort_condition.h"],
)

cc_library(
    name = "find_best_move",
    hdrs = [
        "find_best_move.h",
        "material_difference_comparison.h",
        "principal_variation.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":abort_condition",
        "//bitboard",
        "//evaluate",
    ],
)

cc_library(
    name = "traverse_all_leaves",
    hdrs = ["traverse_all_leaves.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":abort_condition",
        "//bitboard",
    ],
)
