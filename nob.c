#define NOB_IMPLEMENTATION
#include "includes/nob.h/nob.h"

#include "includes/platform_defs.h"

// Some folder paths that we use throughout the build process.
#define SRC_FOLDER   "src/"
#define BUILD_FOLDER "build/"

#if defined(PLATFORM_MACOS)
#define BUILD_SUBFOLDER "build/macos/"
#elif defined(PLATFORM_LINUX)
#define BUILD_SUBFOLDER "build/linux/"
#elif defined(PLATFORM_WINDOWS)
#define BUILD_SUBFOLDER "build/windows/"
#elif defined(PLATFORM_WEB)
#define BUILD_SUBFOLDER "build/web/"
#endif

int buildExe() {
    Nob_Cmd cmd = {0};

    nob_cc(&cmd);
    nob_cc_flags(&cmd);
    nob_cc_output(&cmd, BUILD_SUBFOLDER "main");
    nob_cc_inputs(&cmd, SRC_FOLDER "main.c");

#if defined(PLATFORM_GITHUB_WORKFLOW)
    nob_cmd_append(&cmd, "-DPLATFORM_GITHUB_WORKFLOW");
#endif

    if (!nob_cmd_run(&cmd)) return 1;

    return 0;
}

int runExe() {
    Nob_Cmd cmd = {0};

#if !defined(PLATFORM_WINDOWS)
    nob_cmd_append(&cmd, "cd", BUILD_SUBFOLDER);
    nob_cmd_append(&cmd, "&&", "./main");

#else
    nob_cmd_append(&cmd, "cd", BUILD_SUBFOLDER);
    nob_cmd_append(&cmd, "&&", "main.exe");
#endif

    if (!nob_cmd_run(&cmd)) return 1;

    return 0;
}

int main(int argc, char **argv)
{
    // This line enables the self-rebuilding. It detects when nob.c is updated and auto rebuilds it then
    // runs it again.
    NOB_GO_REBUILD_URSELF(argc, argv);

    if(buildExe() != 0) { return 1; };
    if(runExe() != 0) { return 1; };

    return 0;
}
