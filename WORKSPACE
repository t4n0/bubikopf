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

git_repository(
    name = "com_github_nelhage_rules_boost",
    commit = "0f64534c46155ac3760882727bed5be39c39a309",
    remote = "https://github.com/t4n0/rules_boost",
    shallow_since = "1586009442 +0200"
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")

boost_deps()
