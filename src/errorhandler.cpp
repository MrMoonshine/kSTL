#include <errorhandler.hpp>

void ErrorHandle::logD(const char* TAG, const char* msg){
#ifndef NODEBUG
    logUniversal(TAG, msg, ErrorHandle::DEBUG);
#endif
}

void ErrorHandle::logI(const char* TAG, const char* msg){
    logUniversal(TAG, msg, ErrorHandle::INFO);
}

void ErrorHandle::logW(const char* TAG, const char* msg){
    logUniversal(TAG, msg, ErrorHandle::WARNING);
}

void ErrorHandle::logE(const char* TAG, const char* msg){
    logUniversal(TAG, msg, ErrorHandle::ERROR);
}

void ErrorHandle::logUniversal(const char* TAG, const char* msg, ErrorHandle::Severity severe){
#ifndef NOCOLOR
    switch (severe) {
        case INFO: printf("\033[0;32m"); break;
        case WARNING: printf("\033[0;33m"); break;
        case ERROR: printf("\033[0;31m"); break;
        default: printf("\033[0;37m"); break;
    }
#endif
    printf("[%s] ", TAG);
    switch (severe) {
        case INFO: printf("INFO"); break;
        case WARNING: printf("WARNING"); break;
        case ERROR: printf("ERROR"); break;
        default: break;
    }
    printf(": %s\n", msg);
#ifndef NOCOLOR
    printf("\033[0m");
#endif
}
