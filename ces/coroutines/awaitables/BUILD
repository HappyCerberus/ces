load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "handoff",
    hdrs = ["handoff.h"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "universal",
    hdrs = ["universal.h"],
    visibility = ["//visibility:public"],
    deps = ["//ces/utils:awaitable-data"],
)

cc_library(
    name = "final-continuation",
    hdrs = ["final-continuation.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//ces/utils:concepts",
    ],
)
