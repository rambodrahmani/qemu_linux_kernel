import gdb

class SavePrefixCommand (gdb.Command):
    "Prefix command for saving things."

    def __init__(self):
        super (SavePrefixCommand, self).__init__("save", gdb.COMMAND_SUPPORT, gdb.COMPLETE_NONE, True)

SavePrefixCommand()

class SaveBreakpointsCommand (gdb.Command):
    """Save the current breakpoints to a file.
This command takes  a single argument, a file
name.
The breakpoints can be restored using the
'source' command."""

    def __init__ (self):
        super (SaveBreakpointsCommand, self).__init__("save breakpoints", gdb.COMMAND_SUPPORT, gdb.COMPLETE_FILENAME)

    def invoke (self, arg, from_tty):
        with open (arg, 'w') as f:
            for bp in gdb.breakpoints():
                print("break", bp.location, file=f, end="")
                if bp.thread is not None:
                    print(" thread", bp.thread, file=f, end="")
                if bp.condition is not None:
                    print(" if", bp.condition, file=f, end="")
                print(file=f)
                if not bp.enabled:
                    print("disable $bpnum", file=f)
                # Note: we don't save the ignore count; there doesn't
                # seem to be much point.
                if bp.commands is not None:
                    print("commands", file=f)
                    # Note that COMMANDS has a trailing newline.
                    print(bp.commands, file=f)
                    print("end", file=f)
                print(file=f)

SaveBreakpointsCommand ()
