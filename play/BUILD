load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "command_line",
    srcs = ["main.cpp"],
    visibility = ["//:__pkg__"],
    deps = ["//search"],
)
