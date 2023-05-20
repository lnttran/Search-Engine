## High-level Design of Solution

![UML class diagram](datastructure.pdf)
- High-level pseudo code for each component.

## Your Timeline to Finish the Project
Weekly milestones/tasks. Set out a plan at the beginning and then record when you got it done.

| Task        | Planned completion | Actual completion | Time spent in hrs by X | Time spent in hrs by Y |
| ----------- | :-----------------:| :---------------: | :--------------------: | :--------------------: |
| x           | Nov 3              | Nov 6             | 8.5                    | 0                      |


## Used Data Structures
Explanation of what data structures are used where. For each, state 
- In documentparser.h, hash table (unordered_map) and vector are used. The average case time complexity for all the operations in unordered_map, insert, deletes and updates is O(1), O(1), O(n), best case, average case, and worst case respectively. Space complexity of unordered_map is O(n). 
- For time efficiency, all the tokens and the path file of the token are stored in the unordered_map. Unordered_map is implemented as hash map which which would  a hash function to map keys to buckets in constant time on average. Unordered_map is much faster than regular map becasue it use binary search tree. This would be much efficient in time when it comes to insert and find the key and value. Vector or map could have been used for the data structures, however, vector is contiguous dynamic array, providing constant-time access to elements by index. Map, in another hand, also implemented as binary search tree which maintained elements in sorted order. Therefor, the time complexity of insertion, deletetion, search is O(log n), O(log n), O(n) in best, average and worst case respectively. 

## User Documentation
- How to use the software.
The user will have option to choose in the user interface. In order to search query, the user need to create index from the document first (option 1). This first steps will parse the information from the documents and store it in a corresponding tree. Then, the user will go forward with (option 2), which store the index created to a file for later use purpose. Last, the user can select (option 4) to search query. In addition, the user are allowed to choose one document from the output to display its text. The user can choose (option 5) to view the statistics. 
If the user have already done (option 1) and (option 2) in the first run. In the later search, the user can just choose (option 3) for reading index from the saved file, and the engine is ready for queries search.
- Some example queries with the results.

List of documents that contain: agriculture ORG:agriculture_ministry
------------------------------------------------------------------------------------------------
1.      Title: UPDATE 2-Brazil lifts embargo on some BRF plants, shares rise
        Author: 
        Date: 2018-04-18T18:35:00.000+03:00
------------------------------------------------------------------------------------------------

## Performance
- Statistics for indexing all documents and timing.
Number of file: 10111
Number of texts indexed: 35641
Number of person indexed: 10933
Number of organization indexed: 13530
Time for indexing: 17.72 secondss
Time for write index to file: 36.72 seconds
Time for read index to tree: 1.64 seconds
Time for queries : 0.00 seconds

## Automatic Tests and Memory Tests
You need to describe test cases for:
- Tests for your AVL tree's function
Test inserting and check if the number of nodes is equal the times nodes are inserted. Test remove. Test clear() function. Test rootNode(), copy constructor and print tree.
- Tests for your AVL tree for memory leaks (don't forget to test the persistence functions)
- Tests for indexer
Test if the the index is created and then store it in the file. 
- Tests for performing queries

**Hint:** For testing you should use a very small dataset (e.g., set of documents) where you know what the correct output is 
  and then a larger set to check that your data structures can handle more data.

## Bonus Work (max. of 10)

* During the presentation, we will test your implementation to determine the following. 
    1. The indexing and query speed on a smaller dataset.
    2. The correctness of the result for several queries on that dataset.
  For all teams with correct results, the top 10% of teams according to indexing and query speed will be awarded a bonus of +10. The next 10% will be awarded +5. Note that speed depends all on using the most appropriate and optimized data structures.
* To speed up query processing, you can implement your own hash map from scratch for the organization and person 
  index. (up to +5)
* Analytics for the corpus (e.g., report current indexing speed in documents/second, update the speed every 100 
  documents; 10 most popular terms, 10 least popular terms, number of terms in the index, ...) (up to +2)
* Implementation of very smart relevancy ranking ideas (up to +2)
 

## Grading Rubric

| Task                        | Points Possible | Points Awarded |
| :------------------         | --------------: | -------------: |
| UML diagram/design          | 10              |                |
| AVL tree map + tests        | 20              |                |
| AVL tree persistence + tests| 10              |                |
| Document parser / indexer   | 10              |                |
| Query processor + interface | 20              |                |
| Proper Memory Management    | 5               |                |
| Formatting, Comments, etc.  | 5               |                |
| Documentation               | 20              |                |
| Bonus Work                  | up to +10       |                |
| 48hr early submission bonus | +5              |                |

