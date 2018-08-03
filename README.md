# Predep Language

This is an experimental language to show a proof-of-concept that a dependently typed language can be imperitive while also incurring no runtime-overhead

The backend is in LLVM, so it will compile directly to assembly, and without a garbage collector.

We plan to eventually move to the official Dep language once most of the language specification has been worked out. Some features are still up in the air. For instance, we would like to require move semantics a la Rust, but we may end up just going to non-nullable types a la Kotlin. 
