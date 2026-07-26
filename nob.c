#define NOB_IMPLEMENTATION
#include "includes/nob.h/nob.h"

#include "includes/platform_defs.h"

// Some folder paths that we use throughout the build process.
#define SRC_FOLDER   "src/"

#if defined(PLATFORM_MACOS)
#define BUILD_FOLDER "build/macos/"
#elif defined(PLATFORM_LINUX)
#define BUILD_FOLDER "build/linux/"
#elif defined(PLATFORM_WINDOWS)
#define BUILD_FOLDER "build/windows/"
#elif defined(PLATFORM_WEB)
#define BUILD_FOLDER "build/web/"
#endif

int main(int argc, char **argv)
{
    // This line enables the self-rebuilding. It detects when nob.c is updated and auto rebuilds it then
    // runs it again.
    NOB_GO_REBUILD_URSELF(argc, argv);

    // It's better to keep all the building artifacts in a separate build folder. Let's create it if it
    // does not exist yet.
    //
    // Majority of the nob command return bool which indicates whether operation has failed or not (true -
    // success, false - failure). If the operation returned false you don't need to log anything, the
    // convention is usually that the function logs what happened to itself. Just do
    // `if (!nob_function()) return;`
    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return 1;

    // The working horse of nob is the Nob_Cmd structure. It's a Dynamic Array of strings which represent
    // command line that you want to execute.
    Nob_Cmd cmd = {0};

    nob_cc(&cmd);
    nob_cc_flags(&cmd);
    nob_cc_output(&cmd, BUILD_FOLDER "main");
    nob_cc_inputs(&cmd, SRC_FOLDER "main.c");

#if defined(PLATFORM_GITHUB_WORKFLOW)
    nob_cmd_append(&cmd, "-DPLATFORM_GITHUB_WORKFLOW");
#endif

    if (!nob_cmd_run(&cmd)) return 1;

    return 0;
}
