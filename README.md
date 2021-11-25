# CES - Coroutines Epoll and Sockets

This is a demonstration project for a complex coroutine use case.

Full documentation with explanation is available at https://simontoth.eu/ces.

## Technical notes

This project uses the Bazel build system and was tested with trunk version of GCC 12.

### Things that can be better / potential TODO

- The library supports first-class timeouts, but no cancelation yet.
- GCC was crashing on POSIX headers when using modules, so no modules for now.
- I'm not happy with the design around universal-awaiter / AwaitableData / Emitters.
- No Conan package yet.
- I would love some performance comparison against normal threaded approach, but I honestly have no idea how to meaningfully benchmark something this light and I/O bound.
