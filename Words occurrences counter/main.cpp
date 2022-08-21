#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <algorithm>
#define minimalinput 5 //program name + input file path + output file path + sorting way + at least 1 keyword

using namespace std;

void checkcorrectinput (int realinput, int exceptedinput)
{
    if (realinput < exceptedinput)
    {
        cout << "\n";
        cerr << "There must be at least " << exceptedinput << " arguments!" << endl;
        exit(0);
    }
}

ifstream openinputfile (char *inputfilepath)
{
    ifstream inputfile;
    inputfile.open(inputfilepath);
    if (!inputfile)
    {
        cout << "\n";
        cerr << "Unable to open the input file" << endl;
        exit(0);
    }
    cout << "\n" << "Successfully opened input file" << endl;
    return inputfile;
}

void fillinitialpair (int counter, pair <int, string> *inputpair, char *argument[])
{
    for (int i = 0; i <= (counter - minimalinput); i++)
    {
        inputpair[i].second = argument[(i + (minimalinput - 1))];
        inputpair[i].first = 0;
    };
}

void searchforkeyword (ifstream &inputfile, int counter, pair <int, string> *inputpair)
{
    char character;
    string word;
    while (inputfile.get(character))
    {
        if (character == '.' || character == ',' || character == ';' || character == '?' || character == '!' || character == ':' || character == '"' || character == '(' || character == ')' || character == '̶̶' || character == '/' ||
                character == '<' || character == '>' || character == '\\' || character == '|' || character == '[' || character == ']' || character == '{' || character == '}' || character == '\n');
        else if (character == ' ')
        {
            transform (word.begin(), word.end(), word.begin(), ::tolower);
            for (int i = 0; i <= (counter - minimalinput); i++)
            {
                size_t occurrence = 0;
                string lowercasekeyword = inputpair[i].second;
                transform (lowercasekeyword.begin(), lowercasekeyword.end(), lowercasekeyword.begin(), ::tolower);
                occurrence = word.find(lowercasekeyword, occurrence);
                if (occurrence != string::npos)
                {
                    inputpair[i].first++;
                }
            }
            word.clear();
        }
        else
        {
            word += character;
        }
    }
    inputfile.close();
}

void sorting (char *argument, pair <int, string> *inputpair, int counter)
{
    string sortingway = argument;
    transform (sortingway.begin(), sortingway.end(), sortingway.begin(), ::tolower);
    if (sortingway != "ascending" && sortingway != "descending")
    {
        cout << "\n";
        cerr << "Incorrect sorting argument!" << endl;
        exit(0);
    }
    else if (sortingway == "ascending")
    {
        sort (inputpair, (inputpair + (1 + (counter - minimalinput))));
    }
    else
    {
        sort (inputpair, (inputpair + (1 + (counter - minimalinput))));
        reverse (inputpair, (inputpair + (1 + (counter - minimalinput))));
    }
}

ofstream createdoutputfile (char *outputfilepath)
{
    ofstream outputfile;
    outputfile.open(outputfilepath);
    if (!outputfile)
    {
        cout << "\n";
        cerr << "Unable to create the output file" << endl;
        exit(0);
    }
    return outputfile;
}

void creatreport (int counter, ofstream &outputfile, pair <int, string> *inputpair)
{
    outputfile << "Keywords appearances" << endl;
    for (int i = 0; i <= (counter - minimalinput); i++)
    {
        outputfile << (i + 1) << ". " << inputpair[i].second << " " << inputpair[i].first << " times" << endl;
    }
    cout << "\n" << "Report successfully created" << "\n" << endl;
    outputfile.close();
}

int main(int argc, char *argv[])
{
    checkcorrectinput(argc, minimalinput);

    ifstream textfile = openinputfile(argv[1]);

    pair <int, string> *keyword = new pair <int, string> [(1 + (argc - minimalinput))]; //a pair containing the number of occurrences (first) of a keyword (second)

    fillinitialpair (argc, keyword, argv);

    searchforkeyword (textfile, argc, keyword);

    sorting (argv[3], keyword, argc);

    ofstream reportfile = createdoutputfile(argv[2]);

    creatreport (argc, reportfile, keyword);

    delete [] keyword;

    return 0;
}
