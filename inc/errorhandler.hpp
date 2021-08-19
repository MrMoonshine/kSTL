#ifndef ERRORHANDLER_HPP
#define ERRORHANDLER_HPP
#include <QString>
#include <cstdio>
namespace ErrorHandle {
typedef enum {DEBUG, INFO, WARNING, ERROR} Severity;
    void logD(const char* TAG, const char* msg);
    void logI(const char* TAG, const char* msg);
    void logW(const char* TAG, const char* msg);
    void logE(const char* TAG, const char* msg);
    void logUniversal(const char* TAG, const char* msg,Severity severe = DEBUG);
}

#endif // ERRORHANDLER_HPP
