//============================================================================
// Name        : hashskeleton.cpp
// Author      : Faculty of Engineering, Cairo University
// Version     :
// Description : Hashing using Extendible hashing
//============================================================================

#include <iostream>
#include "ExtendiableHashing.h"

// TODO: Create test cases for multiple const values
// TODO: Test cases
// Test case 0: Given example
// Test case 1: delete kolo le8ayt ma yefda
// Test case 2: An insert that requires multiple directory expansion (less than 5)
// Test case 3: An insert that requires multiple directory expansion (more than 5)
// Test case 4: An insert that requires splitting bucket without directory expansion
// Test case 5: An insert that requires splitting bucket without directory expansion multiple times
// Test case 6: An insert that requires splitting bucket without directory expansion then requires directory expansion
// Test case 7: A delete that requires multiple merges without directory minimization
// Test case 8: A delete that requires multiple merges with directory minimization

int main()
{

    double score = 0.0;
    // Initialize File
    GlobalDirectory globalDirectory;
    Bucket initialFile;
    // Empty Directory
    // displayDirectory(globalDirectory, initialFile, 1);
    //============================================================
    // test insertion without making extra directory
    insertItem(DataItem(1, 33), initialFile, globalDirectory);
    // displayDirectory(globalDirectory, initialFile, 1);
    //============================================================
    // search
    searchItem(13, initialFile, globalDirectory);
    // delete item
    // deleteItem(13, initialFile, globalDirectory);
    // displayDirectory(globalDirectory, initialFile, 1);
    //============================================================

    ////////////////////////////////////////////////////////////

    insertItem(DataItem(2, 20), initialFile, globalDirectory); // hna mafesh directory
    // displayDirectory(globalDirectory, initialFile, 1);
    insertItem(DataItem(128, 70), initialFile, globalDirectory);
    // displayBuckets(globalDirectory);
    // displayDirectory(globalDirectory, initialFile, 1); // hna fe directory , global = 1
    insertItem(DataItem(255, 11), initialFile, globalDirectory);
    // displayBuckets(globalDirectory);
    // displayBuckets(globalDirectory);
    // displayDirectory(globalDirectory, initialFile, 1);
    //============================================================
    insertItem(DataItem(112, 54), initialFile, globalDirectory);
    insertItem(DataItem(240, 40), initialFile, globalDirectory);
    insertItem(DataItem(32, 39), initialFile, globalDirectory);
    insertItem(DataItem(113, 45), initialFile, globalDirectory);
    displayBuckets(globalDirectory);
    // insertItem(DataItem(240, 16), initialFile, globalDirectory);
    // insertItem(DataItem(241, 99), initialFile, globalDirectory);
    // // displayBuckets(globalDirectory);
    // // displayDirectory(globalDirectory, initialFile, 0);
    // //============================================================
    // insertItem(DataItem(159, 30), initialFile, globalDirectory);
    // // displayBuckets(globalDirectory);
    // // displayDirectory(globalDirectory, initialFile, 0);
    // //============================================================

    // insertItem(DataItem(14, 45), initialFile, globalDirectory);
    // insertItem(DataItem(13, 88), initialFile, globalDirectory);
    // // displayBuckets(globalDirectory);
    // // displayDirectory(globalDirectory, initialFile, 0);
    // //============================================================
    // insertItem(DataItem(37, 1), initialFile, globalDirectory);
    // searchItem(13, initialFile, globalDirectory);
    // // displayBuckets(globalDirectory);
    // // displayDirectory(globalDirectory, initialFile, 0);
    // //============================================================
    // // deleteItem(14, initialFile, globalDirectory);
    // // deleteItem(13, initialFile, globalDirectory);
    // // displayBuckets(globalDirectory);
    // // displayDirectory(globalDirectory, initialFile, 0);
    // //============================================================
    // insertItem(DataItem(158, 5), initialFile, globalDirectory);
    // // displayBuckets(globalDirectory);
    // // displayDirectory(globalDirectory, initialFile, 0);
    // insertItem(DataItem(3, 9), initialFile, globalDirectory);
    // // displayBuckets(globalDirectory);
    // // displayDirectory(globalDirectory, initialFile, 0);

    return 0;
}
