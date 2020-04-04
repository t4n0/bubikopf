load("@compilecommands//:aspects.bzl", "compilation_database")

compilation_database(
    name = "create_compilation_database",
    # ideally should be the same as `bazel info execution_root`.
    exec_root = "/home/tano/.cache/bazel/_bazel_tano/45ff21c37ebc3753a4144a1f7175237c/execroot/__main__",
    targets = [
        "//board",
        "//board:unit_tests",
        "//play:bubikopf.so",
        "//play:command_line",
        "//play:lichess_interactor",
        "//play:uci_utilities",
        "//play/test",
        "//search",
        "//search:unit_tests",
    ],
)
