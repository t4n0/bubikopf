load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "uci_interactor",
    srcs = ["uci_interactor.cpp"],
    hdrs = ["uci_interactor.h"],
)

cc_library(
    name = "bubikopf",
    srcs = ["bubikopf.cpp"],
    hdrs = ["bubikopf.h"],
    visibility = ["//play:__subpackages__"],
    deps = [
        "//bitboard",
        "//evaluate",
        "//search",
    ],
)

