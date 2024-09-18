/*
MIT License

Copyright (c) 2010-2020 Stephane Cuillerdier (aka Aiekick)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/*
for use the lib you need to define this the implem one time only before include
EzLog ou EzTools
#define EZ_LOG_IMPLEMENTATION
#include "EzTools.hpp"
or
#include "EzLog.hpp"
*/
#pragma once
#pragma warning(disable : 4251)

#include "EzStr.hpp"

#ifdef USE_GLFW3
#include <GLFW/glfw3.h>
#elif defined(USE_SDL2)
#include <SDL.h>
#endif
#if defined(TRACY_ENABLE) && defined(LOG_TRACY_MESSAGES)
#include <tracy/Tracy.hpp>
#endif

#include <Windows.h>

#include <cstdarg> /* va_list, va_start, va_arg, va_end */

#include <iostream> // std::cout

#include <mutex>
#include <cassert>
#include <string>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <memory>
#include <functional>
using namespace std;

typedef long long int64;

#ifdef MSVC
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#define IsVerboseMode (ez::Log::Instance()->ConsoleVerbose == true)

// #define LogVar(s, ...) ez::Log::Instance()->LogStringWithFunction(std::string(__FUNCTION__), (int)(__LINE__), s,
// ##__VA_ARGS__)

#define LogVarError(s, ...) \
    ez::Log::Instance()->LogStringByTypeWithFunction(ez::Log::LOGGING_MESSAGE_TYPE_ERROR, std::string(__FUNCTION__), (int)(__LINE__), s, ##__VA_ARGS__)

#define LogVarWarning(s, ...) \
    ez::Log::Instance()->LogStringByTypeWithFunction(ez::Log::LOGGING_MESSAGE_TYPE_WARNING, std::string(__FUNCTION__), (int)(__LINE__), s, ##__VA_ARGS__)

#define LogVarInfo(s, ...) \
    ez::Log::Instance()->LogStringByTypeWithFunction(ez::Log::LOGGING_MESSAGE_TYPE_INFOS, std::string(__FUNCTION__), (int)(__LINE__), s, ##__VA_ARGS__)

#define LogVarDebugError(s, ...) \
    ez::Log::Instance()->LogStringByTypeWithFunction_Debug(ez::Log::LOGGING_MESSAGE_TYPE_ERROR, std::string(__FUNCTION__), (int)(__LINE__), s, ##__VA_ARGS__)

#define LogVarDebugWarning(s, ...) \
    ez::Log::Instance()->LogStringByTypeWithFunction_Debug(ez::Log::LOGGING_MESSAGE_TYPE_WARNING, std::string(__FUNCTION__), (int)(__LINE__), s, ##__VA_ARGS__)

#define LogVarDebugInfo(s, ...) \
    ez::Log::Instance()->LogStringByTypeWithFunction_Debug(ez::Log::LOGGING_MESSAGE_TYPE_INFOS, std::string(__FUNCTION__), (int)(__LINE__), s, ##__VA_ARGS__)

#define LogVarLightError(s, ...) ez::Log::Instance()->LogSimpleStringByType(ez::Log::LOGGING_MESSAGE_TYPE_ERROR, s, ##__VA_ARGS__)

#define LogVarLightWarning(s, ...) ez::Log::Instance()->LogSimpleStringByType(ez::Log::LOGGING_MESSAGE_TYPE_WARNING, s, ##__VA_ARGS__)

#define LogVarLightInfo(s, ...) ez::Log::Instance()->LogSimpleStringByType(ez::Log::LOGGING_MESSAGE_TYPE_INFOS, s, ##__VA_ARGS__)

#define LogVarTag(t, s, ...) ez::Log::Instance()->LogStringByTypeWithFunction(t, std::string(__FUNCTION__), (int)(__LINE__), s, ##__VA_ARGS__)

#define LogVarLightTag(t, s, ...) ez::Log::Instance()->LogSimpleStringByType(t, s, ##__VA_ARGS__)

#define LogAssert(a, b, ...)               \
    if (!(a)) {                            \
        LogVarDebugInfo(b, ##__VA_ARGS__); \
        assert(a);                         \
    }

#ifdef USE_OPENGL
#define LogGlError() ez::Log::Instance()->LogGLError("" /*__FILE__*/, __FUNCTION__, __LINE__, "")
#define LogGlErrorVar(var) ez::Log::Instance()->LogGLError("" /*__FILE__*/, __FUNCTION__, __LINE__, var)
#endif

namespace ez {

class Log {
public:
    typedef int MessageType;
    enum MessageTypeEnum { LOGGING_MESSAGE_TYPE_INFOS = 0, LOGGING_MESSAGE_TYPE_WARNING, LOGGING_MESSAGE_TYPE_ERROR };

public:
    static std::function<void(const int& vType, const std::string& vMessage)> sStandardLogFunction;
    static std::function<void(const int& vType, const std::string& vMessage)> sOpenGLLogFunction;

protected:
    static std::mutex Logger_Mutex;

private:
    static ofstream debugLogFile;
    static size_t constexpr sMAX_BUFFER_SIZE = 1024U * 3U;
    int64 lastTick = 0;

private:
    void m_LogString(const MessageType* vType, const std::string* vFunction, const int* vLine, const char* vStr) {
        const int64 ticks = time::getTicks();
        const double time = (ticks - lastTick) / 100.0;

        static char TempBufferBis[sMAX_BUFFER_SIZE + 1];
        int w = 0;
        if (vFunction && vLine)
            w = snprintf(TempBufferBis, sMAX_BUFFER_SIZE, "[%010.3fs][%s:%i] %s", time, vFunction->c_str(), *vLine, vStr);
        else
            w = snprintf(TempBufferBis, sMAX_BUFFER_SIZE, "[%010.3fs] %s", time, vStr);
        if (w) {
            const std::string msg = std::string(TempBufferBis, (size_t)w);

            puMessages.push_back(msg);

#if defined(TRACY_ENABLE) && defined(LOG_TRACY_MESSAGES)
            TracyMessageL(puMessages[puMessages.size() - 1U].c_str());
#endif

            std::cout << msg << std::endl;

            if (vStr && sStandardLogFunction) {
                int type = 0;

                if (vType) {
                    type = (int)(*vType);
                }

                auto arr = str::splitStringToVector(msg, '\n');
                if (arr.size() == 1U) {
                    sStandardLogFunction(type, msg);
                } else {
                    for (auto m : arr) {
                        sStandardLogFunction(type, m);
                    }
                }
            }

            if (!debugLogFile.bad())
                debugLogFile << msg << std::endl;
        }
    }
    void m_LogString(const MessageType* vType, const std::string* vFunction, const int* vLine, const char* fmt, va_list vArgs) {
        static char TempBuffer[sMAX_BUFFER_SIZE + 1];
        int w = vsnprintf(TempBuffer, sMAX_BUFFER_SIZE, fmt, vArgs);
        if (w) {
            m_LogString(vType, vFunction, vLine, TempBuffer);
        }
    }

public:
    static ez::Log* Instance() {
        static auto instance_ptr = std::unique_ptr<ez::Log>(new ez::Log());// std::make_unique is not available in cpp11
        return instance_ptr.get();
    }

public:
    bool ConsoleVerbose = false;
    // file, function, line, msg
    std::vector<std::string> puMessages;

public:
    Log() {
#if defined(TRACY_ENABLE) && defined(LOG_TRACY_MESSAGES)
        ZoneScoped;
#endif
        std::unique_lock<std::mutex> lck(ez::Log::Logger_Mutex, std::defer_lock);
        lck.lock();
        debugLogFile.open("debug.log", ios::out);
        lastTick = time::getTicks();
        ConsoleVerbose = false;
        lck.unlock();
    }
    ~Log() {
#if defined(TRACY_ENABLE) && defined(LOG_TRACY_MESSAGES)
        ZoneScoped;
#endif
        Close();
    }
    void LogSimpleString(const char* fmt, ...) {
#if defined(TRACY_ENABLE) && defined(LOG_TRACY_MESSAGES)
        ZoneScoped;
#endif
        std::unique_lock<std::mutex> lck(ez::Log::Logger_Mutex, std::defer_lock);
        lck.lock();
        va_list args;
        va_start(args, fmt);
        m_LogString(nullptr, nullptr, nullptr, fmt, args);
        va_end(args);
        lck.unlock();
    }
    void LogSimpleStringByType(const MessageType& vType, const char* fmt, ...) {
#if defined(TRACY_ENABLE) && defined(LOG_TRACY_MESSAGES)
        ZoneScoped;
#endif
        std::unique_lock<std::mutex> lck(ez::Log::Logger_Mutex, std::defer_lock);
        lck.lock();
        va_list args;
        va_start(args, fmt);
        m_LogString(&vType, nullptr, nullptr, fmt, args);
        va_end(args);
        lck.unlock();
    }
    void LogStringWithFunction(const std::string& vFunction, const int& vLine, const char* fmt, ...) {
#if defined(TRACY_ENABLE) && defined(LOG_TRACY_MESSAGES)
        ZoneScoped;
#endif
        std::unique_lock<std::mutex> lck(ez::Log::Logger_Mutex, std::defer_lock);
        lck.lock();
        va_list args;
        va_start(args, fmt);
        m_LogString(nullptr, &vFunction, &vLine, fmt, args);
        va_end(args);
        lck.unlock();
    }
    void LogStringByTypeWithFunction(const MessageType& vType, const std::string& vFunction, const int& vLine, const char* fmt, ...) {
#if defined(TRACY_ENABLE) && defined(LOG_TRACY_MESSAGES)
        ZoneScoped;
#endif
        std::unique_lock<std::mutex> lck(ez::Log::Logger_Mutex, std::defer_lock);
        lck.lock();
        va_list args;
        va_start(args, fmt);
        m_LogString(&vType, &vFunction, &vLine, fmt, args);
        va_end(args);
        lck.unlock();
    }
    void LogStringByTypeWithFunction_Debug(const MessageType& vType, const std::string& vFunction, const int& vLine, const char* fmt, ...) {
#ifdef _DEBUG
#if defined(TRACY_ENABLE) && defined(LOG_TRACY_MESSAGES)
        ZoneScoped;
#endif
        std::unique_lock<std::mutex> lck(ez::Log::Logger_Mutex, std::defer_lock);
        lck.lock();
        va_list args;
        va_start(args, fmt);
        m_LogString(&vType, &vFunction, &vLine, fmt, args);
        va_end(args);
        lck.unlock();
#else
        UNUSED(vFunction);
        UNUSED(vLine);
        UNUSED(fmt);
#endif
    }
    void LogStringWithFunction_Debug(const std::string& vFunction, const int& vLine, const char* fmt, ...) {
#ifdef _DEBUG
#if defined(TRACY_ENABLE) && defined(LOG_TRACY_MESSAGES)
        ZoneScoped;
#endif
        std::unique_lock<std::mutex> lck(ez::Log::Logger_Mutex, std::defer_lock);
        lck.lock();
        va_list args;
        va_start(args, fmt);
        m_LogString(nullptr, &vFunction, &vLine, fmt, args);
        va_end(args);
        lck.unlock();
#else
        UNUSED(vFunction);
        UNUSED(vLine);
        UNUSED(fmt);
#endif
    }
#ifdef USE_OPENGL
    bool LogGLError(const std::string& vFile, const std::string& vFunc, int vLine, const std::string& vGLFunc = "") const {
        UNUSED(vFile);

#if defined(TRACY_ENABLE) && defined(LOG_TRACY_MESSAGES)
        ZoneScoped;
#endif
        if (!ez::Log::Instance()->ConsoleVerbose)
            return false;

        const GLenum err(glGetError());
        if (err != GL_NO_ERROR) {
            std::string error;

            switch (err) {
                case GL_INVALID_OPERATION: error = "INVALID_OPERATION"; break;
                case GL_INVALID_ENUM: error = "INVALID_ENUM"; break;
                case GL_INVALID_VALUE: error = "INVALID_VALUE"; break;
                case GL_OUT_OF_MEMORY: error = "OUT_OF_MEMORY"; break;
                case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
                case GL_STACK_UNDERFLOW: error = "GL_STACK_UNDERFLOW"; break;
                case GL_STACK_OVERFLOW: error = "GL_STACK_OVERFLOW"; break;
            }

            if (!error.empty()) {
                const int64 ticks = ct::GetTicks();
                const float time = (ticks - lastTick) / 1000.0f;

                std::string msg;

                if (!vGLFunc.empty()) {
#ifdef USE_GLFW3
                    msg = ct::toStr(
                        "[%010.3fs][GLFW3 0x%X][%s:%i] %s in %s\n", time, (uintptr_t)glfwGetCurrentContext(), vFunc.c_str(), vLine, error.c_str(), vGLFunc.c_str());
#elif defined(USE_SDL2)
                    msg = ct::toStr(
                        "[%010.3fs][SDL2 0x%X][%s:%i] %s in %s\n", time, (uintptr_t)SDL_GL_GetCurrentContext(), vFunc.c_str(), vLine, error.c_str(), vGLFunc.c_str());
#endif
                } else {
#ifdef USE_GLFW3
                    msg = ct::toStr("[%010.3fs][GLFW3 0x%X][%s:%i] %s\n", time, (uintptr_t)glfwGetCurrentContext(), vFunc.c_str(), vLine, error.c_str());
#elif defined(USE_SDL2)
                    msg = ct::toStr("[%010.3fs][SDL2 0x%X][%s:%i] %s\n", time, (uintptr_t)SDL_GL_GetCurrentContext(), vFunc.c_str(), vLine, error.c_str());
#endif
                }

                LogVarLightError("%s", msg.c_str());

                if (sOpenGLLogFunction) {
                    auto arr = ct::splitStringToVector(msg, '\n');
                    if (arr.size() == 1U) {
                        sOpenGLLogFunction(2, msg);
                    } else {
                        for (auto m : arr) {
                            sOpenGLLogFunction(2, m);
                        }
                    }
                }

                if (!debugLogFile.bad())
                    debugLogFile << msg << std::endl;

                return true;
            }
        }

        return false;
    }
#endif
    void Close() {
#if defined(TRACY_ENABLE) && defined(LOG_TRACY_MESSAGES)
        ZoneScoped;
#endif
        std::unique_lock<std::mutex> lck(ez::Log::Logger_Mutex, std::defer_lock);
        lck.lock();
        debugLogFile.close();
        debugLogFile.clear();
        lck.unlock();
    }

public:
    std::string GetLastErrorAsString() {
        std::string msg;

#ifdef WIN32
        // Get the error message, if any.
        const DWORD errorMessageID = ::GetLastError();
        if (errorMessageID == 0 || errorMessageID == 6)
            return std::string();  // No error message has been recorded

        LPSTR messageBuffer = nullptr;
        const size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                           nullptr,
                                           errorMessageID,
                                           MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                           (LPSTR)&messageBuffer,
                                           0,
                                           nullptr);

        msg = std::string(messageBuffer, size);

        // Free the buffer.
        LocalFree(messageBuffer);
#else
        // cAssert(0, "to implement");
#endif
        return msg;
    }
};

#ifdef EZ_LOG_IMPLEMENTATION
std::function<void(const int& vType, const std::string& vMessage)> Log::sStandardLogFunction = nullptr;
std::function<void(const int& vType, const std::string& vMessage)> Log::sOpenGLLogFunction = nullptr;
ofstream Log::debugLogFile;
std::mutex Log::Logger_Mutex;
#endif // EZ_LOG_IMPLEMENTATION

}  // namespace ez
