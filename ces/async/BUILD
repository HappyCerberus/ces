load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "async-connect",
    srcs = ["async-connect.cc"],
    hdrs = ["async-connect.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//ces/coroutines:chainable",
        "//ces/coroutines:scheduler",
        "//ces/epoll:socket",
    ],
)

cc_library(
    name = "async-write",
    srcs = ["async-write.cc"],
    hdrs = ["async-write.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//ces/coroutines:chainable",
        "//ces/coroutines:scheduler",
        "//ces/epoll:socket",
    ],
)

cc_library(
    name = "async-read",
    srcs = ["async-read.cc"],
    hdrs = ["async-read.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//ces/coroutines:chainable",
        "//ces/coroutines:scheduler",
        "//ces/epoll:socket",
    ],
)

cc_library(
    name = "async-accept",
    srcs = ["async-accept.cc"],
    hdrs = ["async-accept.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//ces/coroutines:chainable",
        "//ces/coroutines:scheduler",
        "//ces/coroutines:terminating",
        "//ces/epoll:socket",
    ],
)

cc_library(
    name = "async-server",
    srcs = ["async-server.cc"],
    hdrs = ["async-server.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//ces/async:async-accept",
        "//ces/coroutines:chainable",
        "//ces/coroutines:terminating",
        "//ces/epoll:socket",
    ],
)
