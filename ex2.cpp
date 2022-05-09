//pip example

#include <iostream>
#include <fcntl.h>
#include <errno.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <signal.h>
#include <syslog.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
using namespace std;

const char* DB_NAME = "stocks_db.zip";
int parentPid= 0;
int workerPid= 0;
int READ = 0;
int WRITE = 1;

// We use two pipes
// First pipe to send input from parent to the worker
// Second pipe to send stack information from worker to parent
int fd1[2]; // Used to store two ends of first pipe
int fd2[2]; // Used to store two ends of second pipe

// function declartion
void sigint_handler(int signum);
void printMenu();
string userInput();
const char* fetch_stock_data();
const char* list_fetched_stocks();
const char* print_stocks_data();
const char* save_all_stocks_data();
void worker();
void parent();
void loadDB();
void creatDB();
void cleanup();


// function imp
int main()
{
	loadDB();
	signal(SIGINT, sigint_handler);
	pid_t p;

// ----- pipe -----
	if (pipe(fd1) == -1) {
        printf("Pipe Failed: %s", stderr);
        return 1;
	}
	if (pipe(fd2) == -1) {
		fprintf(stderr, "Pipe Failed");
		return 1;
	}
// ----- fork -----
	p = fork();

	if (p < 0) {
		fprintf(stderr, "fork Failed");
		return 1;
	}
// ----- parent -----
	else if (p > 0) {
		parent();
	}
	// child process
	else {
		worker();
	}
	return 0;
	
}

void worker(){
	close(fd1[WRITE]); // Close writing end of first pipe
	close(fd2[READ]);
	while (true)
	{
		workerPid = getpid();
		int readSize = 256;
		char stringFromWorker[readSize];
		char workerResponse[readSize];
		read(fd1[READ], stringFromWorker, readSize);
		int choce = stoi(stringFromWorker);

		string mission;
		switch (choce) // do worker mission
		{
		case 1:
			strcpy(workerResponse,fetch_stock_data());
			break;
		case 2:
			strcpy(workerResponse,list_fetched_stocks());
			break;		
		case 3:
			strcpy(workerResponse,print_stocks_data());
			break;
		case 4:
			strcpy(workerResponse,save_all_stocks_data());
			break;						
		default:
			break;
		}
		// Write stocks information
		write(fd2[WRITE], workerResponse, readSize);
	}
}
void parent(){
	parentPid = getpid();
		close(fd1[READ]); // Close reading end of first pipe
		close(fd2[WRITE]); // Close writing end of second pipe

		while (true)
		{		
			printMenu();
			string input = userInput();
			char input_str[100];
			strcpy(input_str, input.c_str());
			int readSize = 256;
			char stringFromWorker[readSize];

			// Write input string and close writing end of first
			write(fd1[WRITE], input_str, strlen(input_str) + 1);

			// Read string from child, print it
			read(fd2[READ], stringFromWorker, readSize);
			cout << stringFromWorker << endl;
		}
}

void printMenu(){ 
	std::cout << "1 - Fetch stock data" << endl;
	std::cout << "2 - List fetched stocks" << endl;
	std::cout << "3 - print stocks data" << endl;
	std::cout << "4 - Save all stocks data" << endl;
	std::cout << "Make your choce (1/2/3/4):" << endl;
}
string userInput(){ 
	// get input and check if it is valid input
	string input_str;
	int input = 0;
	do	
	{
		std::cin >> input_str;
		input = stoi(input_str);
		if (input < 1 || input > 4)
		{
			std::cout << "Invalid Input, must be a value between 1 - 4 " << endl;
			std::cout << "Please try again " << endl;
		}		
	} while (input < 1 || input > 4);
	return input_str;
}
void sigint_handler(int signum)
{
	if (getpid() == workerPid) // do mission 4 and exit
	{
		save_all_stocks_data();
		//cout << save_all_stocks_data() << endl;
	}	
	cleanup();
	exit(1);
}

const char* fetch_stock_data(){
	return "mission 1";
}
const char* list_fetched_stocks(){
	return "mission 2";
}
const char* print_stocks_data(){
	return "mission 3";
}
const char* save_all_stocks_data(){
	creatDB();
	return "mission 4";
}
void loadDB (){
	FILE *file;
    	if ((file = fopen(DB_NAME, "r")))
    	{
			// EXTRACT THE FILE FROM THE ZIP FILE AND LOAD TO THE SYSTEM !!!!!
        	fclose(file);
    	}
}
void creatDB (){
	// CREAT ZIP FILE
}

void cleanup()
{
	close(fd1[READ]);
	close(fd1[WRITE]);

	close(fd2[READ]);
	close(fd2[WRITE]);
}