#define CATCH_CONFIG_MAIN
#include "../catch2/catch.hpp"
#include "../AVLtree.h"
#include <iostream> 


TEST_CASE("Checking AVLtree")
{   
    AvlTree <string, int> tree;
    
    SECTION("Inserting new element.") {
        tree.insert("ten", 10); 
        tree.insert("two", 2);
        tree.insert("three", 3);
        tree.insert("eight", 8);
        tree.insert("nine", 9);
        tree.insert("seven", 7);
        tree.insert("five", 5);

        REQUIRE(tree.numOfNode() == 7); 
        REQUIRE(tree.find("three") == true);
    }

    SECTION("Test remove() function"){
        tree.insert("ten", 10); 
        tree.insert("two", 2);
        tree.insert("three", 3);
        tree.remove("three");
        REQUIRE(tree.numOfNode() == 2); 
    }

    SECTION("Test clear() function"){
        tree.insert("ten", 10); 
        tree.insert("two", 2);
        tree.insert("three", 3);
        tree.clear(); 
        REQUIRE(tree.isEmpty() == true); 
        REQUIRE(tree.numOfNode() == 0); 
    }

    SECTION("Test rootNode() function") {
        tree.insert("ten", 10); 
        tree.insert("two", 2);

        REQUIRE(tree.rootNode()->key == "ten"); 
        REQUIRE(tree.rootNode()->value == 10);
    }

    SECTION("Test Copy Constructor") {
        tree.insert("ten", 10); 
        tree.insert("two", 2);
        tree.insert("three", 3);
        AvlTree <string, int> tree2 = tree; 
        tree2.insert("seven", 7);

        REQUIRE(tree2.numOfNode() == 4); 
    }

    SECTION("Print tree") {
        tree.insert("ten", 10); 
        tree.insert("two", 2);
        tree.insert("three", 3);
        tree.insert("eight", 8);
        tree.insert("nine", 9);
        tree.insert("seven", 7);
        tree.insert("five", 5);
        cout << "Print inorder: " << endl; 
        tree.printInorder();
        cout << endl;

        cout << "Print level-order: " << endl; 
        tree.prettyPrintTree(); 
        cout << endl;

        cout << "Pretty print tree: " << endl; 
        tree.prettyPrintTree(); 
        cout << endl; 
    }

}