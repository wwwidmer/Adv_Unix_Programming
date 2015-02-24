#include <fcntl.h> /* for open() */
#include <sys/stat.h> /* for file access permission bits */
#include <stdio.h> /* for FILENAME_MAX */
#include <unistd.h> /* for pathconf() */
#include <string.h> /* for strcat() */
#include <stdlib.h> /* for malloc() */
#include <errno.h> /* for errno */

int getChar(int filedes);
off_t getBytePosition(int row, int filedes);
void putChar(int filedes, char c, off_t byte);

int main(int argc, char *argv[])
{
	off_t curr_byte;
	ssize_t num_bytes;
	char buf[200];
	size_t nbytes;
	int row=3;
	
	char *path_name="books.txt";
	//int fd=creat(path_name, S_IRUSR|S_IWUSR|S_IXUSR);
	int fd=open(path_name, O_CREAT|O_RDWR|O_APPEND);

	char c=getChar(fd);
	//off_t offset=lseek(filedes, byte, SEEK_SET);
	printf("Read character %c\n", c);
	curr_byte=getBytePosition(row, fd);
	printf("Byte number at row %d is %lld.\n", row, curr_byte);
	curr_byte=lseek(fd, curr_byte, SEEK_SET);

	curr_byte=lseek(fd, 0, SEEK_SET); /* 0 bytes from current offset to find current offset */
	printf("Cursor is at the %lldth byte.\n", curr_byte);

	num_bytes=read(fd, buf, 150);
	printf("Reading %zd bytes.\n", num_bytes);
	//printf("Reading:\n %s\n", buf);
	//num_bytes=write(1, buf, num_bytes);

	int num_rows=0;
	int rowbytes[100];
	for (int i=0; i<num_bytes; i++)
	{
		if (buf[i]=='\n')
		{
			printf("EOL at byte %d\n", i);
			printf("EOL\n");
			num_rows++;
		}
		else if (buf[i]==' ')
			printf("EOR at byte %d\n", i);
	}

	//int row=3;
	//off_t b=getBytePosition(row, fd);
	//printf("Byte number at row %d is %lld.\n", row, b);
	//putChar(fd, 'X', b);
	//num_bytes=write(fd, buf, num_bytes);
return 0;
}

/* 
 Write a C program that replaces or inserts a row into the books.txt file
 You can specify your own row number as the row that needs replacing. Books.txt contains
 Two fields:
 book title                  author field

 You should be able to replace the record at the row number specified with your own.

*/

//reads and returns a character from file
int getChar(int filedes)
{
	static char c;
	if (read(filedes, &c, 1)==1)
	{
		off_t offset=lseek(filedes, 0, SEEK_CUR);
		printf("Current offset %lld\n", offset);
		return c;
	}
	else
		return EOF;
}

//writes a character into a file at specified byte
void putChar(int filedes, char c, off_t byte)
{
	off_t offset=lseek(filedes, byte, SEEK_SET);
	write(filedes, &c, 1); //the write starts at the current offset
}

//returns the byte position where a given row starts
off_t getBytePosition(int row, int filedes)
{
        int byte=1, lines=1, currChr;
        do
        {
		printf("Byte # is %d\n", byte);
                currChr=getChar(filedes);
                if(currChr=='\n')
                        lines++;
        } 
	while (lines<row && currChr!=EOF && ++byte);
return byte;
}
