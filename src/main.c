#include <stdio.h>

int main(int argc, char **argv) {

#if defined(PLATFORM_GITHUB_WORKFLOW)
    printf("Hello, Workflow!\n");
#else
    printf("Hello, World!\n");
    return 1;
#endif

    return 0;
}
