#include <fcntl.h>     // open
#include <unistd.h>    // read
#include <sys/types.h> // read
#include <sys/uio.h>   // read
#include <stdio.h>     // fopen, fread
#include <sys/time.h>  // gettimeofday
#include <iostream>    // cout, cerr, endl;

using namespace std;

struct timeval starting, ending;

void startTimer() {
	gettimeofday(&starting, NULL);
}

void stopTimer(const char *str) {
	gettimeofday(&ending, NULL);
	cout << str << "'s elapsed time\t= "
		<< ((ending.tv_sec - starting.tv_sec) * 1000000 + (ending.tv_usec - starting.tv_usec))
		<< endl;
}

int main(int argc, char *argv[]) {
	// validate arguments
	if (argc != 3) {
		cerr << "usage: lab3 filename bytes" << endl;
		return -1;
	}
	int bytes = atoi(argv[2]);
	if (bytes < 1) {
		cerr << "usage: lab3 filename bytes" << endl;
		cerr << "where bytes > 0" << endl;
		return -1;
	}
	char *filename = argv[1];
	char *buf = new char[bytes];

	// unix i/o
	int fd = open(filename, O_RDONLY);
	if (fd == -1) {
		cerr << filename << " not found" << endl;
		return -1;
	}
	startTimer();
	while (read(fd, buf, bytes) > 0);
	stopTimer("Unix read");
	close(fd);

	// standard i/o

	FILE* file = fopen(filename,"r");
	
	//setvbuf(file, NULL, _IONBF, 0);
	if(bytes==1)
	{
		startTimer();
		while(fgetc(file)!=EOF);
		stopTimer("Standard fgetc");
	}
	else
	{
		
		startTimer();
		while(fread(buf,bytes,1,file)==1);
		stopTimer("Standard fread");
	}
	
	fclose(file);

	return 0;
}
