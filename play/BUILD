load("@bubikopf_deps//:requirements.bzl", "requirement")

cc_binary(
    name = "command_line",
    srcs = ["main.cpp"],
    deps = [
        "//board",
        "//search",
    ],
)

py_binary(
    name = "lichess_connector",
    srcs = ["lichess_connector.py"],
    deps = [
        requirement("berserk"),
        requirement("requests"),
        requirement("ndjson"),
        requirement("deprecated"),
        requirement("wrapt"),
    ],
)
