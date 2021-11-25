load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "example-async-main",
    srcs = ["example-async-main.cc"],
    deps = [
        "//ces/coroutines:synchronous",
    ],
)

cc_binary(
    name = "example-symetric-transfer",
    srcs = ["example-symetric-transfer.cc"],
    deps = [
        "//ces/coroutines:chainable",
        "//ces/coroutines:synchronous",
    ],
)

cc_binary(
    name = "example-http-client",
    srcs = ["example-http-client.cc"],
    deps = [
        "//ces/async:async-connect",
        "//ces/async:async-read",
        "//ces/async:async-write",
        "//ces/coroutines:scheduler",
        "//ces/epoll:epoll-emitter",
        "//ces/timeouts:timeout-emitter",
    ],
)

cc_binary(
    name = "example-echo-server",
    srcs = ["example-echo-server.cc"],
    deps = [
        "//ces/async:async-connect",
        "//ces/async:async-read",
        "//ces/async:async-server",
        "//ces/async:async-write",
        "//ces/conditions:condition-emitter",
        "//ces/coroutines:scheduler",
        "//ces/coroutines:terminating",
        "//ces/epoll:epoll-emitter",
        "//ces/timeouts:timeout-emitter",
    ],
)
