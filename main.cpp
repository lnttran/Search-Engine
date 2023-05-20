/**
 * I would like to thank my TAs who helped me with this project. 
 * Michael Lennon 
 * Adam Escobedo
 * Simmin Wang
 * 
 * Thank you very much,
 * Tran Lam
 */

#include <iostream>
#include "AVLtree.h"
#include "queryPro.h"
#include <chrono>
#include <string>
#include <unordered_map>

using namespace std;
using namespace std::chrono;

int main()
{
    DocumentParser doc;
    queryPro query;
    char answer;
    do {
        string input;
        int choice;
        double indexTime, queryTime, writeIndTime , readIndTime;
        query.menu();
        cin >> choice;
        switch(choice){
            case 1:
            {
                //pass the filename here to parse and pre-process the engine
                auto start = steady_clock::now();
                doc.testFileSystem("/users7/cse/tnlam/assignment-4-search-engine-tran-lam-jordan-young/test_data");
                auto stop = steady_clock::now();
                indexTime = duration_cast<milliseconds>(stop - start).count()/1000.0;
                cout << "************************************************************************************************" << endl;
                cout << "                                    Index is created." << endl;
                cout << "************************************************************************************************" << endl;
                break;
            }
            case 2:
            {
                //write index from file
                auto start = steady_clock::now();
                doc.TextIndexToFile("textIndex.csv");
                doc.OrgIndexToFile("orgIndex.csv");
                doc.PersonIndexToFile("personIndex.csv");
                auto stop = steady_clock::now();
                writeIndTime = duration_cast<milliseconds>(stop - start).count()/1000.0;
                cout << "************************************************************************************************" << endl;
                cout << "                              Index is written to the file." << endl;
                cout << "************************************************************************************************" << endl;
                break;
            }
            case 3:
            {
                //read index from file
                auto start = steady_clock::now();
                doc.fileToOrgTree("orgIndex.csv");
                doc.fileToPersonTree("personIndex.csv");
                doc.fileToTextTree("textIndex.csv");
                auto stop = steady_clock::now();
                readIndTime = duration_cast<milliseconds>(stop - start).count()/1000.0;
                cout << "************************************************************************************************" << endl;
                cout << "                             Index is created from the file." << endl;
                cout << "************************************************************************************************" << endl;
                break;
            }
            case 4:
            {
                int number;
                //search query
                cout << "************************************************************************************************" << endl;
                cout << "                                      SEARCH QUERY" << endl;
                cout << "                       Please enter a query you would like to search." << endl;
                cout << "          OGR: for organization entity | PERSON: for person entity | - for exclusion." << endl;
                cout << "          EX: netword security PERSON:carolyn_julie_fairbairn ORG:facebook -profits." << endl;
                cout << "************************************************************************************************" << endl;
                cout << "Search > ";
                cin.ignore();
                getline(cin, input);
                auto start = steady_clock::now();
                query.output(input, doc);
                auto stop = steady_clock::now();
                queryTime = duration_cast<seconds>(stop - start).count()/1000.0;

                cout << "************************************************************************************************" << endl;
                cout << "              Select document number that you would like to view the text." << endl;
                cout << "                  1-15 for viewing corresponding document| 0 for exit." << endl;
                cout << "************************************************************************************************" << endl;
                cout << "Your choice > ";
                cin >> number;
                if( number > 0 && number <= 15) {
                    query.displayText(number, doc);
                }
                break;
            }
            case 5:
            {
                //print statistic
                cout << "************************************************************************************************" << endl;
                cout << "Number of file: " << doc.getCount() << endl;
                cout << "Number of texts indexed: " << doc.getTextIndexCount() << endl;
                cout << "Number of person indexed: " << doc.getPersonIndexCount() << endl;
                cout << "Number of organization indexed: " << doc.getOrgIndexCount() << endl;
                cout << "Time for indexing: " << fixed << setprecision(2) << indexTime << " seconds" << endl;
                cout << "Time for write index to file: " << fixed << setprecision(2) << writeIndTime << " seconds" << endl;
                cout << "Time for read index to tree: " << fixed << setprecision(2) << readIndTime  << " seconds" << endl;
                cout << "Time for queries : " << fixed <<  setprecision(2) << queryTime << " seconds" << endl;
                cout << "************************************************************************************************" << endl;
                break;
            }
            case 6:
                return 0;
                break;
            default:
                cout << "ERROR: INVALID INPUT." << endl;
        }
        cout << "************************************************************************************************" << endl;
        cout << "                            Would you like to go back to the menu?" << endl;
        cout << "                               Y for Yes | N for No" << endl;
        cout << "************************************************************************************************" << endl;
        cout << "Your choice > ";
        cin >> answer;
    }while (answer == 'Y' || answer == 'y');

    return 0;
}