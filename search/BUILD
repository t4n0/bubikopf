load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "search",
    srcs = [
        "node.cpp",
        "populate.cpp",
    ],
    hdrs = [
        "minimax.h",
        "node.h",
        "populate.h",
    ],
    visibility = [
        "//play:__pkg__",
        "//search:__subpackages__",
    ],
    deps = [
        "//bitboard",
        "//board",
        "//evaluate",
    ],
)
