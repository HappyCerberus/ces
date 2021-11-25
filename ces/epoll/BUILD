load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "socket",
    srcs = ["socket.cc"],
    hdrs = ["socket.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":epoll-fwd",
        "//ces/utils:observer",
    ],
)

cc_library(
    name = "epoll-fwd",
    hdrs = ["epoll-fwd.h"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "epoll-emitter",
    srcs = ["epoll-emitter.cc"],
    hdrs = ["epoll-emitter.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//ces:event-emitter",
    ],
)
