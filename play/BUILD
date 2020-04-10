cc_binary(
    name = "command_line",
    srcs = ["main.cpp"],
    visibility = ["//:__pkg__"],
    deps = ["//search"],
)

cc_library(
    name = "uci_utilities",
    srcs = ["uci_utilities.cpp"],
    hdrs = ["uci_utilities.h"],
    visibility = [
        ":__subpackages__",
        "//:__pkg__",
    ],
    deps = ["@boost//:algorithm"],
)

cc_library(
    name = "bubikopf_facade",
    srcs = ["bubikopf_facade.cpp"],
    hdrs = ["bubikopf_facade.h"],
    visibility = [
        "//:__pkg__",
        "//lichess:__pkg__",
    ],
    deps = [
        ":uci_utilities",
        "//search",
    ],
)
