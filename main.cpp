
#include "include/main.h"

#include <iostream>
#include <string>
#include <filesystem>
#include <unistd.h>


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


void worker() {
    close(fd1[WRITE]); // Close writing end of first pipe
    close(fd2[READ]);
    while (true) {
        workerPid = getpid();
        int readSize = 2000;
        char stringFromWorker[readSize];
        char workerResponse[readSize];
        read(fd1[READ], stringFromWorker, readSize);
        string st(stringFromWorker);
        string choose_st = st.substr(0, st.find("-"));
        st = st.substr(st.find("-")+1,st.length());
        int choose = stoi(choose_st);
        Mission m;

        switch (choose) // do worker mission
        {
            case 1: {
                // fetch stock data
            }
            case 2: {
                string result = m.list_fetched_stocks();
                strcpy(workerResponse, &result[0]);
                break;
            }

            case 3: { /// print stock data
                string name;
                int year;
                name = st.substr(0, st.find("-"));
                st = st.substr(st.find("-")+1,st.length());
                year = stoi(st);
                string result = m.PrintStockData(name,year);
                strcpy(workerResponse, &result[0]);
                break;
            }

            case 4: { // create and save all csv file
                string result = "List of fetched stocks: \n";
                result +=m.exportAndCreateDBStocksData();
                strcpy(workerResponse, &result[0]);
                break;
            }
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
        int input = userInput();
        string st = to_string(input)+"-";
        switch (input) {
            case 1:
                //st += userInputForMission1();
                break;
            case 3:
                st += userInputWhichStockAndYear();
                break;
        }

        char input_str[100];
        strcpy(input_str, &st[0]);
        int readSize = 2000;
        char stringFromWorker[readSize];

        // Write input string and close writing end of first
        write(fd1[WRITE], input_str, strlen(input_str) + 1);

        // Read string from child, print it
        read(fd2[READ], stringFromWorker, readSize);
        cout << stringFromWorker << endl;
    }
}

void printMenu(){
    std::cout << "MENU:" << endl;
    std::cout << "1 - Fetch stock data" << endl;
    std::cout << "2 - List fetched stocks" << endl;
    std::cout << "3 - print stocks data" << endl;
    std::cout << "4 - Save all stocks data" << endl;
    std::cout << "Make your choce (1/2/3/4):" << endl;
}
int userInput(){
    // get input and check if it is valid input
    int input = 0;
    do
    {
        std::cin >> input;
        if (input < 1 || input > 4)
        {
            std::cout << "Invalid Input, must be a value between 1 - 4 " << endl;
            std::cout << "Please try again " << endl;
        }
    } while (input < 1 || input > 4);
    return input;
}
string userInputWhichStockAndYear() {
    string stockSymbol;
    string year;
    cout << "Enter one stock symbol and the year after"<<endl;
    cout << "Stock symbol: ";
    cin >> stockSymbol;
    cout << "Year: ";
    cin >> year;
    return stockSymbol+"-"+year;
}

void sigint_handler(int signum)
{
    Mission m;
    if (getpid() == workerPid) // do mission 4 and exit
    {
        m.exportAndCreateDBStocksData();
    }
    cleanup();
    exit(1);
}

void loadDB (){
    FILE *file;
    ZipUtilities DB;
    if ((file = fopen(DB.DB_NAME, "r"))) { // EXTRACT THE FILE FROM THE ZIP FILE AND LOAD TO THE SYSTEM !!!!!
        fclose(file);
        availableStockList = DB.extractZIP();
    }
    else{
		cout << "There isn't available DB"<<endl;
	}
}
void cleanup()
{
    close(fd1[READ]);
    close(fd1[WRITE]);

    close(fd2[READ]);
    close(fd2[WRITE]);
}
