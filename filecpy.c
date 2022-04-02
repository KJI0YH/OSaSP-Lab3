#define failure(str) {perror(str); exit(-1);}
#define BUFF_LEN 4096
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	if (argc < 3)
		failure("Invalid arguments\nFormat: filecpy.run [srcFileName] [destFileName]");
		
	if (strcmp(argv[1], argv[2]) == 0)
		failure("Files can not have the same name");
		
	FILE *fpsrc, *fpdest;
	
	//open files
	if ((fpsrc = fopen(argv[1], "rb")) == NULL)
		failure("Can not open srcFileName\n");
	if ((fpdest = fopen(argv[2], "w+b")) == NULL)
		failure("Can not open destFileName");
		

	//copying a file 
	char buffer[BUFF_LEN];
	int bytes = 0, count = 0;
	while ((bytes = fread(&buffer, 1, BUFF_LEN, fpsrc)) != 0) {
		fwrite(&buffer, 1, bytes, fpdest);
		count += bytes;
	}
	
	//copying a file mode
	struct stat statbuf;
	if (stat(argv[1], &statbuf) == -1)
		perror("Can not get stat of the srcFileName");
	else if (chmod(argv[2], statbuf.st_mode) == -1)
		perror("Can not change mode of the destFileName"); 
		
	printf("PID: %d Bytes copied: %d File: %s \n", getpid(), count, argv[1]);
	
	//close files
	if(fclose(fpsrc))
		perror("Can not close srcFileName");
	if(fclose(fpdest))
		perror("Can not clise destFileName");
	
	return 0;
	
}
