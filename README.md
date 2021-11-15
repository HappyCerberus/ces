# CES - Coroutines Epoll and Sockets

This is a demonstration project for a complex coroutine use case.

## Non-production code

Please note, that because of the nature of this project, using this code in production environment is inadvisable. Notably:

- to clearly demonstrate the asynchronous nature of coroutines, this project is strictly single-threaded
- there is no effort done to address the starvation issues presented by these interfaces
  - epoll itself makes no starvation guarantees
  - our epoll loop fetches one event at a time, with zero consideration towards wait times when multiple sockets are ready
  - due to the single-threaded nature, a non-cooperative coroutine can block all other progress
- the main focus of this project is the coroutine machinery and minimal effort was invested into proper handling of sockets
- compiler support and this code itself is still very fresh
  - this project was developed using TRUNK version of GCC (12)

## How does it work

Watch this space for a much longer explanation coming soon. Here are the main points:

### Coawaitable coroutines with symetric transfer

The core of the coroutine side of things is the `chainable_task` class in `lib/coroutines.h`. This class supports coroutine chaining using `co_await`.

```
chainable_task async_operation();

synchronous_task my_coro() {
    auto status = co_await async_operation();
}
```

The execution will look something like this `[my_coro]->[async_operation]->[my_coro]`. Note that the important point here is that we are not stacking these coroutines, i.e. `my_coro` is not calling `async_operation` and that isn't calling `my_coro` we are chaining them, thus preventing stack blowup. Since `async_operation` will likely handoff to `epoll` to wait for I/O to be ready the actual execution will look like `[my_coro]->[async_operation]->[epoll_loop]->[async_operation]->[epoll_loop]->[async_operation]->[my_coro]`.

