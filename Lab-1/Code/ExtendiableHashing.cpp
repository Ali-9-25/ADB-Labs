
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

// Hashing function and getting directory Index, please don't change this function
int getCurrentHash(int key, int depth)
{
	// key <= MAXKEYVALUE
	//  How might this return -1?
	int hashedKey = (key & MAXKEYVALUE) >> (MAXKEYLENGTH - depth);
	return hashedKey;
}
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
}
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
int redistrubteBucket(GlobalDirectory &globaldirectory, int distributeIndex)
{
	for (int i = 0; i < RECORDSPERBUCKET; i++)
	{
		if (!globaldirectory.entry[distributeIndex]->dataItem[i].valid)
		{
			continue;
		}
		int key = globaldirectory.entry[distributeIndex]->dataItem[i].key;
		int keyHash = getCurrentHash(globaldirectory.entry[distributeIndex]->dataItem[i].key, globaldirectory.globalDepth);
		if (keyHash == -1)
		{
			return 0;
		}
		// If key in the upper half (which points at the new empty bucket) then I should insert it in the new bucket
		if (keyHash < distributeIndex)
		{
			deleteItemFromBucket(*globaldirectory.entry[distributeIndex], key);
			int isSuccess = insertItemIntoBucket(*globaldirectory.entry[keyHash], globaldirectory.entry[distributeIndex]->dataItem[i]);
			if (!isSuccess)
			{
				return 0;
			}
		}
	}
	return 1;
}
int splitBucket(GlobalDirectory &globaldirectory, int splitIndex)
{
	int mask = pow(2, globaldirectory.entry[splitIndex]->localDepth) - 1; // Mask which consists of ones from right to left whose count is equal to the local depth of the bucket
	int shiftedMask = mask << (globaldirectory.globalDepth - globaldirectory.entry[splitIndex]->localDepth); // Shifting mask to left since we would like to extract MSB not LSB
	int bucketIndex= shiftedMask & splitIndex;
	globaldirectory.entry[splitIndex]->localDepth++;
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
int splitBucketAndRedistribute(GlobalDirectory &globaldirectory, int splitIndex)
{
	int bucketIndex = splitBucket(globaldirectory, splitIndex);
	int x = redistrubteBucket(globaldirectory, bucketIndex);
	return x;
}

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
	// As long as the bucket we attempt to insert into is full we will extend the directory and attempt to insert again
	while (!insertItemIntoBucket(*globaldirectory.entry[hashedKey], data))
	{
		while (globaldirectory.globalDepth > globaldirectory.entry[hashedKey]->localDepth)
		{
			cout<<"inside while loop when inserting "<<data.key<< endl; 
			cout<<"global depth now is :"<<globaldirectory.globalDepth<<endl; 
			cout<<"local depth now is :"<<globaldirectory.entry[hashedKey]->localDepth<<endl;
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
	return 1;
}

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
	int half = pow(2, globaldirectory.globalDepth - globaldirectory.entry[indexB]->localDepth);
	for (int i = 0; i < half; i++)
	{
		globaldirectory.entry[indexB + i] = globaldirectory.entry[indexA];
	}
	globaldirectory.entry[indexA]->localDepth--;
}

bool shouldMerge(GlobalDirectory &globaldirectory, int hashedKey)
{
	
	int newLocal = globaldirectory.entry[hashedKey]->localDepth - 1;
	int mask = pow(2, newLocal) - 1;
	int shiftedMask = mask << (globaldirectory.globalDepth - newLocal);
	int bucketIndex = hashedKey & shiftedMask;
	int neighborBucketIndex = bucketIndex + pow(2, globaldirectory.globalDepth - (newLocal + 1));
	return (globaldirectory.entry[neighborBucketIndex]->currentEntries + globaldirectory.entry[bucketIndex]->currentEntries <= RECORDSPERBUCKET);
}
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
	int newLocal = globaldirectory.entry[hashedKey]->localDepth - 1;
	int mask = pow(2, newLocal) - 1;
	int shiftedMask = mask << (globaldirectory.globalDepth - newLocal);
	int bucketIndex = hashedKey & shiftedMask;
	int neighborBucketIndex = bucketIndex + pow(2, globaldirectory.globalDepth - newLocal);
	if (globaldirectory.entry[neighborBucketIndex]->currentEntries + globaldirectory.entry[bucketIndex]->currentEntries <= RECORDSPERBUCKET)
	{
		mergeBucket(globaldirectory, bucketIndex, neighborBucketIndex);
		checkDirectoryMinimization(globaldirectory);
	}
}
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

int extendDirectory(GlobalDirectory &globaldirectory, int splitIndex,int key)
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
		int mask = pow(2, globaldirectory.globalDepth - 1) - 1; // This mask is least signifcant maybe?
		mask = mask << 1;
		int oldIndex = i & mask;
		oldIndex = oldIndex >> 1;
		globaldirectory.entry[i] = prevEntry[oldIndex];
	}
	delete[] prevEntry;
	cout << "After extending directory" << endl;

	return splitBucketAndRedistribute(globaldirectory, getCurrentHash(key, globaldirectory.globalDepth));
}
int checkDirectoryMinimization(GlobalDirectory &globaldirectory)
{

	for (int i = 0; i < globaldirectory.length; i++)
	{
		if (globaldirectory.entry[i]->localDepth == globaldirectory.globalDepth)
			return -1;
	}

	if (globaldirectory.length == 1)
		return -1;
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
	return 1;
}