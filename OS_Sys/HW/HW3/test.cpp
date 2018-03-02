#include <fstream>
#include <string>
#include <iostream>

using namespace std;

int main()
{
    std::string str = "";

    ofstream oFile;
    oFile.open("test.txt");

    if (!oFile)
    {
        printf("Unable to open file");
        exit(1);
    }

    printf("File opened.\n");
    printf("Please enter what you want to be inserted:\n");

    do {
        cin >> str;
        oFile << str;
    } while (str != "STOP!!!");

    oFile.close();
    printf("File closed.\n");

    return 0;
}