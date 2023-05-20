/**
 * Example code related to final project
 */
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <unordered_map>
#include <cctype>



// RapidJSON headers we need for our parsing.
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"
//#include "OleanderStemmingLibrary-master/include/olestem/stemming/english_stem.h"
#include "AVLtree.h"
#include "queryPro.h"
#include "DocumentParser.h"

using namespace rapidjson;
using namespace std;

// Function Prototypes
// void testReadJsonFile(const string &fileName, AvlTree <string, unordered_map<string, int> > &person, AvlTree <string, unordered_map<string, int> > &org, AvlTree <string, unordered_map<string, int> >&term);
// void testFileSystem(const string &path, AvlTree <string, unordered_map<string, int> > &person, AvlTree <string, unordered_map<string, int> > &org, AvlTree <string, unordered_map<string, int> >&term);
// void KeytoTree ( string key, string uuid,  AvlTree<string, unordered_map <string, int> > &tree); 
// int encounterStopWords(string target); 

int main()
{

    // AvlTree <string, unordered_map<string, int> > textAVLtree, personAVLtree, organizationAVLTree;

    // cout << "-------------------------------------------" << endl;
    // cout << "------ RapidJSON Doc Parsing Example ------" << endl;
    // cout << "-------------------------------------------" << endl;
    // testReadJsonFile("sample_data/coll_1/news_0064567.json", personAVLtree, organizationAVLTree, textAVLtree);
    // testReadJsonFile("sample_data/coll_1/news_0064568.json", personAVLtree, organizationAVLTree, textAVLtree);
    // testReadJsonFile("sample_data/coll_1/news_0064569.json", personAVLtree, organizationAVLTree, textAVLtree);
    // testReadJsonFile("sample_data/coll_2/news_0064570.json", personAVLtree, organizationAVLTree, textAVLtree);

    // cout << "\n";
    // cout << "-------------------------------------------" << endl;
    // cout << "------     File System Example       ------" << endl;
    // cout << "-------------------------------------------" << endl;
    // testFileSystem("sample_data",personAVLtree, organizationAVLTree, textAVLtree);

    // textAVLtree.displayInfo(); 
    // personAVLtree.displayInfo(); 
    // organizationAVLTree.displayInfo(); 


    return 0;
}

// /**
//  * example code that reads and parses a json file and extracts the title and person
//  * entities.
//  * @param fileName filename with relative or absolute path included.
//  */
// void testReadJsonFile(const string &fileName, AvlTree <string, unordered_map<string, int> > &person, AvlTree <string, unordered_map<string, int> > &org, AvlTree <string, unordered_map<string, int> >&term)
// {

//     // open an ifstream on the file of interest and check that it could be opened.
//     ifstream input(fileName);
//     if (!input.is_open())
//     {
//         cerr << "cannot open file: " << fileName << endl;
//         return;
//     }

//     // Create a RapidJSON IStreamWrapper using the file input stream above.
//     IStreamWrapper isw(input);

//     // Create a RapidJSON Document object and use it to parse the IStreamWrapper object above.
//     Document d;
//     d.ParseStream(isw);

//     // Now that the document is parsed, we can access different elements the JSON using
//     // familiar subscript notation.

//     // This accesses the -title- element in the JSON. Since the value
//     //  associated with title is a string (rather than
//     //  an array or something else), we call the GetString()
//     //  function to return the actual title of the article
//     //  as a c-string.
//     // auto val = d["title"].GetString();
//     // cout << "Title: " << val << "\n";

//     // auto text = d["text"].GetString();
//     // auto uuid = d["uuid"].GetString();

//     //space complexity
//     //vector <pair<string, int> >

//     //iterator 
//     //using for loop to print out the unordered_map 
//     auto val = d["title"].GetString();
//     auto persons = d["entities"]["persons"].GetArray();
//     auto organization = d["entities"]["organizations"].GetArray();
//     auto text = d["text"].GetString();
//     auto uuid = d["uuid"].GetString();

//     //insert the text to text AVLtree
//     KeytoTree(text, uuid, term);
    
//     //insert the person to person AVLtree
//     for (auto &p : persons){ 
//         KeytoTree(p["name"].GetString(), uuid, person);
//     }

//     //insert the organization to organization AVLtree
//     for (auto &o : organization){
//         KeytoTree(o["name"].GetString(), uuid, org); 
//     }
         

//     // stringstream ss(text); 
//     // string word_in_text; 
//     // while ( ss >> word_in_text) {
//     //     stemming::english_stem<> StemEnglish;
//     //     StemEnglish(reinterpret_cast<basic_string<wchar_t> &>(word_in_text));
//     //     cout << word_in_text << endl;
//     // }

//     // The Persons entity for which you're building a specific
//     //  inverted index is contained in top level -entities- element.
//     //  So that's why we subscript with ["entities"]["persons"].
//     //  The value associated with entities>persons is an array.
//     //  So we call GetArray() to get an iterable collection of elements
//     // auto persons = d["entities"]["persons"].GetArray();
//     // auto uuid = d["uuid"].GetString();
//     // We iterate over the Array returned from the line above.
//     //  Each element kind of operates like a little JSON document
//     //  object in that you can use the same subscript notation
//     //  to access particular values.
//     //  cout << "  Person Entities + sentiment:"
//     //       << "\n";
//     //  for (auto &p : persons)
//     //  {
//     //      cout << "    > " << setw(30) << left << p["name"].GetString()
//     //           << setw(10) << left << p["sentiment"].GetString() << "\n";
//     //  }
//     // cout << uuid << endl;
//     cout << endl;

//     input.close();
// }

// void KeytoTree (string key, string uuid,  AvlTree<string, unordered_map <string, int> > &tree) {
//     stringstream ss(key); 
//     string word_in_text; 

//     while ( ss >> word_in_text){
//         // Check if word contains any digits
//         bool has_non_alpha_num = false;
//         for (char c : word_in_text) {
//             //of one of the character of the word is non-alphabetic word, continue to the next word
//             if (!isalpha(c)) {
//                 has_non_alpha_num = true;
//                 break;
//             }
//         }
//         // Skip over word if it contains a digit
//         if (has_non_alpha_num)
//             continue;
        
//         //stemming word 
//         // stemming::english_stem<> StemEnglish;
//         // StemEnglish(reinterpret_cast<basic_string<wchar_t> &>(word_in_text));

//         /**
//          * Removing stopword
//          * If the word_in_text is a stopword, continue
//          */
//         int SWstatus = encounterStopWords(word_in_text);
//         if (SWstatus == -1) {
//             return; 
//         }

//         //if the word_in_test found in stopword list, then continue. 
//         if (SWstatus == 1)
//             continue;

//         //if the word_in_test is not found in the stopword list, then insert it in AVLTree
//         if (SWstatus == 0){

//             for ( char &c : word_in_text){
//                 c = tolower(c); 
//             }

//             //If the word_in_text is found in the tree
//             if (tree.find(word_in_text) == true){
//                 unordered_map<string, int>* temp_map = &(tree.findNode(word_in_text)->value);
//                 //if the the uuid is not found in map
//                 if (temp_map->find(uuid) == temp_map->end()){
//                     //insert the new uuid into the map
//                     (*temp_map)[uuid] = 1; 
//                 }
//                 //if the uuid is found in map
//                 else {
//                     //update the frequency of the word that appear in the uuid
//                     (*temp_map)[uuid] += 1; 
//                 }
//             }
//             //if the word is not found in the tree, insert a new word and uuid in the tree
//             else {
//                 unordered_map<string, int> temp_map; 
//                 temp_map[uuid] = 1;
//                 tree.insert(word_in_text, temp_map); 
//             }
//         }  
        
//     }
// }

// int encounterStopWords(string target)  {
//     //open stopword.txt
//     ifstream stopword_file ("/Users/tranlam/Documents/GitHub/assignment-4-search-engine-tran-lam-jordan-young/stopwords.txt"); 

//     //check if file exists
//     if (!stopword_file.is_open()){
//         cerr << "Error in opening stopwords file." << endl;
//         return -1;  
//     }
//     vector<string> stopwords; 
//     string word; 
//     while (stopword_file >> word){
//         stopwords.push_back(word);
//     }

//     if (binary_search(stopwords.begin(), stopwords.end(), target))
//         return 1; // return 1 if found
//     else  
//         return 0; // return 0 if not found
// }

// /**
//  * example code for how to traverse the filesystem using std::filesystem
//  * which is new for C++17.
//  *
//  * @param path an absolute or relative path to a folder containing files
//  * you want to parse.
//  */
// void testFileSystem(const string &path, AvlTree <string, unordered_map<string, int> > &person, AvlTree <string, unordered_map<string, int> > &org, AvlTree <string, unordered_map<string, int> >&term)
// {

//     // recursive_director_iterator used to "access" folder at parameter -path-
//     // we are using the recursive iterator so it will go into subfolders.
//     // see: https://en.cppreference.com/w/cpp/filesystem/recursive_directory_iterator
//     auto it = filesystem::recursive_directory_iterator(path);

//     // loop over all the entries.
//     for (const auto &entry : it)
//     {

//         cout << "--- " << setw(60) << left << entry.path().c_str() << " ---" << endl;

//         // We only want to attempt to parse files that end with .json...
//         if (entry.is_regular_file() && entry.path().extension().string() == ".json")
//         {
//             testReadJsonFile(entry.path().string(), person, org, term);
//         }
//     }
// }
