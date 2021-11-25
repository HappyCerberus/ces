load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "synchronous",
    hdrs = ["synchronous.h"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "chainable",
    hdrs = ["chainable.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//ces/coroutines/awaitables:final-continuation",
        "//ces/utils:awaitable-data",
        "//ces/utils:concepts",
    ],
)

cc_library(
    name = "scheduler",
    srcs = ["scheduler.cc"],
    hdrs = ["scheduler.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//ces:event-emitter",
        "//ces/coroutines/awaitables:handoff",
        "//ces/coroutines/awaitables:universal",
        "//ces/utils:concepts",
    ],
)

cc_library(
    name = "terminating",
    hdrs = ["terminating.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//ces/coroutines/awaitables:final-continuation",
    ],
)
