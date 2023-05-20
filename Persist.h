//
// Created by jorda on 4/27/2023.
//

#ifndef FINALPROJEXAMPLES_PERSIST_H
#define FINALPROJEXAMPLES_PERSIST_H

//Class needs to contain persistence mechanisms
//accepts the avl trees and reads out the info to an output file
//can also read from the designated output file

#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/document.h"
#include "DocumentParser.h"
#include "AVLtree.h"
#include <sstream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <map>

using namesapce rapidjson;
using namespace std;

class Persist
{
private:

public:
    void WritePersistFile(AvlTree<string, unordered_map <string, int> > persons, AvlTree<string, unordered_map <string, int> > orgs, AvlTree<string, unordered_map <string, int> > terms) {

    Document d;
    const char* json;

    string jsonString = "{\"PERSONS\": ";

    jsonString += traverseTree(persons.rootNode()) + ", \"ORGS\": " + traverseTree(orgs.rootNode()) + ",\"TERMS\": " + traverseTree(terms.rootNode()) + "}";

    json = jsonString;

    d.parse(json);

    ofstream ofs("persistDir.json");

    OStreamWrapper osw(ofs);

    Writer<OStreamWrapper> writer(osw);

    d.Accept(writer);

    ofs << json;

    }

    void ReadPersistFile(AvlTree<string, unordered_map <string, int> > persons, AvlTree<string, unordered_map <string, int> > orgs, AvlTree<string, unordered_map <string, int> > terms) {

        ifstream input("persistDir.json");

        IStreamWrapper isw(input);

        Document d;
        d.ParseStream(isw);

        auto personsArray = d["PERSONS"].GetArray();
        auto orgsArray = d["ORGS"].GetArray();
        auto termsArray = d["TERMS"].GetArray();

        for (auto name : personsArray) {
            for (auto pair : name) {
                persons.insert(name, pair, persons.rootNode());
            }
        }

        for (auto organization : orgsArray) {
            for (auto pair : organization) {
                orgs.insert(organization, pair, orgs.rootNode());
            }
        }

        for (auto word : termsArray) {
            for (auto pair : word) {
                terms.insert(word, pair, terms.rootNode());
            }
        }

    }

    string traverseTree(AvlNode* node) {
        string allData = "";
        if (node == nullptr) {
            return allData;
        }

        traverseTree(node->left);

        allData += "{\"" + node->key + "\": ";
        for (pair<string s, int i> p : node->value) {
            allData +=  "{" + s + "\": " + to_string(i) + "},"
        }
        allData = allData.substr(0, allData.size()-1);
        allData += "}";

        traverseTree(node->right);
    }
};

#endif //FINALPROJEXAMPLES_PERSIST_H
