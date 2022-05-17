#include <libgen.h>
#include <zip.h>
#include <stdbool.h>
#include <fcntl.h>
#include <syslog.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include "util_zip.h"

#define ZIP_BUFF_SIZE	(128*1024)
#define FILE_PATH_STRING_SIZE	256
#define NOP {}

#define UTIL_ASSERT_LOG(statmant , whatToDO, format , ...)if(statmant){ \
		printf ( format , ##__VA_ARGS__);whatToDO;} 			\
                                                                    

int is_dir(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

bool UTIL_ZIP_create( char *file_paths[], int n,  char *zip_out_file)
{
	struct zip *za = NULL;
	struct zip_source* zs = NULL;
	char err_buf[FILE_PATH_STRING_SIZE];
	int err;
	int i;
	bool ret = false;
	FILE *fp;

	UTIL_ASSERT_LOG(NULL == file_paths, return false, "file_paths NULL pointer");
	UTIL_ASSERT_LOG(NULL == zip_out_file, return false, "zip_out_file NULL pointer");

	if ((za = zip_open(zip_out_file, ZIP_CREATE, &err)) == NULL)
	{
		zip_error_to_str(err_buf, sizeof(err_buf), err, errno);
		UTIL_ASSERT_LOG(true,return false,"can't open zip archive %.200s: %.200s",zip_out_file, err_buf);
	}

	for (i = 0; i < n; i++)
	{
		UTIL_ASSERT_LOG((file_paths[i]==NULL),goto ERR_FREE_MEM,"file_paths[%d]==NULL",i);

		if ((fp=fopen(file_paths[i], "r")) == NULL) 
		{
			UTIL_ASSERT_LOG(true,goto ERR_FREE_MEM,"can't open input file '%s': %s",file_paths[i], strerror(errno));
		}

		//zs = zip_source_file(za,file_paths[i],0, 0);
		zs = zip_source_filep(za,fp,0, -1);
		UTIL_ASSERT_LOG((zs == NULL),goto ERR_FREE_MEM,"zip_stat() failed: %.200s",zip_strerror(za));

		//err = zip_add(za,basename(file_paths[i]),zs);
		if (is_dir(file_paths[i]))
		{
			err = zip_dir_add(za,basename(file_paths[i]),0);
			UTIL_ASSERT_LOG((err==-1),goto ERR_FREE_MEM,"zip_dir_add() failed: %.200s",zip_strerror(za));
		}
		else
		{
			err = zip_file_add(za,basename(file_paths[i]),zs,0);
			UTIL_ASSERT_LOG((err==-1),goto ERR_FREE_MEM,"zip_add() failed: %.200s",zip_strerror(za));
		}
		zs=0;
	 }

	ret = true;

	ERR_FREE_MEM:	// free mem

	if(zs)
	{
		zip_source_free(zs);
	}

	if (za)
	{
		err = zip_close(za);
		UTIL_ASSERT_LOG((err != 0),NOP,"zip_close() failed: %.200s",zip_strerror(za));
	}

	return ret;

}




