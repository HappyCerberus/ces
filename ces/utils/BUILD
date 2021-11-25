load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "event-info",
    hdrs = ["event-info.h"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "awaitable-data",
    srcs = ["awaitable-data.cc"],
    hdrs = ["awaitable-data.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":event-info",
        ":time-fwd",
    ],
)

cc_library(
    name = "concepts",
    hdrs = ["concepts.h"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "observer",
    hdrs = ["observer.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":time-fwd",
    ],
)

cc_library(
    name = "time-fwd",
    hdrs = ["time-fwd.h"],
    visibility = ["//visibility:public"],
)
