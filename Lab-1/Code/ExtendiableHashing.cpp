
#include "ExtendiableHashing.h"
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
		std::cout << "\t\t\t key: " << std::bitset<8>(dataItem->key) << "\t value:\t" << dataItem->data << std::endl;
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
					std::cout << "\t\t\t key: " << std::bitset<8>(currentBucket.dataItem[i].key) << "\t value:\t" << currentBucket.dataItem[i].data << std::endl;
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

void displayDirectory(GlobalDirectory &globaldirectory, Bucket &currentBucket, int verbose)
{
	std::cout << "Directory:\t global depth:" << globaldirectory.globalDepth << std::endl;
	string values = "(";
	string depths = "(";
	int count = 0;
	// string locations = "(";
	if (globaldirectory.length == 0)
	{
		count++;
		std::cout << "\tNo Directory yet\n";
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
				std::cout << "\t key: " << std::bitset<8>(i) << "\t value:\t" << globaldirectory.entry[i] << std::endl;
			displayBucket(*globaldirectory.entry[i], depths, values, verbose);
			if (verbose)
				std::cout << "-----------------------------------------------\n\n";
		}
		// values.pop_back();
		depths.pop_back();
	}

	values.append(")");
	depths.append(")");
	std::cout << " buckets:\t" << count << "/" << globaldirectory.length << endl;
	std::cout << "values:\t" << values << endl;
	std::cout << "depths:\t" << depths << endl;
	std::cout << "=========================\n";
	// std::cout << "Press any key to continue\n";
	char t[100];
	// std::cin >> t;
}

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
	// HEBAAAAAAA MARKER
	Bucket *tempBucket = globaldirectory.entry[distributeIndex];
	globaldirectory.entry[distributeIndex] = new Bucket(globaldirectory.entry[distributeIndex]->localDepth);
	// This loop redistributes the data in the old bucket to the new buckets
	for (int i = 0; i < RECORDSPERBUCKET; i++)
	{
		int key = getCurrentHash(tempBucket->dataItem[i].key, globaldirectory.globalDepth);
		if (key == -1)
		{
			return 0;
		}
		int isSuccess = insertItemIntoBucket(*globaldirectory.entry[key], tempBucket->dataItem[i]);
		if (!isSuccess)
		{
			// isSucess should never be false since I am attempting to distrubute RECORDSPERBUCKET items among 2 buckets
			// Worst case is one of the buckets ends up being full (but it should never overflow)
			return 0;
		}
	}
	delete tempBucket;
	return 1;
}

// Helper function
// Functionality: Split bucket at split index and adjust bucket pointers in the global directory
// Input: globaldirectory, splitIndex (the index of the bucket to be split)
// Return :	1 if succedded
//			0 if failed
// HEBAAAAAAA MARKER
// TODO: When might this function fail? Add return 0 for failure
// TODO: Retest this function when global index = split index (i.e when we are extending the directory), it should not work
int splitBucketAli(GlobalDirectory &globaldirectory, int splitIndex)
{
	// HEBAAAAAAA MARKER
	// Assume global depth = 5 and local depth = 2
	int mask = pow(2, globaldirectory.entry[splitIndex]->localDepth) - 1; // Mask which consists of ones from right to left whose count is equal to the local depth of the bucket
	// mask = 11
	int shiftedMask = mask << (globaldirectory.globalDepth - globaldirectory.entry[splitIndex]->localDepth); // Shifting mask to left since we would like to extract MSB not LSB
	// Why shift max by global - local? For example if global = 5 and local = 2.
	// Then the bucketKey would represent the 2 MSB/left most bits of a 5 bit number. Meaning the mask we need to use is 11000
	// The current mask we have is 11 so we need to shift it to the left
	// // bucketIndex = 010
	int bucketIndex = splitIndex & shiftedMask; // Extracting the indices/keys represented by the bucket
	// Why? We would like to find out which keys belong to our split bucket. Since local depth = 2, we know it will be one of the following:
	// 00, 01, 10, 11
	// If splitIndex is 00xxx then it'll be 00
	// If splitIndex is 01xxx then it'll be 01
	// If splitIndex is 10xxx then it'll be 10
	// etc
	// Assume splitIndex = 9/01001, then bucketIndex = 01000 (notice that the 2 left most bits/ the $localDepth left most bits represent the keys in our bucket)
	// int newBucketIndex = bucketIndex + 1; // Finding index of the new bucket
	int newBucketIndex = bucketIndex + pow(2, (globaldirectory.globalDepth - globaldirectory.entry[splitIndex]->localDepth - 1)); // Finding index of the new bucket
	// Now we know the leftmost $LocaDepth bits of bucketIndex represent the keys of the old bucket before splitting
	// We would like to find the keys of the 2 new buckets after splitting.
	// We know it will always be the leftmost $LocaDepth bits of newBucketIndex followed by 0 or 1 then the remaining bits (according to our global depth)
	// In the example above it would be 01 followed by 0 or 1 followed by 00
	// I.e 010000 and 011000
	// Now the (localDepth + 1) left most bits of newBucketIndex represent the keys of the new buckets after splitting
	// We clearly do not need to change bucketIndex to find the first key
	// For the second key however we need to add a "1" whose position is to the right of the leftmost $localDepth bits
	// You can think of this ones position being defined by shifting to the left by global depth - 1 to reach position of leftmost bit
	// then shifting right by local depth
	globaldirectory.entry[splitIndex]->localDepth++;

	// Now before all 01xxx pointers pointed to the same bucket
	// Now we would like 010xx pointers to point to the same bucket and 011xx pointers to point at a different bucket
	// We can achieve this by leaving the 010xx pointers unchanged
	// Then for any pointer whose index is 011xx we would like to make it point at the new bucket
	int newMask = pow(2, globaldirectory.entry[splitIndex]->localDepth) - 1;
	int newShiftedMask = newMask << (globaldirectory.globalDepth - globaldirectory.entry[splitIndex]->localDepth);
	// This shiftedMask will extract the 3 left most bits eg in this example newShiftedMask = 11100
	// Inside the loop we will check if the 3 left most bits of the bucket index match newBucketIndex i.e 011 then we will make it point at the new bucket

	// This loop creates the new bucket and adjusts the pointers of the global directory accordingly
	for (int i = 0; i < globaldirectory.length; i++)
	{
		// 0100
		int bucketKey = i & newShiftedMask; // In this example extracts the 3 left most bits of the bucket index
		// int shiftedBucketIndex = i >> (globaldirectory.globalDepth - globaldirectory.entry[splitIndex]->localDepth);
		// This condition translates to if the $localDepth+1 left most bits of the bucket index match newBucketIndex i.e if the 3 left most bits match 011
		// Notice that first time this condition is true will be for the index 01100
		if (bucketKey == newBucketIndex)
		{
			// We first need to create the new bucket
			globaldirectory.entry[i] = new Bucket(globaldirectory.entry[splitIndex]->localDepth);
			// Now remember that we need all the 011xx pointers to point at the same bucket not just 01100
			// The number of dont cares on the right is equal to global depth - the new incremented local depth
			// The number of buckets we need to loop over is pow(2, global depth - new local depth)
			for (int j = 1; j < pow(2, globaldirectory.globalDepth - globaldirectory.entry[splitIndex]->localDepth); j++)
			{
				globaldirectory.entry[i + j] = globaldirectory.entry[i];
			}
			return 0; // Success
		}
	}
}

// Helper function
// Functionality: Split bucket at split index then redestribute the data among the 2 new buckets
// Input: globaldirectory, splitIndex (the index of the bucket to be split)
// Return :	1 if succedded
//			0 if failed
int splitBucketAndRedistribute(GlobalDirectory &globaldirectory, int splitIndex)
{
	// HEBAAAAAAA MARKER
	int isSuccess = splitBucketAli(globaldirectory, splitIndex);
	if (!isSuccess)
	{
		return 0;
	}
	return redistrubteBucket(globaldirectory, splitIndex);
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
	int hashedKey = getCurrentHash(data.key, globaldirectory.globalDepth);
	int extentionTimes = 0;
	// As long as the bucket we attempt to insert into is full we will extend the directoryand attempt to insert again
	while (!insertItemIntoBucket(*globaldirectory.entry[hashedKey], data))
	{
		// HEBAAAAAAA MARKER
		// If local depth is smaller than global depth, we don't need to extend the directory yet
		// We will attempt to split the full bucket and redistribute the data until the local depth matches the global depth
		while (globaldirectory.globalDepth > globaldirectory.entry[hashedKey]->localDepth)
		{
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
		if (!extendDirectory(globaldirectory, hashedKey))
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
	return deleteItemFromBucket(*globaldirectory.entry[hashedKey], key);
}

// create  the first directory, this might help you to implement extendDirectory
int createFirstTimeDirectory(GlobalDirectory &globaldirectory, Bucket &currentBucket)
{
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
int extendDirectory(GlobalDirectory &globaldirectory, int splitIndex)
{
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
		int mask = pow(2, globaldirectory.globalDepth - 1); // This mask is least signifcant maybe?
		int oldIndex = i & mask;
		globaldirectory.entry[i] = prevEntry[oldIndex];
	}
	delete[] prevEntry;
	// HEBAAAAAAA MARKER
	return splitBucketAndRedistribute(globaldirectory, splitIndex);

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

// copilot :
int removeMSB(int value, GlobalDirectory &globaldirectory)
{
	// in binar 0111 1111 1111 1111 1111 1111 1111 0101
	// out =    0101 0000 0000 0000
	int mask = 0x7FFFFFFF; // Mask with all bits set to 1 except the MSB

	// globalDepth before incrementing
	value << (sizeof(int) * 8 - globaldirectory.globalDepth) >> (sizeof(int) - globaldirectory.globalDepth);
	return value & mask;
}

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