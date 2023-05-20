#define CATCH_CONFIG_MAIN
#include "../catch2/catch.hpp"
#include "../DocumentParser.h"
#include <iostream> 


TEST_CASE("Test Persistence")
{   
    DocumentParser doc;

    SECTION("Write index to file") {
        doc.testFileSystem("sample_data"); 
        doc.TextIndexToFile("textIndex_test.csv"); 
        doc.OrgIndexToFile("orgIndex_test.csv");
        doc.PersonIndexToFile("personIndex_test.csv");
    }
 
    SECTION("Read file to index") {
        doc.fileToOrgTree("orgIndex_test.csv");
        doc.fileToPersonTree("personIndex_test.csv"); 
        doc.fileToTextTree("textIndex_test.csv");
        REQUIRE(doc.getOrgAVLtree().isEmpty() == false);
        REQUIRE(doc.getTextAVLtree().isEmpty() == false);
        REQUIRE(doc.getPersonAVLtree().isEmpty() == false);
        
    }
}