//
// Created by Daniel Laplana Gimeno on 5/12/24.
//
#include <iostream>

#ifndef AURAGL_ASSERTION_H
#define AURAGL_ASSERTION_H


#define COMETA_DEBUG

#ifdef COMETA_DEBUG
    #define COMETA_ASSERT(x) Assertion::Assert(x)
    #define COMETA_WARNING(x) Assertion::Warning(x)
    #define COMETA_ERROR(x) Assertion::Error(x)
    #define COMETA_MSG(...) Assertion::Message(__VA_ARGS__)
#else
    #define COMETA_ASSERT(x) ((void)0)
    #define COMETA_WARNING(x) ((void)0)
    #define COMETA_ERROR(x) ((void)0)
    #define COMETA_MSG(...) ((void)0)
#endif

class Assertion {
public:
    inline static void Assert(const char* msg){
        std::cout << "[INFO]: " << msg << std::endl;
    }

    inline static void Error(const char* msg){
        std::cout << "[ERROR]: " << msg << std::endl;
    }

    inline static void Warning(const char* msg){
        std::cout << "[WARNING]: " << msg << std::endl;
    }

    template<typename... Args>
    inline static void Message(const Args&... args) {
        std::string result;
        (append(result, args), ...);
        std::cout << "[MESSAGE]: " << result << std::endl;
    }

private:
    /**
     * Append a value to a string if its not an string or a char*
     */
    template<typename T>
    static void append(std::string& result, const T& value) {
        result += std::to_string(value);
    }

    /**
     * Append a string to a string
     */
    static void append(std::string& result, const char* value) {
        result += value;
    }

    /**
     * Append a string to a string
     */
    static void append(std::string& result, const std::string& value) {
        result += value;
    }
};

#endif //AURAGL_ASSERTION_H
