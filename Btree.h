#ifndef H_Btree
#define H_Btree

#include <iostream>
#include <fstream>
#include <cassert>
#include <queue>
using namespace std;

static int BtreeOrder;

class Btree
{
public:
	class BtreeNode
	{
	public:
		int recCount;
		int* list = new int[BtreeOrder - 1];
		BtreeNode **children = new BtreeNode*[BtreeOrder];
	};
	bool search(const int& searchItem);
	void insert(const int& insertItem);
	void inOrder();
	void printLevel(int level);
	Btree() {}
	Btree(int order) { BtreeOrder = order; root = nullptr; }
protected:
	BtreeNode *root;

private:
	void searchNode(BtreeNode *current, const int& item, bool& found, int& location);
	void insertBtree(BtreeNode *current, const int &insertItem, int &median, BtreeNode* &rightChild, bool &isTaller, bool& found);
	void insertNode(BtreeNode *current, const int& insertItem, BtreeNode* &rightChild, int insertPosition);
	void splitNode(BtreeNode *current, const int& insertItem, BtreeNode* rightChild, int insertPosition, BtreeNode* &rightNode, int &median);
	void recInorder(BtreeNode *current);
	void recPrintLevel(BtreeNode* current, int level, bool& exists);
};

bool Btree::search(const int& searchItem)
{
	bool found = false;
	int location;

	BtreeNode *current;

	current = root;

	while (current != nullptr && !found)
	{
		searchNode(current, searchItem, found, location);

		if (!found)
			current = current->children[location];
	}

	return found;
} //end search


void Btree::searchNode
(BtreeNode *current, const int& item, bool& found, int& location) // Change to binary search
{
	location = 0;
	while (location < current->recCount
		&& item > current->list[location])
		location++;

	if (location < current->recCount
		&& item == current->list[location])
		found = true;
	else
		found = false;
} //end searchNode

void Btree::insert(const int& insertItem)
{
	bool isTaller = false;
	int median;
	BtreeNode *rightChild;
	bool found;

	insertBtree(root, insertItem, median,
		rightChild, isTaller, found);

	if (isTaller) 
				  
	{
		BtreeNode *tempRoot;
		tempRoot = new BtreeNode;
		tempRoot->recCount = 1;
		tempRoot->list[0] = median;
		tempRoot->children[0] = root;
		tempRoot->children[1] = rightChild;

		root = tempRoot;
	}
} //insert


void Btree::insertBtree(BtreeNode *current, const int &insertItem, int &median, BtreeNode* &rightChild, bool &isTaller, bool& found)
{
	isTaller = false;
	int location;
	if (current == NULL)
	{
		median = insertItem;
		rightChild = NULL;
		isTaller = true;
	}
	else
	{
		searchNode(current, insertItem, found, location); if (found) { return; }
		else
		{
			int newMedian;
			BtreeNode* node;
			insertBtree(current->children[location], insertItem, newMedian, node, isTaller, found);
			if (isTaller)
			{
				if (current->recCount < BtreeOrder - 1)
				{
					isTaller = false;
					insertNode(current, newMedian, node, location);
				}
				else
					splitNode(current, newMedian, node, location, rightChild, median);
			}
		}
	}
} 


void Btree::insertNode
(BtreeNode *current, const int& insertItem, BtreeNode* &rightChild, int insertPosition)
{
	int index;

	for (index = current->recCount; index > insertPosition;
		index--)
	{
		current->list[index] = current->list[index - 1];
		current->children[index + 1] = current->children[index];
	}

	current->list[index] = insertItem;
	current->children[index + 1] = rightChild;
	current->recCount++;
} 

void Btree::splitNode
(BtreeNode *current, const int& insertItem, BtreeNode* rightChild, int insertPosition, BtreeNode* &rightNode, int &median)
{
	rightNode = new BtreeNode;

	int mid = (BtreeOrder - 1) / 2;

	if (insertPosition <= mid)

	{
		int index = 0;
		int i = mid;

		while (i < BtreeOrder - 1)
		{
			rightNode->list[index] = current->list[i];
			rightNode->children[index + 1] =
				current->children[i + 1];
			index++;
			i++;
		}

		current->recCount = mid;
		insertNode(current, insertItem, rightChild,
			insertPosition);
		(current->recCount)--;

		median = current->list[current->recCount];

		rightNode->recCount = index;
		rightNode->children[0] =
			current->children[current->recCount + 1];
	}
	else
	{
		int i = mid + 1;
		int index = 0;

		while (i < BtreeOrder - 1)
		{
			rightNode->list[index] = current->list[i];
			rightNode->children[index + 1] =
				current->children[i + 1];
			index++;
			i++;
		}

		current->recCount = mid;
		rightNode->recCount = index;

		median = current->list[mid];
		insertNode(rightNode, insertItem, rightChild,
			insertPosition - mid - 1);
		rightNode->children[0] =
			current->children[current->recCount + 1];
	}
} 

void Btree::inOrder()
{
	recInorder(root);
} 


void Btree::recInorder(BtreeNode *current)
{
	if (current != nullptr)
	{
		recInorder(current->children[0]);

		for (int i = 0; i < current->recCount; i++)
		{
			cout << current->list[i] << " ";

			recInorder(current->children[i + 1] );
		}
	}
} 

void Btree::printLevel(int level)
{
	bool exists = false;
	int count = 0;
	recPrintLevel(root, level, exists);
	if (!exists)
		cout << "empty";
}

void Btree::recPrintLevel(BtreeNode* current, int level, bool& exists) // Currently prints only the first level
{
	if (current == nullptr)
		return;
	if (level == 1)
	{
		for (int i = 0; i < current->recCount; i++)
		{
			cout << current->list[i] << " ";
			exists = true;
		}
	}
	else if (level > 1)
	{
		for (int i = 0; i < ((current->recCount)+1); i++) 
			recPrintLevel(current->children[i], level - 1, exists);
	}
}

#endif
