//
// Created by barsela on 5/13/22.
//
#include "../include/Mission.h"

string Mission::PrintStockData(string name, int year ){
    stockData s(name);
    return s.printStockByYear(year);
}

string Mission::list_fetched_stocks() {
    set <string> stocksSet = getSetOfAllTheFilesInDir(); /// get all the name files with .csv
    set <string>::iterator itr;
    string result;
    /// convert the vector to string
    for (itr = stocksSet.begin(); itr != stocksSet.end(); itr++) {
        result += *itr +" ";
   }
    return result;
}

string Mission::exportAndCreateDBStocksData(set <string> availableStockList) {
    string result = "export and create DB for StocksData: \n";
    if(availableStockList.size() == 0) /// there isn't stock in the system and the data base
        return "zero stock";
    vector <stockData> stockVector;
    set <string>::iterator itr;
    for (itr = availableStockList.begin(); itr != availableStockList.end(); itr++) {
        result += *itr +"\n";
    }

    ZipUtilities createZIP;
    createZIP.createDB(availableStockList);
    return result;
}

set <string>  Mission::getSetOfAllTheFilesInDir(){ /// get all the files with .csv suffix
    set <string> stocksNameSet;
    struct dirent *d;
    DIR *dr;
    dr = opendir(".");
    string fullName;
    string name;
    if(dr!=NULL)
    {
        while((d=readdir(dr))!=NULL){
            string sufix(d->d_name);
            fullName = string(d->d_name);
            int size = sufix.length();
            if (sufix.find(".")!= std::string::npos) {
                sufix = sufix.substr(sufix.find("."),size);
                if (sufix==".csv")
                     name = fullName.substr(0,fullName.find("."));
                    if (name !="")
                        stocksNameSet.insert(name);
            }
        }
        closedir(dr);
    }
    return stocksNameSet;
}
set<string> Mission::removeDupWord(string str) /// convert string of stocks name to set of stocks name
{
    set<string> stocksSet;
    string word = "";
    for (char x : str)
    {
        if (x == ' ')
        {
            stocksSet.insert(word);
            word = "";
        }
        else {
            word = word + x;
        }
    }
    return stocksSet;
}