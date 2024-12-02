#include "my_dir.h"

int my_rmdir(const char *path)
{
    if (rmdir(path) == 0)
    {
        printf("Directory '%s' removed successfully.\n", path);
        return 0;
    }
    else
    {
        perror("rmdir");
        return -1;
    }
}
