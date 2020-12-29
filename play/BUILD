load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "uci_interactor",
    srcs = ["uci_interactor.cpp"],
    hdrs = ["uci_interactor.h"],
)

cc_library(
    name = "engine_api",
    srcs = ["bubikopf.cpp"],
    hdrs = ["bubikopf.h"],
    visibility = ["//play:__subpackages__"],
    deps = [
        "//bitboard",
        "//evaluate",
        "//search",
    ],
)

cc_binary(
    name = "bubikopf",
    srcs = ["main.cpp"],
    linkopts = ["-pthread"],
    deps = [
        ":engine_api",
        ":uci_interactor",
    ],
)
