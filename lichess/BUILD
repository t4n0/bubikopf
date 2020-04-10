load("@bubikopf_deps//:requirements.bzl", "requirement")

cc_library(
    name = "lichess_interactor",
    srcs = ["lichess_interactor.cpp"],
    hdrs = ["lichess_interactor.h"],
    deps = ["//play:bubikopf_facade"],
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
    name = "daemon",
    srcs = ["daemon.py"],
    deps = [
        ":bubikopf",
        requirement("berserk"),
        requirement("requests"),
        requirement("ndjson"),
        requirement("deprecated"),
        requirement("wrapt"),
    ],
)
