import gdb

def frame_list():
    num_df = gdb.parse_and_eval("N_DF")

    for i in range(0, num_df):
        vdf = gdb.parse_and_eval("vdf[" + i + "]")
        yield vdf



