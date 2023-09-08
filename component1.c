#include "command.h"

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