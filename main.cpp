#include <iostream>
#include "stockData.h"
#include <zip.h>

using namespace std;

 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <limits.h>
#include <zip.h>
#include <vector>
const char *prg;
 
static void safe_create_dir(const char *dir)
{
    if (mkdir(dir, 0755) < 0) {
        if (errno != EEXIST) {
            perror(dir);
            exit(1);
        }
    }
}
void createDB(vector <stockData*> stocksList)
{
    int err =0 ;
    struct zip *newDB = zip_open("stockDB.zip",ZIP_CREATE,&err);

    for (int i = 0; i < stocksList.size(); i++)
    {
        string esp = stocksList[i]->getStockName() + ".esp";
        //int size_esp = esp.length();  
        char* espChar = &esp[0];
        zip_source* newDBsource = zip_source_file(newDB,espChar,0,0);
        zip_file_add(newDB,espChar,newDBsource,0);


        string stock = stocksList[i]->getStockName() + ".stock";
        // int size_stock = stock.length();
        char* stockChar = &stock[0];
        zip_source* newDBsource2 = zip_source_file(newDB,stockChar,0,0);
        zip_file_add(newDB,stockChar,newDBsource,0);
    }    
    zip_close(newDB);
}
void extractZIP(){
/// open db
    int err =0 ;
    const char *archive;
    struct zip *za;
    struct zip_file *zf;
    struct zip_stat sb;
    char buf[100];
 
    int i, len;
    int fd;
    long long sum;
    prg = "DB.zip";
    archive = "DB.zip";
    if ((za = zip_open(archive, 0, &err)) == NULL) { 
        zip_error_to_str(buf, sizeof(buf), err, errno);
        fprintf(stderr, "%s: can't open zip archive `%s': %s/n", prg,
            archive, buf);
    }


    cout << "zip_get_num_entries(za, 0) "<<zip_get_num_entries(za, 0)<<endl; 


    for (i = 0; i < zip_get_num_entries(za, 0); i++) {
        if (zip_stat_index(za, i, 0, &sb) == 0) {
            len = strlen(sb.name);
            printf("Name: [%s], ", sb.name);
            printf("Size: [%llu], ", sb.size);
            printf("mtime: [%u]/n", (unsigned int)sb.mtime);
            if (sb.name[len - 1] == '/') {
                safe_create_dir("sb.name"+i);
                cout<<"create dir"<<endl;
            } else {
                zf = zip_fopen_index(za, i, 0);
                if (!zf) {
                    fprintf(stderr, "boese, boese");
                                    cout<<"\n";
                    exit(100);
                }
 
                fd = open(sb.name, O_RDWR | O_TRUNC | O_CREAT, 0644);
                if (fd < 0) {
                    fprintf(stderr, "boese, boese");
                                    cout<<"\n";
                    exit(101);
                }
 
                sum = 0;
                while (sum != sb.size) {
                    len = zip_fread(zf, buf, 100);
                    if (len < 0) {
                        fprintf(stderr, "boese, boese");
                                        cout<<"\n";
                        exit(102);
                    }
                    write(fd, buf, len);
                    sum += len;
                }
                close(fd);
                zip_fclose(zf);
                //cout<<"-------\n";
                //cout<<buf;
                ofstream outfile(sb.name);
                outfile << buf;
                outfile.close();
                //cout<<"-------\n";

            }
        } else {
            printf("File[%s] Line[%d]/n", __FILE__, __LINE__);
        }
    }   
 
    if (zip_close(za) == -1) {
        fprintf(stderr, "%s: can't close zip archive `%s'/n", prg, archive);
    }
}
 
int main(int argc, char *argv[])
{
    extractZIP();
    return 1;
}