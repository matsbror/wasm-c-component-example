#!/bin/bash

# assuming wasmtime's source code are at ../../wasmtime
ln -s ../../wasmtime/crates/wasi/wit wit
wit-bindgen c wit  --world=wasi:cli/command

/opt/wasi-sdk/bin/clang \
    command.c component1.c \
    command_component_type.o \
    -o component1-core.c.wasm -mexec-model=reactor

wasm-tools component new component1-core.c.wasm -o component1.c.wasm

wasmtime component1.c.wasm --wasm-features component-model
