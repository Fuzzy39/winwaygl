#define _POSIX_C_SOURCE 200112L
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>
#include "wayland.h"
#include <wayland-client-protocol.h>



// creates a psudorandom 6 character string (A-O, a-o) based on the clock's time
static void randname(char *buf)
{
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	long r = ts.tv_nsec;
	for (int i = 0; i < 6; ++i) 
    {
        // A + (0-15)+(0, 32) (A-O, a-o)
		buf[i] = 'A'+(r&15)+(r&16)*2;
		r >>= 5;
	}
}


static int create_shm_file(void)
{
	int retries = 100;
	do 
	{
		// generate a radom file name
		char name[] = "/wl_shm-XXXXXX";
		randname(name + sizeof(name) - 7);

		--retries;
		// create a shared memory object. Read/write, return an error if it already exists.
		// not sure what mode is here
		int fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, 0600);
		if (fd >= 0) 
		{
			// unlink (destroy(?)) the object. Why?
			shm_unlink(name);
			return fd;
		}
	} while (retries > 0 && errno == EEXIST);
	return -1;
}

int allocate_shm_file(size_t size)
{
	int fd = create_shm_file();
	
	if (fd < 0)return -1;

	int ret;
	do 
	{
		ret = ftruncate(fd, size);
	} 
	while (ret < 0 && errno == EINTR);

	// ftruncate failed in some unexpected way.
	if (ret < 0)
	{
		close(fd);
		return -1;
	}
	return fd;
}


void create_shm_pool()
{
	
	const int width = 512, height = 512;
	const int bytes_per_pixel = 4;
	// 2 for double buffer.
	
	const int buffer_size = height * width * bytes_per_pixel;
	const int shm_pool_size = buffer_size * 1;

	int fd = allocate_shm_file(shm_pool_size);
	if(fd<0)
	{
		printf("Couldn't make shm.\n");
		exit(1);
	}

	uint8_t *pool_data = mmap(NULL, shm_pool_size,
    PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	struct wl_shm_pool *pool = wl_shm_create_pool(way.shm, fd, shm_pool_size);
	way.pool = pool;

	// make a buffer
	int index = 0;
	int offset = index*buffer_size;
	struct wl_buffer *buffer = wl_shm_pool_create_buffer(pool, offset,
    width, height, width*bytes_per_pixel, WL_SHM_FORMAT_ARGB8888);

	// GIve it a bunch of pixel data
	uint32_t *pixels = (uint32_t *)&pool_data[offset];
	memset(pixels, 0, buffer_size);
	
	for(int i = 0; i<buffer_size/4; i++)
	{
		int j = i; //- i%1024 ;
		int k = j - j%4 + 2;
		pixels[i]= (k*64) | (255 << 24);
	}

	wl_surface_attach(way.surface, buffer, 0, 0);
	wl_surface_damage(way.surface, 0, 0, UINT32_MAX, UINT32_MAX);
	wl_surface_commit(way.surface);
}