#ifndef DocParser_h
#define DocParser_h

#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"
#include "porter2_stemmer.h"
#include <iostream>
#include <filesystem>
#include <vector>
#include <fstream>


using namespace std;

//Lines 4 - () written by Jordan Young
//started at 10:00 am April 5 2023

/* Class DocParser will responsible for converting 
 * documents into appropriate terms and pushing
 * to an appropriately formatted document for persistence
 */

//Considering using open source library jsoncpp to aid in doc parsing @Jordan Young - 10:15 am April 5
//Nope, using rapidjson as provided by project files @ Jordan Young - 10:24 am
class DocParser 
{
    private:

    public:

    void OpenReadDir(const string &dirName)
    {
        auto it = std::filesystem::recursive_directory_iterator(dirName);

        // loop over all the files in the directory
        for (const auto &entry : it)
        {

            if (entry.is_regular_file() && entry.path().extension().string() == ".json") //check if it is a json file
            {
                ReadFile(entry.path().string());
            }
        }
    }

    void ReadFile(const string &fileName)
    {
        ifstream file;
        file.open(fileName);

        if (!file.is_open())
        {
            cerr << "file: " << fileName << " cannot be opened." << endl;
            return;
        }

        rapidjson::IStreamWrapper wrapper(file);

        rapidjson::Document doc;
        doc.ParseStream(wrapper);

        auto docTitle = doc["title"].GetString(); //gets title from the json doc

        auto persons = doc["entities"]["persons"].GetArray(); //get all the names of people

        //need to implement at this point, pushing the info to the map itself
        //might also be worthwhile to do persistence concurrently either here or in OpenReadDoc()

        auto orgs = doc["entities"]["organizations"].GetArray(); //similar to persons line
        
        /**
         * @brief Need to read the uuid 
         * 
         */



        vector<string> docText = split(doc["text"].GetString());
        // AVLtree <

        //here we need to stem, remove stop word, and then organize non person and orgs terms
        //working out stop word removal algorithm
        for (string word : docText) { //consider consolidating this into split()

        }

    }


    vector<string> split(const string &orig) //utility function to split string to vector based on given char separator
    { //send all to lowercase
        vector<string> splitUp;
        int count = 0;
        int index = 0;
        for (char c : orig) {

            if ((c >= 65 && c<= 90) || (c >= 97 && c <= 122)) { //ASCII values of letters, both lower and uppercase
                count++;
            }
            else if (count != 0){ //case so that multiple non letter characters do not add empty string onto vector

                string word = orig.substr((index - count), count);

                Porter2Stemmer::trim(word);
                Porter2Stemmer::stem(word);

                splitUp.push_back(word); //.substr(start, length of substring), this is a part of the standard string library, it creates a substring of a string starting at the start value and is the given length
                count = 0; //this restarts the count for the length of the next word
            }
            index++; 
        }
        return splitUp; //returns the completed vector
    }

};

#endif