#ifndef QUERYPRO_H
#define QUERYPRO_H

#include "DocumentParser.h"
#include <sstream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>

using namespace std;

//gettext

class queryPro {

private:

    vector<pair<string, int> >resultWithRank; //store the final 15 highest relevancy ranking file 

public:

    /**
     * @brief Search engine. This is the function that does the searching job and print out the list of documents.
     * 
     * @param input 
     * @param filename 
     */
    void output(string input, DocumentParser &d){
        //use stringstream for tokenization 
        stringstream ss(input);
        string word;

        //retrieve the list of documents after searching the word in the tree
        unordered_map<string, int> ogr_result, person_result, result, not_include;

        //Analyzing the user input
        while (ss >> word){
            if(word.substr(0,4) == "ORG:"){
                string org = word.substr(4);
                for ( char &c : org) {
                    if(c == '_')
                        c = ' ';
                    c = tolower(c);
                }
                ogr_result = searchWord(org, d.getOrgAVLtree());
            }
            else if (word.substr(0,7)== "PERSON:") {
                string person = word.substr(7);
                for ( char &c : person) {
                    if(c == '_')
                        c = ' ';
                    c = tolower(c);
                }
                person_result = searchWord(person, d.getPersonAVLtree());
            }
            else if (word.substr(0,1) == "-") {
                string notInclude = d.stemWord(word.substr(1));
                not_include = searchWord(notInclude, d.getTextAVLtree());
            }
            else {
                string wordStemmed = d.stemWord(word);

                unordered_map<string, int> wordResult = searchWord(wordStemmed, d.getTextAVLtree());

                if (result.empty())
                    result.insert(wordResult.begin(), wordResult.end());
                else {
                    findIntersection(result, wordResult);
                }
            }
        }

        if(!ogr_result.empty() || !person_result.empty() || !not_include.empty() || !result.empty()) {

            if (!person_result.empty() && result.empty())
                result = person_result;
            else if (!ogr_result.empty() && result.empty())
                result = ogr_result;

            if (!result.empty() && !person_result.empty()){
                findIntersection(result, person_result);
            }

            if (!result.empty() && !ogr_result.empty()){
                findIntersection(result, ogr_result);
            }

            //if there is a word that user does not want to include
            if (!result.empty() && !not_include.empty()){
                deleteIntersection(result, not_include);
            }
        }

        if (result.empty()) {
            cout << "************************************************************************************************" << endl;
            cerr << "There is no documents that contain: " << input << endl;
            cout << "************************************************************************************************" << endl;
        }
        else {
            cout << "************************************************************************************************" << endl;
            cout << "List of documents that contain: " << input << endl;
            resultWithRank = relevancyRanking(result);
            for ( size_t c = 0; c < resultWithRank.size(); ++c){
                d.getFileInfo(resultWithRank[c].first , c);
            }
            cout << "------------------------------------------------------------------------------------------------" << endl;
        }
    }

    /**
     * @brief this function called when the user choose one file to display text
     * 
     * @param num number that associated with the file
     * @param d 
     */
    void displayText (int num, DocumentParser &d) {
        if (num <= resultWithRank.size()){
            d.getText(resultWithRank[num - 1].first);
        }
        else
            cout << "The file is not exist." << endl;
    }

    /**
     * @brief this function search a word in its corresponding category tree.
     * 
     * @param w word
     * @param tree 
     * @return vector<pair <string, int>> the vector of list of the uuid and frequency that the word appears.
     * What will this return if the word is not found in the tree based map? nullptr
     */
    unordered_map<string, int> searchWord(const string &w, const AvlTree<string, unordered_map <string, int> > &tree){
        //return an empty vector if the word is not found in the tree
        //using function find node to return to the node contains the word 
        if (tree.findNode(w) == nullptr)
            throw runtime_error("Error: Word is not found. \nSuggestion: \n1. Create an index from a directory with documents.\n2. Write index to the file.\n3. Read an index from a file.\n4. Search query.");

        return tree.findNode(w)->value;
    }

    /**
     * @brief This function finds the intersection of the two document vectors
     * Return intersection with the largest frequency of the word appear in the same document  
     * @param a vector a 
     * @param b vector b
     */
    void findIntersection( unordered_map<string, int> &a, const unordered_map<string, int> &b) {
        unordered_map<string, int> result;
        //if appears, insert the key with the highest frequency to the result map
        for (const auto& i : a) {
            if(b.count(i.first) > 0) {
                result[i.first] = max(i.second, b.at(i.first));
            }
        }
        //Update the map a with the result
        a = result;
    }

    /**
     * @brief This function deletes intersection. Function is for do not contain a particular word  
     * @param a the vector to update
     * @param b vector 2 
     */
    void deleteIntersection( unordered_map<string, int> &a, const unordered_map<string,int> &b) {
        //iterating over the container
        for ( auto it = a.begin(); it != a.end();) {
            //check if the key exitsts in the second map
            //if the key in vector a found in the vector b
            if(b.find(it->first) != b.end()){
                //erase the key in vector a
                it = a.erase(it);
                //don't need to increment the iterator in the loop header (++it) becasue the 
                //erase() function already return an iterator to the next element
            }
            else
                ++it;
        }
    }

    /**
     * @brief This function display the list of document in the most relevant document
     * return the 15 highest frequency.
     * https://stackoverflow.com/questions/5056645/sorting-stdmap-using-value
     */
    vector<pair<string, int> >relevancyRanking (unordered_map<string, int> &m) const {
        vector<pair<string, int> > highestRel;
        for (auto& it : m){
            highestRel.push_back(it);
        }
        sort(highestRel.begin(), highestRel.end(),[](const auto &a, const auto &b){return a.second > b.second;});
        highestRel.resize(min(15, (int)highestRel.size()));

        return highestRel;
    }

    void menu() {
        cout << "************************************************************************************************" << endl;
        cout << "                                   WELLCOME TO SEARCH ENGINE" << endl;
        cout << "************************************************************************************************" << endl;
        cout << "                           1. Create an index from a directory with documents." << endl;
        cout << "                           2. Write index to the file. " << endl;
        cout << "                           3. Read an index from a file. " << endl;
        cout << "                           4. Search query." << endl;
        cout << "                           5. Print statistic." << endl;
        cout << "                           6. Quit." << endl;
        cout << "************************************************************************************************" << endl;
        cout << "Your choice > ";
    }

};

#endif