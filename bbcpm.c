#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

/* function to get the size of the file */
size_t filesz(char *filename) {
	int ret = 0;
	size_t size = 0;
	struct stat st;

	ret = stat(filename, &st);
	if (ret < 0) {
		fprintf(stderr, "Can't stat file!\n");
		return ret;
	}

	size = st.st_size;

	return size;
}

int cp_contents(char *file_in, char *file_out) {
	int fd_in = 0, fd_out = 0;
	size_t sz = 0;
	char *s_addr = NULL, *t_addr = NULL;

	fd_in = open(file_in, O_RDONLY);
	if (fd_in < 0) {
		fprintf(stderr, "Can't open input file!\n");
		return 1;
	}

	fd_out = open(file_out, (O_RDWR | O_CREAT), 0666);
	if (fd_out < 0) {
		fprintf(stderr, "Can't open output file!\n");
		return 1;
	}

	sz = filesz(file_in);
	if (sz < 0) {
		return 1;
	}

	/* memory mapping of the source file */
	s_addr = mmap(0, sz, PROT_READ, MAP_SHARED, fd_in, 0);

	/* memory mapping of the output file */
	t_addr = mmap(0, sz, (PROT_READ | PROT_WRITE), MAP_SHARED, fd_out, 0);

	/* need to make the output file the size of the
	 * input file before copying anything into it
	 */
	ftruncate(fd_out, sz);

	/* copy the memory in s_addr to t_addr */
	memcpy(t_addr, s_addr, sz);

	/* unmap the memory */
	munmap(s_addr, sz);
	munmap(t_addr, sz);

	return 0;
}

int main(int argc, char *argv[]) {
	int ret;

	if (argc != 3) {
		fprintf(stderr, "Usage: %s <source> <target>\n", argv[0]);
	}

	ret = cp_contents(argv[1], argv[2]);
	if (ret == 1) {
		fprintf(stderr, "Can't copy the contents!\n");
	}

	return 0;
}
