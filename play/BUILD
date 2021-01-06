load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "logging",
    srcs = ["logging.cpp"],
    hdrs = ["logging.h"],
)

cc_library(
    name = "uci_interactor",
    srcs = ["uci_interactor.cpp"],
    hdrs = ["uci_interactor.h"],
    deps = [":logging"],
)

cc_library(
    name = "engine_api",
    srcs = ["bubikopf.cpp"],
    hdrs = ["bubikopf.h"],
    visibility = ["//play:__subpackages__"],
    deps = [
        ":logging",
        "//bitboard",
        "//evaluate",
        "//search:find_best_move",
    ],
)

cc_binary(
    name = "bubikopf",
    srcs = ["main.cpp"],
    linkopts = ["-pthread"],
    deps = [
        ":engine_api",
        ":logging",
        ":uci_interactor",
    ],
)
