package(default_visibility = ["//visibility:public"])

load("@bazel_compilation_db//:aspects.bzl", "compilation_database")
compilation_database(
    name = "cdb",
    exec_root = "/home/p/.cache/bazel/_bazel_p/405e30466fe35c921f24598dc330b9ed/execroot/mpmc",
    targets = ["@mtfe//mutex:mpmc", "@mtfe//dispatch:dispatch", "//:mtfe"],
)

cc_binary(
    name = "mtfe",
    srcs = ["main.cc"],
    deps = ["//mutex:mpmc", "//dispatch:dispatch", "@com_github_gabime_spdlog//:spdlog"],
    copts = ["-std=c++17"],
)

