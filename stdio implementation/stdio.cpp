#include <fcntl.h>     // open
#include <sys/types.h> // read
#include <sys/uio.h>   // read
#include <unistd.h>    // read, close
#include <strings.h>   // bcopy
#include <string.h>    // strlen
#include <stdarg.h>    // format, ...
#include <stdlib.h>    // abs

using namespace std;

char decimal[100];

int recursive_itoa(int arg) {
	int div = arg / 10;
	int mod = arg % 10;
	int index = 0;
	if (div > 0)
		index = recursive_itoa(div);
	decimal[index] = mod + '0';
	return ++index;
}

char *itoa(const int arg) {
	bzero(decimal, 100);
	int order = recursive_itoa(arg);
	char *new_decimal = new char[order + 1];
	bcopy(decimal, new_decimal, order + 1);
	return new_decimal;
}

int printf(const void *format, ...) {
	va_list list;
	va_start(list, format);

	char *msg = (char *)format;
	char buf[1024];
	int nWritten = 0;

	int i = 0, j = 0, k = 0;
	while (msg[i] != '\0') {
		if (msg[i] == '%' && msg[i + 1] == 'd') {
			buf[j] = '\0';
			nWritten += write(1, buf, j);
			j = 0;
			i += 2;

			int int_val = va_arg(list, int);
			char *dec = itoa(abs(int_val));
			if (int_val < 0)
				nWritten += write(1, "-", 1);
			nWritten += write(1, dec, strlen(dec));
			delete dec;
		}
		else
			buf[j++] = msg[i++];
	}
	if (j > 0)
		nWritten += write(1, buf, j);
	va_end(list);
}

int setvbuf(FILE *stream, char *buf, int mode, size_t size) {
	if (mode != _IONBF && mode != _IOLBF && mode != _IOFBF)
		return -1;
	stream->mode = mode;
	stream->pos = 0;

	if (stream->buffer != (char *)0 && stream->bufown == true)
		delete stream->buffer;

	switch (mode) {
	case _IONBF:
		stream->buffer = (char *)0;
		stream->size = 0;
		stream->bufown = false;
		break;
	case _IOLBF:
	case _IOFBF:
		if (buf != (char *)0) {
			stream->buffer = buf;
			stream->size = size;
			stream->bufown = false;
		}
		else {
			stream->buffer = new char[BUFSIZ];
			stream->size = BUFSIZ;
			stream->bufown = true;
		}
		break;
	}

	return 0;
}

void setbuf(FILE *stream, char *buf) {
	setvbuf(stream, buf, (buf != (char *)0) ? _IOFBF : _IONBF, BUFSIZ);
}

FILE *fopen(const char *path, const char *mode) {
	FILE *stream = new FILE();
	setvbuf(stream, (char *)0, _IOFBF, BUFSIZ);

	// fopen( ) mode
	// r or rb           =  O_RDONLY
	// w or wb           =  O_WRONLY | O_CREAT | O_TRUNC
	// a or ab           =  O_WRONLY | O_CREAT | O_APPEND
	// r+ or rb+ or r+b  =  O_RDWR
	// w+ or wb+ or w+b  =  O_RDWR   | O_CREAT | O_TRUNC
	// a+ or ab+ or a+b  =  O_RDWR   | O_CREAT | O_APPEND

	switch (mode[0]) {
	case 'r':
		if (mode[1] == '\0')            // r
			stream->flag = O_RDONLY;
		else if (mode[1] == 'b') {
			if (mode[2] == '\0')          // rb
				stream->flag = O_RDONLY;
			else if (mode[2] == '+')      // rb+
				stream->flag = O_RDWR;
		}
		else if (mode[1] == '+')        // r+  r+b
			stream->flag = O_RDWR;
		break;
	case 'w':
		if (mode[1] == '\0')            // w
			stream->flag = O_WRONLY | O_CREAT | O_TRUNC;
		else if (mode[1] == 'b') {
			if (mode[2] == '\0')          // wb
				stream->flag = O_WRONLY | O_CREAT | O_TRUNC;
			else if (mode[2] == '+')      // wb+
				stream->flag = O_RDWR | O_CREAT | O_TRUNC;
		}
		else if (mode[1] == '+')        // w+  w+b
			stream->flag = O_RDWR | O_CREAT | O_TRUNC;
		break;
	case 'a':
		if (mode[1] == '\0')            // a
			stream->flag = O_WRONLY | O_CREAT | O_APPEND;
		else if (mode[1] == 'b') {
			if (mode[2] == '\0')          // ab
				stream->flag = O_WRONLY | O_CREAT | O_APPEND;
			else if (mode[2] == '+')      // ab+
				stream->flag = O_RDWR | O_CREAT | O_APPEND;
		}
		else if (mode[1] == '+')        // a+  a+b
			stream->flag = O_RDWR | O_CREAT | O_APPEND;
		break;
	}

	mode_t open_mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

	if ((stream->fd = open(path, stream->flag, mode)) == -1) {
		delete stream;
		printf("fopen failed\n");
		stream = NULL;
	}

	return stream;
}

int fpurge(FILE *stream) {
	stream->pos=0;
	stream->actual_size=0;
	return 0;
}



size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream) {
	int total, count, readcount, buffersize;
	total=count=size*nmemb;
	char * userobj= (char*)ptr;
	stream->lastop='r';
	if(stream->eof) return 0;
	if(total==0) return 0;
	while(count!=0)
	{
		//if there are bytes avail in fread buffer
		if(stream->actual_size - stream->pos !=0)
		{
			//read whole buffer or some subset
			if(count > stream->actual_size-stream->pos)
			{
				readcount=stream->actual_size-stream->pos;
			}
			else
			{
				readcount=count;
			}
			memcpy(userobj,stream->buffer+stream->pos,readcount);
			count -= readcount;
			stream->pos += readcount;
			userobj += readcount;
		}
		//no bytes in buffer 
		//syscall a buffsize into user obj
		else if(count>=stream->size)
		{
			//library implementation uses mutliples of buffersize
			readcount=read(stream->fd,userobj,stream->size);
			if(readcount==0)
			{
				stream->eof = true;
				return(total-count)/size;
			}
			count-= readcount;
			userobj +=readcount;
			
		}
		//fill the buffer
		else
		{
			if(stream->pos==0||stream->pos==stream->size)
				readcount=read(stream->fd,stream->buffer,stream->size);
			else
				readcount=0;
			if(readcount==0)
			{
				stream->eof = true;
				return(total-count)/size;
			}
			stream->actual_size=readcount;
			stream->pos=0;
		}
	}
	return nmemb;
}

int fflush(FILE *stream) {
	int result;
	//if the last operation was writing and there is stuff in the buffer
	if(stream->lastop=='w' && stream->actual_size-stream->pos!=0)
	{
		result=write(stream->fd,stream->buffer,stream->pos);
		stream->pos=0;
		stream->actual_size=0;
		if(result==0)
		{
			stream->eof=true;
			return EOF;
		}
	}
	else
	{
		stream->pos=0;
		stream->actual_size=0;
	}
	return 0;
}

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) {
	int total, count, writecount, buffersize;
	total=count=size*nmemb;
	char * userobj= (char*)ptr;
	stream->lastop='w';
	if(total==0) return 0;
	if(stream->eof) return 0;
	while(count !=0)
	{
		//if there is room in the buffer put it or some subset in buffer
		if(stream->pos < stream->size)
		{
			if(count > stream->size-stream->pos)
			{
				writecount= stream->size-stream->pos;
			}
			else
			{	
				writecount= count;
			}
			memcpy(stream->buffer+stream->pos,userobj,writecount);
			count-=writecount;
			stream->pos+=writecount;
			userobj+=writecount;
		}	
		// else if what we want to write won't fit in out buffer size
		//flush then write directly to file
		else if(count>=stream->size)
		{
			if(fflush(stream)==EOF)
			{
				return(total-count)/size;
			}
			writecount=write(stream->fd,userobj,stream->size);
			if(writecount==0)
			{
				stream->eof=true;
				return(total-count)/size;
			}
			count-=writecount;
			userobj +=writecount;
		}
		//else flush the buffer
		else
		{	
			if(fflush(stream)==EOF)
			{
				return(total-count)/size;
			}
		}
	}
	return nmemb;
}

int fgetc(FILE *stream) {
	char temp;
	if (fread(&temp,sizeof(char),1,stream)==0)
	{
		return EOF;
	}
	else
	{
		return temp;
	}
}

int fputc(int c, FILE *stream) {
	char temp=c;
	if(fwrite(&temp,sizeof(char),1,stream)==0)
		return EOF;
	else
		return c;
}

char *fgets(char *str, int size, FILE *stream) {
	int character, readin=0;
	while(readin<size)
	{	
		character=fgetc(stream);
		if(character==EOF)
		{
			if(readin==0)
				return NULL;
			else
				break;
		}
		*(str+readin)=character;
		readin++;
		if(character=='\n')
			break;
	}
	*(str+readin) = '\0';
	return str;
}

int fputs(const char *str, FILE *stream) {
	if(fwrite(str,strlen(str),1,stream)==1)
		return 0;
	else
		return EOF;
}

int feof(FILE *stream) {
	return stream->eof == true;
}

int fseek(FILE *stream, long offset, int whence) {
	
	if (whence != SEEK_SET && whence != SEEK_END && whence!=SEEK_CUR)
		return -1;
	stream->eof=false;
	fflush(stream);
	//todo flag clearing
	int rc=lseek(stream->fd,offset,whence);
	if(rc == -1)
		return -1;
	else
		return 0;
}

int fclose(FILE *stream) {
	fflush(stream);
	int rc=close(stream->fd);
	return rc;
}
