#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <stdexcept>
#include <algorithm>
#include <cctype>

// RapidJSON headers we need for our parsing.
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"
#include "porter2_stemmer.h"
#include "AVLtree.h"

using namespace rapidjson;
using namespace std;

class DocumentParser
{
private:

    AvlTree<string, unordered_map <string, int> > personAVLtree; //store the words that are found in person entities
    AvlTree<string, unordered_map <string, int> > organizationAVLtree; //store the words that are found in organization entities
    AvlTree<string, unordered_map <string, int> > textAVLtree; //store the words that are found in text entities.
    size_t count = 0; //file count

public:

    AvlTree<string, unordered_map <string, int> > getPersonAVLtree () const {
        return personAVLtree;
    }

    AvlTree<string, unordered_map <string, int> > getTextAVLtree () const {
        return textAVLtree;
    }

    AvlTree<string, unordered_map <string, int> > getOrgAVLtree () const {
        return organizationAVLtree;
    }

    void TextIndexToFile (string filename) {
        writeIndexToFile(filename, textAVLtree);
    }

    void PersonIndexToFile (string filename) {
        writeIndexToFile(filename, personAVLtree);
    }

    void OrgIndexToFile (string filename) {
        writeIndexToFile(filename, organizationAVLtree);
    }

    void fileToTextTree(string filename) {
        readFiletoIndex(filename, textAVLtree);
    }

    void fileToPersonTree(string filename) {
        readFiletoIndex(filename, personAVLtree);
    }

    void fileToOrgTree(string filename) {
        readFiletoIndex(filename, organizationAVLtree);
    }

    size_t getCount() const {
        return count;
    }

    int getPersonIndexCount() const {
        return personAVLtree.numOfNode();
    }

    int getTextIndexCount() const {
        return textAVLtree.numOfNode();
    }

    int getOrgIndexCount() const {
        return organizationAVLtree.numOfNode();
    }

    string stemWord (string word) {
        Porter2Stemmer::trim(word);
        Porter2Stemmer::stem(word);

        return word;
    }


    /**
     * example code for how to traverse the filesystem using std::filesystem
     * which is new for C++17.
     *
     * @param path an absolute or relative path to a folder containing files
     * you want to parse.
     */
    void testFileSystem(const string &path)
    {
        count = 0;
        // recursive_director_iterator used to "access" folder at parameter -path-
        // we are using the recursive iterator so it will go into subfolders.
        // see: https://en.cppreference.com/w/cpp/filesystem/recursive_directory_iterator
        auto it = filesystem::recursive_directory_iterator(path);
        // loop over all the entries.
        for (const auto &entry : it)
        {
            count += 1;

            filesystem::path path = entry.path();
            // We only want to attempt to parse files that end with .json...
            if (entry.is_regular_file() && path.extension().string() == ".json")
            {
                ReadJsonFile(path.string());
            }

            // if ( count == 5000) {
            //     break;
            // }
        }
    }

    /**
     * @brief Get the File Information including tittle, author, and date published
     *
     * @param filename
     * @param c is iterator
     */
    void getFileInfo (const string &filename, int c) {
        // open an ifstream on the file of interest and check that it could be opened.
        ifstream input(filename);
        if (!input.is_open())
        {
            cerr << "cannot open file: " << filename << endl;
            return;
        }

        // Create a RapidJSON IStreamWrapper using the file input stream above.
        IStreamWrapper isw(input);

        // Create a RapidJSON Document object and use it to parse the IStreamWrapper object above.
        Document d;
        d.ParseStream(isw);

        auto title = d["title"].GetString();
        auto author = d["author"].GetString();
        auto date = d["published"].GetString();

        cout << "------------------------------------------------------------------------------------------------" << endl;
        cout << (c + 1) << ".\tTitle: " << title << endl;
        cout << "\tAuthor: " << author << endl;
        cout << "\tDate: " << date << endl;

        input.close();
    }

    /**
     * @brief Get the Text from the file. This function called in user interface.
     *
     * @param filename
     */
    void getText (const string &filename) {
        fstream input(filename);
        if (!input.is_open())
        {
            cerr << "cannot open file: " << filename << endl;
            return;
        }

        // Create a RapidJSON IStreamWrapper using the file input stream above.
        IStreamWrapper isw(input);

        // Create a RapidJSON Document object and use it to parse the IStreamWrapper object above.
        Document d;
        d.ParseStream(isw);

        auto text = d["text"].GetString();
        cout << "------------------------------------------------------------------------------------------------" << endl;
        cout << text << endl;
        cout << "------------------------------------------------------------------------------------------------" << endl;
    }


private:

    /**
     * @brief This function read Json file then insert it to the corresponding tree.
     * Each tree should contains the word, path of the document where the word is found, and the frequency of the word
     * @param fileName
     */
    void ReadJsonFile(const string &fileName)
    {

        // open an ifstream on the file of interest and check that it could be opened.
        ifstream input(fileName);
        if (!input.is_open())
        {
            cerr << "cannot open file: " << fileName << endl;
            return;
        }

        // Create a RapidJSON IStreamWrapper using the file input stream above.
        IStreamWrapper isw(input);

        // Create a RapidJSON Document object and use it to parse the IStreamWrapper object above.
        Document d;
        d.ParseStream(isw);

        auto persons = d["entities"]["persons"].GetArray();
        auto organization = d["entities"]["organizations"].GetArray();
        auto text = d["text"].GetString();

        //insert the text to text AVLtree

        KeytoTree(text, fileName, textAVLtree);
        //insert the person to person AVLtree
        for (auto &p : persons){
            EntitytoTree(p["name"].GetString(), fileName, personAVLtree);
        }

        //insert the organization to organization AVLtree
        for (auto &og : organization){
            EntitytoTree(og["name"].GetString(), fileName, organizationAVLtree);
        }

    }


    /**
     * @brief This function insert the key and the value to the AVLtree.
     * just for text tree because it has stopword and stemming.
     * @param key a single string word read from JSon file
     * @param path a path that the word is found
     * @param tree AVL tree to insert
     */
    void KeytoTree (string key, const string path,  AvlTree<string, unordered_map <string, int> > &tree) {
        stringstream ss(key);
        string word_in_text;

        while ( ss >> word_in_text){

            if (word_in_text.size() <= 1) {
                continue;
            }

            // Check if word contains any non-alphabetic
            bool has_non_alpha_num = false;
            for (char &c : word_in_text) {
                if (!isalpha(c)) {
                    has_non_alpha_num = true;
                    break;
                }
            }
            // Skip over word if it contains non-alphabetic characters
            if (has_non_alpha_num)
                continue;

            //stemming word
            string wordStemmed = stemWord(word_in_text);

            //If the word_in_text is found in the tree, then insert the new path or updated frequency.
            unordered_map<string, int>* temp_map = tree.findGetMap(wordStemmed);
            if (temp_map != nullptr){
                (*temp_map)[path] += 1;
            }
                //if the word is not found in the tree, insert a new word and path in the tree
            else {
                unordered_map<string, int> temp_map;
                temp_map[path] = 1;
                tree.insert(wordStemmed, temp_map);
            }

        }
    }

    /**
     * @brief this function is relatively similar to keytotree function, however, this is for person
     * and organization. In the purpose of keeping the original name of the person and organization,
     * therefore, there is not stemming or remove non-alphabetic word.
     * @param key
     * @param path
     * @param tree
     */
    void EntitytoTree (string key, const string path, AvlTree<string, unordered_map <string, int> > &tree) {
        if (key.size() <= 1) {
            return;
        }
        for (char &c: key){
            c = tolower(c);
            if (c == ',') {
                key.erase(&c - &key[0], 1);
            }
        }

        unordered_map<string, int>* temp_map = tree.findGetMap(key);
        if (temp_map != nullptr){
            (*temp_map)[path] += 1;
        }
            //if the word is not found in the tree, insert a new word and path in the tree
        else {
            unordered_map<string, int> temp_map;
            temp_map[path] = 1;
            tree.insert(key, temp_map);
        }
    }


    /**
     * @brief This function check if the word is found in the stopwords list
     * using binary search for smaller time complexity O(log n) time
     *
     * @param target is the string to search
     * @return -1 if error opening file,  1 if found, 0 otherwise
     */
    int encounterStopWords(const string target) const {
        //open stopword.txt
        ifstream stopword_file ("/Users/tranlam/Documents/GitHub/assignment-4-search-engine-tran-lam-jordan-young/stopwords.txt");

        //check if file exists
        if (!stopword_file.is_open()){
            //cerr << "Error in opening stopwords file." << endl;
            throw runtime_error("Error in opening stopwords file.");
            //return -1;
        }
        vector<string> stopwords;
        string word;
        while (stopword_file >> word){
            stopwords.push_back(word);
        }

        if (binary_search(stopwords.begin(), stopwords.end(), target))
            return 1; // return 1 if found
        else
            return 0; // return 0 if not found
    }

    /**
     * @brief This function write index from the tree to file
     * by calling internal function from class AvlTree.
     * @param filename
     * @param tree
     */
    void writeIndexToFile(const string &filename, AvlTree<string, unordered_map <string, int> > &tree) {
        if (tree.isEmpty()) {
            //cerr << "Index needs to be created." << endl;
            throw runtime_error("Index needs to be created.");
        }
        //using inorder traversal to write it to the file
        fstream outFile(filename, ios::out);
        if(outFile.fail()) {
            cerr << "Error: Unable to open file " << filename << endl;
            return;
        }
        tree.displayInfoToFile(outFile);
        outFile.close();

    }

    /**
     * @brief This function read the file to the tree.
     *
     * @param filename
     * @param tree
     */
    void readFiletoIndex( const string &filename, AvlTree<string, unordered_map <string, int> > &tree) {
        fstream inFile(filename, ios::in);
        if(!inFile.is_open()) {
            throw runtime_error("Error reading file.");
        }
        tree.clear();
        string line;
        while (getline(inFile, line)){
            stringstream ss(line);
            string word;
            getline(ss, word , ',');
            unordered_map<string, int> tempMap;
            string uuid_freq;
            while(getline(ss, uuid_freq, ',')){
                stringstream is(uuid_freq);
                string path, freq_str;
                getline(is, path, ' ');
                getline(is, freq_str);
                int freq = stoi(freq_str);
                tempMap[path] = freq;
            }
            tree.insert(word, tempMap);
        }

    }
};