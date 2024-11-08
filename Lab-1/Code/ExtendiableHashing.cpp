
#include "ExtendiableHashing.h"
#include <fstream>
#include <bitset>
#include <iostream>
#include <cmath>
#include <string>
using namespace std;

void displayNotFound(int key)
{
	std::cout << "\t\t\t key:  " << key << " \t value : \t not found" << std::endl;
}

// Display one record entry, please don't change this function
void displayItem(DataItem *dataItem)
{
	if (dataItem != 0 && dataItem->valid)
	{
		std::cout << "\t\t\t key: " << dataItem->key << "\t value:\t" << dataItem->data << std::endl;
		// std::cout << "\t\t\t key: " << std::bitset<8>(dataItem->key) << "\t value:\t" << dataItem->data << std::endl;
	}
	else
	{
		std::cout << "\t\t\t key:  ============ \t value:\t empty data" << std::endl;
	}
}

void displayBucket(Bucket &currentBucket, string &depths, string &values, int verbose)
{
	depths.append(std::to_string(currentBucket.localDepth));
	depths.append(",");
	values.append("[");
	if (verbose)
		std::cout << "\t\tBucket:\t local depth:" << currentBucket.localDepth << std::endl;
	if (currentBucket.currentEntries == 0)
	{
		if (verbose)
			std::cout << "\t\t\tNo Data yet\n";
		for (int i = 0; i < RECORDSPERBUCKET; i++)
		{
			values.append("null");
			values.append(",");
		}
	}
	else
	{
		for (int i = 0; i < RECORDSPERBUCKET; i++)
		{
			if (currentBucket.dataItem[i].valid)
			{
				values.append(std::to_string(currentBucket.dataItem[i].data));
				values.append(",");
				if (verbose)
					std::cout << "\t\t\t key: " << currentBucket.dataItem[i].key << "\t value:\t" << currentBucket.dataItem[i].data << std::endl;
				// std::cout << "\t\t\t key: " << std::bitset<8>(currentBucket.dataItem[i].key) << "\t value:\t" << currentBucket.dataItem[i].data << std::endl;
			}
			else
			{
				values.append("null");
				values.append(",");
				if (verbose)
					std::cout << "\t\t\t key:  ============ \t value:\t empty data" << std::endl;
			}
		}
	}
	values.pop_back();
	values.append("]");
}

void displayDirectory(GlobalDirectory &globaldirectory, Bucket &currentBucket, int verbose, std::ofstream &file)
{
	std::cout << "Directory:\t global depth:" << globaldirectory.globalDepth << std::endl;
	file << "Directory:\t global depth:" << globaldirectory.globalDepth << std::endl;

	string values = "(";
	string depths = "(";
	int count = 0;

	if (globaldirectory.length == 0)
	{
		count++;
		std::cout << "\tNo Directory yet\n";
		file << "\tNo Directory yet\n";

		displayBucket(currentBucket, depths, values, verbose);
	}
	else
	{
		for (int i = 0; i < globaldirectory.length; i++)
		{
			if (i == 0)
			{
				count++;
			}
			else
			{
				if (globaldirectory.entry[i - 1] != globaldirectory.entry[i])
					count++;
			}
			if (verbose)
			{
				std::cout << "\t key: " << std::bitset<8>(i) << "\t value:\t" << globaldirectory.entry[i] << std::endl;
				file << "\t key: " << std::bitset<8>(i) << "\t value:\t" << globaldirectory.entry[i] << std::endl;
			}
			displayBucket(*globaldirectory.entry[i], depths, values, verbose);
			if (verbose)
			{
				std::cout << "-----------------------------------------------\n\n";
				file << "-----------------------------------------------\n\n";
			}
		}
		depths.pop_back();
	}

	values.append(")");
	depths.append(")");

	std::cout << " buckets:\t" << count << "/" << globaldirectory.length << std::endl;
	file << " buckets:\t" << count << "/" << globaldirectory.length << std::endl;

	std::cout << "values:\t" << values << std::endl;
	file << "values:\t" << values << std::endl;

	std::cout << "depths:\t" << depths << std::endl;
	file << "depths:\t" << depths << std::endl;

	std::cout << "=========================\n";
	file << "=========================\n";
}

// void displayDirectory(GlobalDirectory &globaldirectory, Bucket &currentBucket, int verbose, std::ofstream &file)
// {
// 	std::cout << "Directory:\t global depth:" << globaldirectory.globalDepth << std::endl;
// 	string values = "(";
// 	string depths = "(";
// 	int count = 0;
// 	// string locations = "(";
// 	if (globaldirectory.length == 0)
// 	{
// 		count++;
// 		std::cout << "\tNo Directory yet\n";
// 		// std::cout << "before display bucket in length = 0";
// 		displayBucket(currentBucket, depths, values, verbose);
// 	}
// 	else
// 	{

// 		for (int i = 0; i < globaldirectory.length; i++)
// 		{
// 			if (i == 0)
// 			{
// 				count++;
// 			}
// 			else
// 			{
// 				if (globaldirectory.entry[i - 1] != globaldirectory.entry[i])
// 					count++;
// 			}
// 			if (verbose)
// 				// std::cout << "\t key: " << i << "\t value:\t" << globaldirectory.entry[i] << std::endl;
// 				std::cout << "\t key: " << std::bitset<8>(i) << "\t value:\t" << globaldirectory.entry[i] << std::endl;
// 			// std::cout << "before display bucket in length > 0";
// 			displayBucket(*globaldirectory.entry[i], depths, values, verbose);
// 			if (verbose)
// 				std::cout << "-----------------------------------------------\n\n";
// 		}
// 		// values.pop_back();
// 		depths.pop_back();
// 	}

// 	values.append(")");
// 	depths.append(")");
// 	std::cout << " buckets:\t" << count << "/" << globaldirectory.length << endl;
// 	std::cout << "values:\t" << values << endl;
// 	std::cout << "depths:\t" << depths << endl;
// 	std::cout << "=========================\n";
// 	// std::cout << "Press any key to continue\n";
// 	char t[100];
// 	// std::cin >> t;
// }

// Hashing function and getting directory Index, please don't change this function
int getCurrentHash(int key, int depth)
{
	// key <= MAXKEYVALUE
	//  How might this return -1?
	int hashedKey = (key & MAXKEYVALUE) >> (MAXKEYLENGTH - depth);
	return hashedKey;
}

// TODO1: Implement this function, Don't change the interface please
//  functionlity: try to insert item into a bucket
//  return:   1 if succedded
//			 0 if failed
//  input:   currentBucket, dataItem to be inserted (check ExtendiableHashing.h for the content of the file)
//  Hint1: don't forget to update currentEntries upon insertion, you will need it later

int insertItemIntoBucket(Bucket &currentBucket, DataItem data)
{
	for (int i = 0; i < RECORDSPERBUCKET; i++)
	{
		if (currentBucket.dataItem[i].key == data.key)
		{
			// If 2 items with the same key are inserted, then behavior of deletion and search will be ambigious since I could match any of the duplicate entries
			cout << "Key already exists, invalid insertion" << endl;
			return 0;
		}
		if (currentBucket.dataItem[i].valid)
		{
			continue;
		}
		currentBucket.dataItem[i] = data;
		currentBucket.currentEntries++;
		currentBucket.dataItem[i].valid = 1;
		return 1;
	}
	return 0;

	// TODO: What is the purpose of the comment below
	//  int hashedKey=getCurrentHash(data.key,currentBucket.localDepth);
	//  if(!currentBucket.dataItem[hashedKey].valid) {
	//  	currentBucket.dataItem[hashedKey].data=data;
	//  	currentBucket.dataItem[hashedKey].valid=true;
	//  	currentBucket.currentEntries++;
	//  	return 1;
	//  }
	//  else {
	//  	return 0;
	//  }
}

// TODO2: Implement this function, Don't change the interface please
//  functionlity: look for an item in a bucket using key, if found call displayItem(..), if not found call displayNotFound()
//  return:   nothing
//  input:   currentBucket, key value to search for
//  Hint: use displayNotFound & displayItem functions

void findItemInBucket(Bucket &currentBucket, int key)
{
	for (int i = 0; i < RECORDSPERBUCKET; i++)
	{
		if (currentBucket.dataItem[i].key == key && currentBucket.dataItem[i].valid == 1)
		{
			displayItem(&currentBucket.dataItem[i]);
			return;
		}
		return;
	}
	displayNotFound(key);
	return;
}

// TODO3: Implement this function, Don't change the interface please
//  functionlity: try to Delete item based on a key value from a bucket
//  return:   1 if succedded
//			 0 if failed (when does it fail to delete??)
//  input:   currentBucket, key to be inserted (check ExtendiableHashing.h)
//  Hint1:   don't forget to update currentEntries upon deletion, you will need it later

int deleteItemFromBucket(Bucket &currentBucket, int key)
{
	for (int i = 0; i < RECORDSPERBUCKET; i++)
	{
		if (currentBucket.dataItem[i].key == key && currentBucket.dataItem[i].valid == 1)
		{
			cout << "Found item to be deleted, " << "key:" << currentBucket.dataItem[i].key << endl;
			currentBucket.dataItem[i].valid = 0;
			currentBucket.currentEntries--;
			return 1;
		}
	}
	return 0;
}

// Helper function
// Functionality: Redistribute data in the bucket
// Input: globaldirectory, distributeIndex (the index of the bucket whise data is to be redistributed)
// Return :	1 if succedded
//			0 if failed
int redistrubteBucket(GlobalDirectory &globaldirectory, int distributeIndex)
{

	// distributeIndex = awel index fel nos el ta7tany ely feha el data
	// cout << "distribute index: " << distributeIndex << endl;
	// This loop redistributes the data in the old bucket to the new buckets
	for (int i = 0; i < RECORDSPERBUCKET; i++)
	{

		// cout << "current entries = " << globaldirectory.entry[distributeIndex]->currentEntries << endl;
		if (!globaldirectory.entry[distributeIndex]->dataItem[i].valid)
		{
			continue;
		}
		int key = globaldirectory.entry[distributeIndex]->dataItem[i].key;
		int keyHash = getCurrentHash(globaldirectory.entry[distributeIndex]->dataItem[i].key, globaldirectory.globalDepth);
		// cout << "key: " << keyHash << endl;
		if (keyHash == -1)
		{
			// cout << "return after keyHash == -1, key =" << key << endl;
			return 0;
		}
		// If key in the upper half (which points at the new empty bucket) then I should insert it in the new bucket
		if (keyHash < distributeIndex)
		{
			// cout << "Attempting to delete item with key:" << key << endl;
			deleteItemFromBucket(*globaldirectory.entry[distributeIndex], key);
			int isSuccess = insertItemIntoBucket(*globaldirectory.entry[keyHash], globaldirectory.entry[distributeIndex]->dataItem[i]);
			if (!isSuccess)
			{
				// isSucess should never be false since I am attempting to distrubute RECORDSPERBUCKET items among 2 buckets
				// Worst case is one of the buckets ends up being full (but it should never overflow)
				// cout << "return after insert" << endl;
				return 0;
			}
		}
		// cout << "End of loop, i = " << i << endl;
		// Else if key is in the bottom half then I shouldnt move it
	}
	return 1;
}

// Helper function
// Functionality: Split bucket at split index and adjust bucket pointers in the global directory
// Input: globaldirectory, splitIndex (the index of the bucket to be split)
// HEBAAAAAAA MARKER
// Return :	Index of first bucket in the bottom half of the split bucket (the half that still points at the old data)
// TODO: When might this function fail? Add return 0 for failure
// TODO: Retest this function when global depth = local depth (i.e when we are extending the directory), it should not work
int splitBucketAli(GlobalDirectory &globaldirectory, int splitIndex)
//split index is now 10 
{	/*
	^ index  	00 points to bucket [1,2]
	^ index 	01 points to bucket [1,2]
	^ index  	10 points to bucket [220,245]
	^ index  	11 points to bucket [220,245]


	^ insert 180 : 1011 0100
	^ split index is now 1 which is no more correct since 00 and 01 points to [1,2] and i will split at index 10 

   */
  	cout<<"split index is now : "<<splitIndex<<endl; 
	cout<<"global depth is now :"<<globaldirectory.globalDepth<<endl; 
	int mask = pow(2, globaldirectory.entry[splitIndex]->localDepth) - 1; // Mask which consists of ones from right to left whose count is equal to the local depth of the bucket
	int shiftedMask = mask << (globaldirectory.globalDepth - globaldirectory.entry[splitIndex]->localDepth); // Shifting mask to left since we would like to extract MSB not LSB
	int bucketIndex= shiftedMask & splitIndex;//bucket index= splitIndex (1)-> 
	cout<<"bucket index now is : "<<bucketIndex<<endl; 
	//^ This line is totally incorrect 
	globaldirectory.entry[splitIndex]->localDepth++;
	/*
	^ in case of inserting key 180 : 1011 0100
	^ where the global depth is now 2 , the bucket index should be 10 , why its zero ?!!!! 
	*/ 
	globaldirectory.entry[bucketIndex] = new Bucket(globaldirectory.entry[splitIndex]->localDepth);
	int index = bucketIndex;
	int half = pow(2, globaldirectory.globalDepth - globaldirectory.entry[splitIndex]->localDepth);
	cout << "half = " << half << endl;
	for (int i = 1; i < half; i++)
	{
		cout << "I am in split loop" << endl;
		globaldirectory.entry[index + i] = globaldirectory.entry[index];
	}
	int newBucketIndex = bucketIndex + pow(2, (globaldirectory.globalDepth - globaldirectory.entry[splitIndex]->localDepth)); // Finding index of the new bucket
	//the one containg old data :
	cout<<" the newBucket Index : containing old data is : "<<newBucketIndex<<endl ; 
	return newBucketIndex;
}

// Helper function
// Functionality: Split bucket at split index then redestribute the data among the 2 new buckets
// Input: globaldirectory, splitIndex (the index of the bucket to be split)
// Return :	1 if succedded
//			0 if failed
int splitBucketAndRedistribute(GlobalDirectory &globaldirectory, int splitIndex)
{
	// HEBAAAAAAA MARKER
	int bucketIndex = splitBucketAli(globaldirectory, splitIndex);
	// displayBuckets(globaldirectory);
	int x = redistrubteBucket(globaldirectory, bucketIndex);
	// cout << "After redistribution: \n";
	// displayBuckets(globaldirectory);
	return x;
}

// TODO4: Implement this function, Don't change the interface please
//  functionlity: try to insert item in the file, if the bucket is full, extend the directory,
//				  if you extended the directory five times but it still doesn't work, return 0
//  return:   1 if succedded
//			 0 if failed (when does it fail to insert??)
//  input:   data:--> dataitem to be inserted, currentBucket --> initialBucket before creating director , globaldirectory
//  Hint: use insertItemIntoBucket,getCurrentHash, extendDirectory functions
//  Hint1:   don't forget to check for corner cases, for example if several entries points to the same bucket and you are going to split it
//  Hint2:   a bucket could be split without expanding the directory (think when this will happen?)
//  Hint3:   don't forget to delete extra data
int insertItem(DataItem data, Bucket &currentBucket, GlobalDirectory &globaldirectory)
{
	// First corner case is done for you: if no directory yet
	if (globaldirectory.globalDepth == 0)
	{
		if (insertItemIntoBucket(currentBucket, data) == 0)
		{
			createFirstTimeDirectory(globaldirectory, currentBucket);
		}
		else
		{
			return 1; // successfully inserted;
		}
	}
	// std::cout << "After creating directory \n";
	int hashedKey = getCurrentHash(data.key, globaldirectory.globalDepth);
	int extentionTimes = 0;
	// As long as the bucket we attempt to insert into is full we will extend the directory and attempt to insert again
	// TODO: Refactor 2 insertItemIntoBucket calls into one call
	while (!insertItemIntoBucket(*globaldirectory.entry[hashedKey], data))
	{
		// std::cout << "Inside outer while loop in insertItem \n";
		// std::cout << " local depth of hashkey is " << globaldirectory.entry[hashedKey]->localDepth << "and global depth is " << globaldirectory.globalDepth << std::endl;
		// If local depth is smaller than global depth, we don't need to extend the directory yet
		// We will attempt to split the full bucket and redistribute the data until the local depth matches the global depth
		while (globaldirectory.globalDepth > globaldirectory.entry[hashedKey]->localDepth)
		{
			cout<<"inside while loop when inserting "<<data.key<< endl; 
			cout<<"global depth now is :"<<globaldirectory.globalDepth<<endl; 
			cout<<"local depth now is :"<<globaldirectory.entry[hashedKey]->localDepth<<endl; 

			// std::cout << "Inside inner while loop in InsertItem" << std::endl;
			splitBucketAndRedistribute(globaldirectory, hashedKey);
			if (insertItemIntoBucket(*globaldirectory.entry[hashedKey], data))
			{
				return 1;
			}
		}
		// If we have extended directory five times and the bucket is still full (we know bucket is full since we havent exited the while loop) then stop
		if (extentionTimes == EXTENTION_LIMIT)
			return 0;
		// Attempting to extend the directory (if successful, will split the full bucket and redistribute the data)
		if (!extendDirectory(globaldirectory, hashedKey,data.key))
			return 0;
		// Getting new hashed key after incrementing the global depth
		hashedKey = getCurrentHash(data.key, globaldirectory.globalDepth);
		extentionTimes++;
	}
	// For if we insert successfulyl after extending the directory and redistributing
	return 1;
}

// TODO5: Implement this function, Don't change the interface please
//  functionlity: search the directory for an item using the key
//  return:   nothing
//  input:   key to be searched for , currentBucket --> initialBucket before creating directory,  globaldirectory
//  Hint1:   use findItemInBucket & getCurrentHash functions

void searchItem(int key, Bucket &currentBucket, GlobalDirectory &globaldirectory)
{
	if (!globaldirectory.globalDepth)
	{
		findItemInBucket(currentBucket, key);
		return;
	}
	int hashedKey = getCurrentHash(key, globaldirectory.globalDepth);
	findItemInBucket(*globaldirectory.entry[hashedKey], key);
	return;
}
// helper function : split and re
void splitBucket(GlobalDirectory &globaldirectory, int splitIndex)
{
	DataItem dataItems[RECORDSPERBUCKET];
	// create a new bucket with the same increased local depth
	Bucket *firstBucket = new Bucket(globaldirectory.entry[splitIndex]->localDepth + 1);
	Bucket *secondBucket = new Bucket(globaldirectory.entry[splitIndex]->localDepth + 1);
	// we want to delete the old bucket and re distrbute the data
	// for every data item in a bucket , delete it

	// now both buckets have increased local depth by one .
	// loop over data in the previous bucket , calculate its new hash
}

// Merge all items in bucket B into bucket A
void mergeBucket(GlobalDirectory &globaldirectory, int indexA, int indexB)
{
	// Loop over all items in bucket B and insert them into A
	for (int i = 0; i < RECORDSPERBUCKET; i++)
	{
		if (globaldirectory.entry[indexB]->dataItem[i].valid == 1)
		{
			insertItemIntoBucket(*globaldirectory.entry[indexA], globaldirectory.entry[indexB]->dataItem[i]);
		}
	}
	delete globaldirectory.entry[indexB];
	// for (int i = 0; i < globaldirectory.length; i++)
	// {
	// 	if (globaldirectory.entry[i] == globaldirectory.entry[indexB])
	// 	{
	// 	}
	// }
	// Have the half that pointed at B, now point at A
	int half = pow(2, globaldirectory.globalDepth - globaldirectory.entry[indexB]->localDepth);
	for (int i = 0; i < half; i++)
	{
		globaldirectory.entry[indexB + i] = globaldirectory.entry[indexA];
	}
	globaldirectory.entry[indexA]->localDepth--;
}

bool shouldMerge(GlobalDirectory &globaldirectory, int hashedKey)
{
	// hashedKey = 001
	// new local = 2
	int newLocal = globaldirectory.entry[hashedKey]->localDepth - 1;
	// mask = 11
	int mask = pow(2, newLocal) - 1;
	// mask = 110
	int shiftedMask = mask << (globaldirectory.globalDepth - newLocal);
	// bucketIndex = 000
	int bucketIndex = hashedKey & shiftedMask;
	// neighborBucketIndex = 001
	int neighborBucketIndex = bucketIndex + pow(2, globaldirectory.globalDepth - (newLocal + 1));
	return (globaldirectory.entry[neighborBucketIndex]->currentEntries + globaldirectory.entry[bucketIndex]->currentEntries <= RECORDSPERBUCKET);
}

// TODO6: Implement this function, Don't change the interface please
//  functionlity: search on an item based on the key and delete it.
//  return:   1 if succedded
//			 0 if failed (when does it fail to delete??)
//  input:   key to be searched for , currentBucket --> initialBucket before creating directory,  globaldirectory
//  Hint:    use deleteItemFromBucket & getCurrentHash & checkDirectoryMinimization functions
//  Hint1:   in case the whole bucket is empty, the bucket should be merged again and the pointer should point to the peer bucket
//  Hint2:   in how many steps do we reach our goal?
//  Hint3:   in case of delete success don't forget to call checkDirectoryMinimization to compress directory if needed.
//  Hint4: You might want to loop on checkDirectoryMinimization, not just call it once to continue merging
int deleteItem(int key, Bucket &currentBucket, GlobalDirectory &globaldirectory)
{
	// Corner case: if no directory yet
	if (!globaldirectory.globalDepth)
	{
		if (!deleteItemFromBucket(currentBucket, key))
			return 0;
		else
			return 1;
	}
	int hashedKey = getCurrentHash(key, globaldirectory.globalDepth);
	if (!deleteItemFromBucket(*globaldirectory.entry[hashedKey], key))
	{
		return 0;
	}

	// While loop since we may need to merge bucket multiple times
	// TODO: What is condition for merging?
	// Condition: If the bucket is less than or equal half full then I should check the adjacent bucket for merging
	// Why? If bucket is more than half full and I am sure the other adjacent bucket is at least half full then (0.5 + x) > 1 where x > 0.5
	// Why ceil? If RECORDSPERBUCKET = 3, and bucket A has 3 record and bucket B has 1 record
	// If we delete an item from bucket A then should should merge with bucket B even though bucket A will more than half (3/2)
	// If RECORDSPERBUCKET = 6
	// If bucket A has 5 records and bucket B has 2 records
	// If we delete an item from bucket A then A has 4 records then we should merge even though A isnt half full
	// (bucketA.currentEntries + bucketB.currententries) <= RECORDSPERBUCKET
	// while (globaldirectory.entry[hashedKey]->currentEntries <= ceil(RECORDSPERBUCKET / 2))
	// {
	// 	// TODO: check if we need to merge, if so merge

	// 	checkDirectoryMinimization(globaldirectory);
	// 	hashedKey = getCurrentHash(key, globaldirectory.globalDepth);
	// }

	while (shouldMerge(globaldirectory, hashedKey))
	{
		cout << "??????????????????????????" << endl;
		cout << "Trying to merge hashed key:" << hashedKey << endl;
		cout << "??????????????????????????" << endl;
		int newLocal = globaldirectory.entry[hashedKey]->localDepth - 1;
		int mask = pow(2, newLocal) - 1;
		int shiftedMask = mask << (globaldirectory.globalDepth - newLocal);
		int bucketIndex = hashedKey & shiftedMask;
		int neighborBucketIndex = bucketIndex + pow(2, globaldirectory.globalDepth - (newLocal + 1));
		mergeBucket(globaldirectory, bucketIndex, neighborBucketIndex);
		checkDirectoryMinimization(globaldirectory);
		hashedKey = getCurrentHash(key, globaldirectory.globalDepth);
	}
	return 1;
	// If global = 3, local = 3
	// If global = 3, local = 2
	// In other words pointer 000 points at 000 and pointer 001 points at 001
	// In other words pointers of 000,001 point at bucket 00 and 010,011 point at bucket 01
	// We would like to check law han merge 000 and 001
	// We would like to check law han merge 00 and 01
	// hashedKey = 001
	// hashedKey = 011
	// I deleted from bucket 001
	// I deleted from bucket 01
	// oldHashedKey = 01
	// Loop over kol el buckets, awel mala2y wa7d el 2 leftmost bits beto3o be 01 ha2ol en da match
	// newLocal = 2
	// newLocal = 1
	int newLocal = globaldirectory.entry[hashedKey]->localDepth - 1;
	// mask = 11
	// mask = 1
	int mask = pow(2, newLocal) - 1;
	// shiftedMask = 110
	// shiftedMask = 100
	int shiftedMask = mask << (globaldirectory.globalDepth - newLocal);
	// bucketIndex = 000
	// bucketIndex = 000
	int bucketIndex = hashedKey & shiftedMask;
	// NeighborBucketIndex = 001
	// NeighborBucketIndex = 010
	int neighborBucketIndex = bucketIndex + pow(2, globaldirectory.globalDepth - newLocal);
	if (globaldirectory.entry[neighborBucketIndex]->currentEntries + globaldirectory.entry[bucketIndex]->currentEntries <= RECORDSPERBUCKET)
	{
		mergeBucket(globaldirectory, bucketIndex, neighborBucketIndex);
		checkDirectoryMinimization(globaldirectory);
	}
}

// create  the first directory, this might help you to implement extendDirectory
int createFirstTimeDirectory(GlobalDirectory &globaldirectory, Bucket &currentBucket)
{
	std::cout << "first time creating directory";
	globaldirectory.globalDepth = 1;
	globaldirectory.length = 2;
	globaldirectory.entry = new Bucket *[globaldirectory.length];
	globaldirectory.entry[0] = new Bucket(globaldirectory.globalDepth);
	globaldirectory.entry[1] = new Bucket(globaldirectory.globalDepth);
	for (int i = 0; i < RECORDSPERBUCKET; i++)
	{
		int newKey = getCurrentHash(currentBucket.dataItem[i].key, globaldirectory.globalDepth);
		// kolo ones is -1
		// TODO: Check why he returns -1
		if (newKey == -1)
		{
			return -1;
		}
		insertItemIntoBucket(*globaldirectory.entry[newKey], currentBucket.dataItem[i]);
	}
	return 1;
}

// TODO: Add a func to display all buckets in a directory and their contents
void displayBuckets(const GlobalDirectory &globaldirectory)
{
	cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n";
	cout << "Global depth: " << globaldirectory.globalDepth << endl;
	for (int i = 0; i < globaldirectory.length; ++i)
	{
		Bucket *currentBucket = globaldirectory.entry[i];
		if (currentBucket != nullptr)
		{
			cout << "\t Bucket " << std::bitset<8>(i) << " (Local Depth: " << currentBucket->localDepth << "): ";
			for (int j = 0; j < RECORDSPERBUCKET; ++j)
			{
				if (currentBucket->dataItem[j].valid)
				{
					cout << "[Key: " << std::bitset<8>(currentBucket->dataItem[j].key) << "," << currentBucket->dataItem[j].key << ", Data: " << currentBucket->dataItem[j].data << "] ";
				}
			}
			cout << endl;
		}
	}
}

// In global directory: 010 points at a bucket whose index is 01
// key = 01000000 local depth = 2 , hashed key = 01
// key = 01000000 local depth = 3 , hashed key = 010

// Comparing key between 010 and 011

// TODO7: Implement this function, Don't change the interface please
//  functionlity: this expands the directory because we can't find a space anymore in the file,
//                it also redistrubtes data over the split buckets
//  return:   1 if succedded
//			 0 if failed (when does it fail to expand??)
//  input:   Directory, hashKey(bucket index) at which the overflow occurs
//  Hint1:   don't forget todelete unneeded pointers to avoid memory leakage
//  Hint2:   what is the size of the new directory compared to old one? what is the new depth?
//  Hint3:   some entries will point to the same bucket
int extendDirectory(GlobalDirectory &globaldirectory, int splitIndex,int key)
{
	// displayBuckets(globaldirectory);
	globaldirectory.globalDepth++;
	if (globaldirectory.globalDepth > MAXKEYLENGTH)
	{
		return 0;
	}
	globaldirectory.length = globaldirectory.length * 2;
	Bucket **prevEntry = globaldirectory.entry;
	globaldirectory.entry = new Bucket *[globaldirectory.length];
	for (int i = 0; i < globaldirectory.length; i++)
	{
		// msk = 1, glo
		int mask = pow(2, globaldirectory.globalDepth - 1) - 1; // This mask is least signifcant maybe?
		mask = mask << 1;
		int oldIndex = i & mask;
		oldIndex = oldIndex >> 1;
		globaldirectory.entry[i] = prevEntry[oldIndex];
	}
	// displayBuckets(globaldirectory);
	delete[] prevEntry;
	cout << "After extending directory" << endl;
	// displayBuckets(globaldirectory);

	// HEBAAAAAAA MARKER 
	return splitBucketAndRedistribute(globaldirectory, getCurrentHash(key, globaldirectory.globalDepth));

	// Old Implementation:
	//  int shiftedNewBucketIndex = newBucketIndex << (globaldirectory.globalDepth - globaldirectory.entry[splitIndex]->localDepth); // Shifting bucket index since bucket index is in MSB not LSB form

	// int newDepth = globaldirectory.entry[splitIndex]->localDepth + 1;
	// // At split index me7tageen ne loop over the items in the bucket to redistribute the data to the new bucket
	// int newSplitIndex = splitIndex + pow(2, globaldirectory.globalDepth - 1); // ya3ni 01 3yzen ne add one more bucket 101
	// globaldirectory.entry[newSplitIndex] = new Bucket(newDepth);
	// for (int i = 0; i < RECORDSPERBUCKET; i++)
	// {
	// 	int newKey = getCurrentHash(globaldirectory.entry[splitIndex]->dataItem[i].key, globaldirectory.globalDepth);
	// 	if (newKey == -1)
	// 	{
	// 		// TODO: When might this fail
	// 		return 0;
	// 	}
	// 	if (newKey != splitIndex)
	// 	{
	// 		int isSuccess = insertItemIntoBucket(*globaldirectory.entry[newSplitIndex], globaldirectory.entry[splitIndex]->dataItem[i]);
	// 		if (!isSuccess)
	// 		{
	// 			// TODO: When might this fail?
	// 			return 0;
	// 		}
	// 		globaldirectory.entry[splitIndex]->dataItem[i].valid = 0;
	// 		globaldirectory.entry[splitIndex]->currentEntries--;
	// 	}
	// }
	// return 1;
}

// HEBAAAAAAA MARKER
// Explanation for extend directory:
// for (int i = 0; i < globaldirectory.length; i++)
// {
// 	// d = 3
// 	// 000 - 111
// 	// new d = 4 , mask = 111
// 	// 0000 - 0111 --> 000 - 111
// 	// 1000 - 1111 --> 000 - 111
// 	// 0000 = 000
// 	// 0101 = 101

// 	// First half:
// 	// 0000 -> 000
// 	// 0101 -> 101

// 	// Second half:
// 	// 1000 -> 000
// 	// 1101 -> 101

// 	new d = 4 , mask = 111
// 	int mask = pow(2,globaldirectory.globalDepth-1);
//  111 & ay 7aga = last 3 bits in ay 7aga
// 	int	oldIndex = i & mask;
//  Last 3 bits in new index == old index
// 	globaldirectory.entry[i] = prevEntry[oldIndex];
// }

// If all buckets have depth less than global depth,
//  compress the directory depth by one
//  loop on this function as appropriate to compress the file back
int checkDirectoryMinimization(GlobalDirectory &globaldirectory)
{

	for (int i = 0; i < globaldirectory.length; i++)
	{
		if (globaldirectory.entry[i]->localDepth == globaldirectory.globalDepth)
			return -1;
	}
	// if its depth =1, there is nothing more we can do, stop here
	if (globaldirectory.length == 1)
		return -1;
	// all are smaller than localdepth, initiate minimization
	//  this is actually too stupid, it should be at least half empty to initiate minimization
	//  but we will keep it like that for simplicity
	// Ali: Makes sense since if the directory is more than half full, splitting the capacity by half will obviously be invalid. But if we are sure that all buckets have depth smaller than global depth, doesnt this already imply that the directory is less than half full?
	int oldGlobalDepth, oldLength;
	Bucket **oldEntry;
	oldGlobalDepth = globaldirectory.globalDepth;
	oldLength = globaldirectory.length;
	globaldirectory.globalDepth -= 1;
	globaldirectory.length /= 2;
	oldEntry = globaldirectory.entry;
	globaldirectory.entry = new Bucket *[globaldirectory.length];
	for (int i = 0; i < globaldirectory.length; i++)
	{
		globaldirectory.entry[i] = oldEntry[2 * i];
	}

	delete[] oldEntry;
	// delete oldEntry;
	return 1;
}