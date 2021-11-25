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
