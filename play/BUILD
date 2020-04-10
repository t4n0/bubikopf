load("@bubikopf_deps//:requirements.bzl", "requirement")

cc_binary(
    name = "command_line",
    srcs = ["main.cpp"],
    visibility = ["//:__pkg__"],
    deps = [
        "//board",
        "//search",
    ],
)

cc_library(
    name = "lichess_interactor",
    srcs = ["lichess_interactor.cpp"],
    hdrs = ["lichess_interactor.h"],
    deps = [":bubikopf_facade"],
)

cc_binary(
    name = "bubikopf.so",
    srcs = ["lichess_interactor_py_binding.cpp"],
    linkshared = True,
    linkstatic = False,
    deps = [
        ":lichess_interactor",
        "@boost//:python",
    ],
)

py_library(
    name = "bubikopf",
    data = [":bubikopf.so"],
    imports = ["."],
)

py_binary(
    name = "lichess_connector",
    srcs = ["lichess_connector.py"],
    deps = [
        ":bubikopf",
        requirement("berserk"),
        requirement("requests"),
        requirement("ndjson"),
        requirement("deprecated"),
        requirement("wrapt"),
    ],
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
    visibility = ["//:__pkg__"],
    deps = [
        ":uci_utilities",
        "//board",
        "//search",
    ],
)
