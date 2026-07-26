#define NOB_IMPLEMENTATION
#include "includes/nob.h/nob.h"

#include "includes/platform_defs.h"

// Some folder paths that we use throughout the build process.
#define SRC_FOLDER   "src/"
#define BUILD_FOLDER "build/"
#define INCLUDES_FOLDER   "includes/"

#if defined(PLATFORM_MACOS)
#define BUILD_SUBFOLDER "build/macos/"
#elif defined(PLATFORM_LINUX)
#define BUILD_SUBFOLDER "build/linux/"
#elif defined(PLATFORM_WINDOWS)
#define BUILD_SUBFOLDER "build/windows/"
#elif defined(PLATFORM_WEB)
#define BUILD_SUBFOLDER "build/web/"
#endif

void configure() {
    Nob_Cmd cmd = {0};

#ifdef PLATFORM_LINUX

    nob_cmd_append(&cmd, "sudo", "apt", "install", 
        "libx11-dev", 
        "libxi-dev", 
        "libxcursor-dev", 
        "mesa-common-dev", 
        "libgl1-mesa-dri", 
        "libgbm1"
    );

    if (!nob_cmd_run(&cmd)) {
        exit(1);
    }
#endif /* PLATFORM_LINUX */

#ifdef TARGET_PLATFORM_IOS

    //nob_cmd_append(&cmd, "sudo", "xcode-select", "--install");
    //nob_cmd_append(&cmd, "xcodebuild", "-showsdks");
    nob_cmd_append(&cmd, "ls", "/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/");
    //nob_cmd_append(&cmd, "ls", "/Applications/Xcode.app/Contents/Developer/Platforms/");

    if (!nob_cmd_run(&cmd)) {
        exit(1);
    }

#endif /* PLATFORM_LINUX */

    if (cmd.count) {
        if (!nob_cmd_run(&cmd)) {
            exit(1);
        }
    }
}

int buildExe() {
    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return 1;
    if (!nob_mkdir_if_not_exists(BUILD_SUBFOLDER)) return 1;

    Nob_Cmd cmd = {0};

#ifdef TARGET_PLATFORM_IOS
    nob_cmd_append(&cmd, "xcrun", "-sdk", "iphoneos");

#endif

    nob_cc(&cmd);
    nob_cc_flags(&cmd);

#ifdef PLATFORM_DARWIN
    nob_cc_inputs(&cmd, "-x", "objective-c");
#endif


#ifdef TARGET_PLATFORM_IOS
    //nob_cmd_append(&cmd, "-target", "arm64-apple-ios26");
    //nob_cmd_append(&cmd, "-target", "arm64-apple-ios");
#endif


    nob_cc_inputs(&cmd, "-I", INCLUDES_FOLDER);
    //nob_cmd_append(&cmd, "-I",  "/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk/System/Library/Frameworks");


    nob_cc_inputs(&cmd, SRC_FOLDER "main.c");
    nob_cc_output(&cmd, BUILD_SUBFOLDER "main");


#ifdef PLATFORM_DARWIN
    nob_cmd_append(&cmd, "-framework",  "QuartzCore");

#ifndef TARGET_PLATFORM_IOS
    nob_cmd_append(&cmd, "-framework",  "AppKit");
#else

#define IOS_FRAMEWORKS_DIR "/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk/System/Library/Frameworks/"
    //nob_cmd_append(&cmd, "-F", IOS_FRAMEWORKS_DIR);
    // nob_cmd_append(&cmd, "-framework", IOS_FRAMEWORKS_DIR "UIKit.framework");
    // nob_cmd_append(&cmd, "-framework", IOS_FRAMEWORKS_DIR "Foundation.framework");

    nob_cmd_append(&cmd, "-framework", "UIKit");
    nob_cmd_append(&cmd, "-framework", "Foundation");
    nob_cmd_append(&cmd, "-framework", "CoreGraphics");
#endif

    // Set graphics backend
    nob_cmd_append(&cmd, "-DSOKOL_METAL");
    nob_cmd_append(&cmd, "-framework",  "Metal");
#else
    nob_cmd_append(&cmd, "-DSOKOL_GLCORE");
#endif /* PLATFORM_DARWIN */

#ifdef PLATFORM_LINUX
    // Set graphics backend
    nob_cmd_append(&cmd, "-lX11");
    nob_cmd_append(&cmd, "-lXi"); // XInput
    nob_cmd_append(&cmd, "-lXcursor");
    nob_cmd_append(&cmd, "-lGL");
    nob_cmd_append(&cmd, "-lm");
    // nob_cmd_append(&cmd, "-lopengl32");
#endif

#if defined(PLATFORM_GITHUB_WORKFLOW)
    nob_cmd_append(&cmd, "-DPLATFORM_GITHUB_WORKFLOW");
#endif

    if (!nob_cmd_run(&cmd)) return 1;

    return 0;
}

int runExe() {
    Nob_Cmd cmd = {0};

    nob_set_current_dir(BUILD_SUBFOLDER);

#if !defined(PLATFORM_WINDOWS)
    //nob_cmd_append(&cmd, "cd", "./" BUILD_SUBFOLDER);
    //nob_cmd_append(&cmd, "&&", "./main");
    nob_cmd_append(&cmd, "./main");

#else
    //nob_cmd_append(&cmd, "cd", BUILD_SUBFOLDER);
    //nob_cmd_append(&cmd, "&&", "main.exe");
    nob_cmd_append(&cmd, "main.exe");
#endif

    if (!nob_cmd_run(&cmd)) return 1;

    return 0;
}

int main(int argc, char **argv)
{
    // This line enables the self-rebuilding. It detects when nob.c is updated and auto rebuilds it then
    // runs it again.
    NOB_GO_REBUILD_URSELF(argc, argv);

    configure();

    if(buildExe() != 0) { return 1; };
    
#ifndef PLATFORM_GITHUB_WORKFLOW
    if(runExe() != 0) { return 1; };
#endif

    return 0;
}
