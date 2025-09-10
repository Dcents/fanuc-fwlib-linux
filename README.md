# Fanuc FOCAS fwlib32 - CNC Machine Connection
Simple C program that uses Fanuc's FOCAS fwlib32 library to connect to a Fanuc CNC controller.

> Note: Program tested on Fanuc 31i controller (Tsugami SS327-III-5AX)

## Linux 64-bit Support 
The `fwlib32` library uses 4-byte alignment and expects `long` types to be 4 bytes. On 64-bit Linux systems, `long` is 8 bytes, which leads to misaligned struct fields and garbage data returned. To address this, the demo uses a custom header, `fwlib32_compat.h`, which replaces all `long` types with explicit `int32_t` or `uint32_t` types. This ensures consistent 4-byte alignment and correct data handling across platforms.

## Dev
Remember to change the controller IP address and port in `main.c` before testing.

## Build
To build and run the project, you need the linux version of the `fwlib32` library (e.g., libfwlib32-linux-x64.so.1.0.5). You can get the shared object binary file from Fanuc or find it online.

Follow the below steps to build and run:
```sh
# Create sym links for library
ln -s libfwlib32-linux-x64.so.1.0.5 libfwlib32.so
ln -s libfwlib32-linux-x64.so.1.0.5 libfwlib32.so.1
# Build
gcc -L./ -Wl,-rpath . main.c -lfwlib32 -lm -lpthread -o fwlib_demo
# Run
./fwlib_demo
```

## Result
If the connection is successfull, you should see the following information:
- System Info:
    - CNC Type
    - Machine Type
    - Series
    - Version
    - Axes
- Current Program:
    - Running Program Number
    - Main Program Number
- Timer Data:
    - Cycle Time
- Param Info:
    - Data No, Type
    - L Data