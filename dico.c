#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>   //(for "srand(time(NULL));")

#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

static int	myrand(void)
{
	int fd, ret;

	if ( (fd = open("/dev/urandom", O_RDONLY)) < 0 ) return -1;
	read(fd, &ret, sizeof(ret));
	close(fd);

	return ret;
}

int	load_dico(char *buf, int size, const char *fn)
{
	unsigned int	fpos, i;
	char		*r;
	FILE		*file;
	
	if ( ! (file = fopen(fn, "r")) ) {
		fputs("Dictionary not found !\n", stderr);
		return -1;
	}
	fgets(buf, size, file); // 1st line = Number of dictionary entries
	i = atoi(buf);
	
	if ( ! i ) {
		fputs("Error reading dictionary !\n", stderr);
		fclose(file);
		return -2;
	}
	
	srand(time(NULL));
	fpos = rand() % i;
	//fpos = myrand() % i;
	#ifdef TESTMODE
	printf("fpos[%u/%u] -> ", fpos, i);
	#endif

	buf[0] = i = 0;
	do {
		r = fgets(buf, size, file);
	}
	while ( r && i++ < fpos );

	fclose(file);

	if ( ! buf[0] || buf[0] == '\n' ) {
		fputs("Dictionary empty !\n", stderr);
		return -3;
	}
	buf[strlen(buf)-1] = 0; // Strip \n
	
	for ( i=0 ; buf[i] ; i++ ) buf[i] = toupper(buf[i]);
	
	#ifdef TESTMODE
	puts(buf);
	#endif
	
	return strlen(buf);
}
#ifdef TESTMODE
int	main(void)
{
	char buf[64];
	return load_dico(buf, sizeof(buf), "dico");
}
#endif
