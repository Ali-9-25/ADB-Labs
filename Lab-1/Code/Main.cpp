//============================================================================
// Name        : hashskeleton.cpp
// Author      : Faculty of Engineering, Cairo University
// Version     :
// Description : Hashing using Extendible hashing
//============================================================================

#include <iostream>
#include "ExtendiableHashing.h"
#include <fstream>

int main()
{

    double score = 0.0;
    std::ofstream outFile("output.txt");
    if (!outFile)
    {
        std::cerr << "Error opening file for writing\n";
        return 1;
    }

    // Initialize File
    GlobalDirectory globalDirectory;
    Bucket initialFile;
    insertItem(DataItem(220, 220), initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0, outFile);
    insertItem(DataItem(245, 245), initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0, outFile);
    insertItem(DataItem(180, 180), initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0, outFile);
    insertItem(DataItem(255, 255), initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0, outFile);
    insertItem(DataItem(1, 1), initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0, outFile);
    insertItem(DataItem(2, 2), initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0, outFile);
    insertItem(DataItem(127, 127), initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0, outFile);
    //^ My TestCase 
    //  cout<<"inserted 1"<<endl;
    // insertItem(DataItem(1, 1), initialFile, globalDirectory);
    // displayDirectory(globalDirectory, initialFile, 0, outFile);
    // cout<<"inserted 2"<<endl;
    // insertItem(DataItem(2, 2), initialFile, globalDirectory);
    // displayDirectory(globalDirectory, initialFile, 0, outFile);
    // cout<<"inserted 220"<<endl;
    // insertItem(DataItem(220, 220), initialFile, globalDirectory);
    // displayDirectory(globalDirectory, initialFile, 0, outFile);
    // cout<<"inserted 245"<<endl;
    // insertItem(DataItem(245, 245), initialFile, globalDirectory);
    // displayDirectory(globalDirectory, initialFile, 0, outFile);
    // cout<<"inserted 180"<<endl;
    // insertItem(DataItem(180, 180), initialFile, globalDirectory);
    // displayDirectory(globalDirectory, initialFile, 0, outFile);
    // cout<<"inserted 225"<<endl;
    // insertItem(DataItem(255, 255), initialFile, globalDirectory);
   
    outFile.close();

    return 0;
}
