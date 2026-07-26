#if defined(__APPLE__)
    #if !defined(__cplusplus)
    #endif

    #include <TargetConditionals.h>
    #if defined(TARGET_OS_IPHONE) && !TARGET_OS_IPHONE
        // MacOS
        #define PLATFORM_MACOS
    #else
        // iOS or iOS Simulator
        #define PLATFORM_IOS
    #endif
#elif defined(__EMSCRIPTEN__)
    #define PLATFORM_WEB
#elif defined(_WIN32)
    #define PLATFORM_WINDOWS
#elif defined(__ANDROID__)
    #define PLATFORM_ANDROID
#elif defined(__linux__) || defined(__unix__)
    #define PLATFORM_LINUX
#else
#error "platform_defs.h: Unknown platform"
#endif

#if defined(PLATFORM_MACOS) || defined(PLATFORM_LINUX) || defined(PLATFORM_WINDOWS)
    #define PLATFORM_DESKTOP
#endif

#if defined(PLATFORM_IOS) || defined(PLATFORM_ANDROID)
    #define PLATFORM_MOBILE
#endif

#if defined(PLATFORM_MACOS) || defined(PLATFORM_IOS)
    #define PLATFORM_DARWIN
#endif

#if defined(PLATFORM_WEB)
    #if defined(HEADLESS_ONLY)
    #define PLATFORM_WEBSERVER
    #else
    #define PLATFORM_WEBBROWSER
    #endif
#endif