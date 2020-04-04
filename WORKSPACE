load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "googletest",
    remote = "https://github.com/google/googletest",
    tag = "release-1.10.0",
)

git_repository(
    name = "compilecommands",
    remote = "https://github.com/grailbio/bazel-compilation-database",
    tag = "0.4.2",
)

git_repository(
    name = "rules_python",
    remote = "https://github.com/bazelbuild/rules_python",
    commit = "748aa53d7701e71101dfd15d800e100f6ff8e5d1",
)

load("@rules_python//python:pip.bzl", "pip3_import")

pip3_import(
   name = "bubikopf_deps",
   requirements = "//:requirements.txt",
)
load("@bubikopf_deps//:requirements.bzl", "pip_install")

pip_install()