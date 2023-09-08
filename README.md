# An example of WebAssembly components in C and how to run it with wasmtime cli

## Tools

Required tooling:

* [Install rust](https://www.rust-lang.org/tools/install)
* [`wasm-tools`](https://github.com/bytecodealliance/wasm-tools/)
* [`wit-bindgen`](https://github.com/bytecodealliance/wit-bindgen)
* [`wasi-sdk`](https://github.com/Webassembly/wasi-sdk)
* To run the component outside browser you need wasmtime with component model feature enabled


### `wasm-tools`

Installation instruction:
```
cargo install wasm-tools
```

### `wit-bindgen`

Installation instruction:
```
cargo install --git https://github.com/bytecodealliance/wit-bindgen wit-bindgen-cli
```

### `wasi-sdk`

Install a recent (version 20 tried here) version of wasi-sdk. These instructions assume wasi-sdk is installed in `/opt/wasi-sdk`

### wasmtime

Build a recent wasmtime from source (you will need the source for the wit-file later):

```
git clone https://github.com/bytecodealliance/wasmtime.git
cd wasmtime
git submodule update --init
cargo build --features=component-model --release
```

Put `target/release/wasmtime` somewhere in `PATH`.

## Hello component

Here we implement a simple component to the wasi:cli/command world interface that modern wasmtime can use. Instead of a `main` function, the component needs to implement a `run` function. 

### Link to the wasi:cli/command world wit in wasmtime and build the interface files

```
ln -s <wasmtime-directory>/crates/wasi/wit wit
wit-bindgen c wit  --world=wasi:cli/command
```

This will create:
```
Generating "command.c"
Generating "command.h"
Generating "command_component_type.o"
```

### Write the component

Check the file `component1.c`

```
#include "command.h"

// The run-function required by the wit interface
bool exports_wasi_cli_run_run() {

    wasi_cli_stdout_output_stream_t stdout_stream = wasi_cli_stdout_get_stdout();

    const char * str = "Hello component!\n";

    command_list_u8_t str_p;
    str_p.ptr = str;
    str_p.len = strlen(str);

    command_tuple2_u64_wasi_io_streams_stream_status_t ret;

    // Doing some extra outputs to force a flush as blocking doesn't worj properly
    wasi_io_streams_blocking_write(stdout_stream, &str_p, &ret);
    wasi_io_streams_blocking_write(stdout_stream, &str_p, &ret);

    return true;
}
```

### Compile
```
/opt/wasi-sdk/bin/clang \
    command.c component1.c \
    command_component_type.o \
    -o component1-core.c.wasm -mexec-model=reactor
```
Convert the resulting core webassembly file to a webassembly component:

`wasm-tools component new component1-core.c.wasm -o component1.c.wasm`

### Run the resulting component

```
 wasmtime component1.c.wasm --wasm-features component-model
```

The output should be:

```
Hello component!
```

Note: the output should really be the text repeated twice, but due to a bug in wasmtime, the blocked output does not work properly at this time.

