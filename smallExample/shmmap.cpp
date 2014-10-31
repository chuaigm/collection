#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h> // 为文件描述符提供控制

#include <assert.h>

#define FILE_SIZE 128
const char* shmfilename="shmfile";

typedef struct testshm {
	int a;
	char b[10];
}testshm;

int main()
{
	// create shm
	int fd=shm_open(shmfilename, O_RDWR | O_CREAT, 0777);
	assert(fd >= 0);

	// set file size
	int ret = ftruncate(fd, FILE_SIZE);
	assert(0==ret);
	
	// mmap
	void *shmptr = mmap(0, FILE_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	assert(MAP_FAILED!=shmptr);

	// read
	testshm bb;
	memcpy(&bb, shmptr, sizeof(testshm));
	printf("1before==%4d==%8s==\n",bb.a, bb.b);

	// write 
	testshm aa;
	aa.a=5;
	strcpy(aa.b, "abcdefg");
	printf("2write ==%4d==%8s==\n",aa.a, aa.b);
	// write 
	memcpy(shmptr, &aa, sizeof(testshm));

	// read
	memcpy(&bb, shmptr, sizeof(testshm));
	printf("3after ==%4d==%8s==\n",bb.a, bb.b);

	//munmap
	ret = munmap(shmptr, FILE_SIZE);
	shmptr = NULL;
	assert(0==ret);

#if 1
	//shm_unlink, means delete shm file
	// in linux, in /dev/shm
	ret = shm_unlink(shmfilename);
	assert(0==ret);
#endif
}

