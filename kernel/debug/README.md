# Debugging Utilities
This folder contains GDB plugin utilities to be used when debugging the Kernel.

# Debugging the Kernel
Since we will be using a virtual machine (QEMU) to run our system, we can
connect a debugger from the host machine to see what is going on. To do so, the
`run` command must be used with the `-g` option:
```
./run.sh -g
```
Once done, you will see
```
INF	0	Waiting for gdb connection...
```
Now `cd` to the directory where the `run` command was executed and start `gdb`.
In case of error, try:
```
echo "add auto-load-safe-path ." >>   ̃/.gdbinit
```

# GDB Commands
Some additionals command are available whne using GDB to debug the kernel: the
list of active processes can be printed using
```
process list
```
process details can be dumped using
```
process dump [id]
```
sempahores with processes in their queue can be listed using
```
sem list
```
all available semaphores can be listed using
```
sem list all
```

