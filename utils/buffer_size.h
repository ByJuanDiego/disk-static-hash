//
// Created by juan diego on 9/4/23.
//

#ifndef STATIC_HASH_BUFFER_SIZE_H
#define STATIC_HASH_BUFFER_SIZE_H

#if defined(_WIN64)
#include <windows.h>

#elif defined(__unix__)
#include <unistd.h>

#endif


unsigned long get_buffer_size() {
#if defined(_WIN64)
    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);
    DWORD page_size = systemInfo.dwPageSize;
    return page_size;
#elif defined(__unix__)
    return getpagesize();
#endif
}


#endif //STATIC_HASH_BUFFER_SIZE_H
