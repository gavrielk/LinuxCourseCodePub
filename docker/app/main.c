#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h> 
#include "util_zip.h"

int scandir_filter(const struct dirent* file)
{
    if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0)
    {
        return 0;
    }
    return 1;
}

int main()
{
    struct dirent **namelist;
    int n;

    n = scandir("/root/zip_in", &namelist, scandir_filter, alphasort);
    
    char  *file_paths[n];
    for (int i = 0; i < n; i++)
    {
        file_paths[i] = namelist[i]->d_name;
    }
   
    chdir("/root/zip_in"); 
    char  *zip_out_file = "/root/zip_out/zipped.zip";
    UTIL_ZIP_create(file_paths, n, zip_out_file);

    for (int i = 0; i < 100; i++)
    {
        free(namelist[i]);
    }
    free(namelist);

    return 0;
}
