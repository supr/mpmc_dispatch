package(default_visibility = ["//visibility:public"])

cc_library(
    name = "dispatch",
    srcs = ["dispatch.cc", "dispatch.h"],
    hdrs = ["dispatch.h"],
    copts = ["-std=c++17"],
    deps = ["@com_github_gabime_spdlog//:spdlog", "@mtfe//mutex:mpmc"],
    linkopts = ["-pthread"],
)
