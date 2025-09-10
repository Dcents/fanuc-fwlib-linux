#include <stdio.h>
#include <string.h>
#include <stdint.h>
// Workaround for 64-bit linux to replace long with int32_t and ensure 4-byte alignment
#include "fwlib32_compat.h"

int main() {
    short ret;
    unsigned short handle;
    const char* ip = "192.168.1.111";  // Machine IP
    unsigned short port = 8193; // FOCAS port

// Required for linux
#ifndef _WIN32
    if (cnc_startupprocess(0, "focas.log") != EW_OK) {
        fprintf(stderr, "Failed to create required log file!\n");
    return 1;
    }
#endif

    // Connect to the CNC
    ret = cnc_allclibhndl3(ip, port, 10, &handle);
    if (ret != EW_OK) {
        printf("Failed to connect to CNC. Error code: %d\n", ret);
        return 1;
    }
    printf("Connected to CNC. Handle = %d\n", handle);

    // Read system info
    ODBSYS sysinfo;
    ret = cnc_sysinfo(handle, &sysinfo);
    if (ret == EW_OK) {
        printf("System Info:\n");
        printf("  CNC Type: %.*s\n", 2,  sysinfo.cnc_type);
        printf("  Mach Type: %.*s\n", 2,  sysinfo.mt_type);
        printf("  Series: %.*s\n", 4,  sysinfo.series);
        printf("  Version: %.*s\n", 4,  sysinfo.version);
        printf("  Axes: %.*s\n", 2,  sysinfo.axes);
    } else {
        printf("Failed to read system info. Error: %d\n", ret);
    }

    // Read current program number
    ODBPRO odbpro;
    ret = cnc_rdprgnum(handle, &odbpro);
    if (ret == EW_OK) {
        printf("Current Program:\n");
        printf("  Running Program Number: O%d\n", odbpro.data);
        printf("  Main Program Number: O%d\n", odbpro.mdata);
    } else {
        printf("Failed to read current program number. Error: %d\n", ret);
    }

    // Read timer data like cutting time, cycle time
    IODBTIME timeData;
    ret = cnc_rdtimer(handle, 3, &timeData); // 3=Cycle Time
    if (ret == EW_OK) {
        printf("Timer Data:\n");
        printf("  Cycle Time: %d:%02d.%03d (MM:SS.mmm)\n", timeData.minute, timeData.msec / 1000, timeData.msec % 1000);
    } else {
        printf("Failed to read cycle/cutting time. Error: %d\n", ret);
    }

    // Read parameter info
    // Example: 6711 (Part Count)
    IODBPSD param;
    ret = cnc_rdparam(handle, 6711, 0, sizeof(IODBPSD), &param);
    if (ret == EW_OK) {
        printf("Param Info:\n");
	    printf(" Data No: %d, Type: %d\n", param.datano, param.type);
        printf(" L Data: %d\n", param.u.ldata);
    } else {
        printf("Failed to read parameter info. Error: %d\n", ret);
    }

    // Disconnect
    ret = cnc_freelibhndl(handle);
    if (ret != EW_OK) {
        printf("Failed to release handle. Error: %d\n", ret);
    } else {
        printf("Disconnected from CNC.\n");
    }
// Required for linux
#ifndef _WIN32
    cnc_exitprocess();
#endif

    return 0;
}
