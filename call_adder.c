
#include "adderi.h"

// The run-function required by the wit interface
bool exports_wasi_cli_run_run() {

    wasi_cli_stdout_output_stream_t stdout_stream = wasi_cli_stdout_get_stdout();

    int32_t added = wasmtime_wasi_adder_add(23, 19);

    char * str = (added != 23+19) ? "Wrong value\n" : "Hello component!\n";

    adderi_list_u8_t str_p;
    str_p.ptr = str;
    str_p.len = strlen(str);

    adderi_tuple2_u64_wasi_io_streams_stream_status_t ret;

    // Doing some extra outputs to force a flush as blocking doesn't worj properly
    wasi_io_streams_blocking_write(stdout_stream, &str_p, &ret);
    wasi_io_streams_blocking_write(stdout_stream, &str_p, &ret);

    return true;
}