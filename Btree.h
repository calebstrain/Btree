#ifndef H_Btree
#define H_Btree

#include <iostream>
#include <fstream>
#include <cassert>
#include <queue>
using namespace std;

static int BtreeOrder;

template<class T>
class Btree
{
private:
	struct BtreeNode
	{
		int recCount;
		T* list = new T[BtreeOrder - 1];
		BtreeNode** children = new BtreeNode * [BtreeOrder];
	};
	void searchNode(BtreeNode* current, const T& item, bool& found, int& location);
	void insertBtree(BtreeNode* current, const T& insertItem, int& median, BtreeNode*& rightChild, bool& isTaller, bool& found);
	void insertNode(BtreeNode* current, const T& insertItem, BtreeNode*& rightChild, int insertPosition);
	void splitNode(BtreeNode* current, const T& insertItem, BtreeNode* rightChild, int insertPosition, BtreeNode*& rightNode, int& median);
	void recInorder(BtreeNode* current);
	void recPrintLevel(BtreeNode* current, int level, bool& exists);

public:
	bool search(const T& searchItem);
	void insert(const T& insertItem);
	void inOrder();
	void printLevel(int level);
	Btree() { BtreeOrder = 3; root = nullptr; } // by default the order of the tree is 3
	Btree(int order) { BtreeOrder = order; root = nullptr; }
protected:
	BtreeNode* root;
};

template<class T>
bool Btree<T>::search(const T& searchItem)
{
	bool found = false;
	int location;

	BtreeNode* current;

	current = root;

	while (current != nullptr && !found)
	{
		searchNode(current, searchItem, found, location);

		if (!found)
			current = current->children[location];
	}

	return found;
}


template<class T>
void Btree<T>::searchNode(BtreeNode* current, const T& item, bool& found, int& location)
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
} 

template<class T>
void Btree<T>::insert(const T& insertItem)
{
	bool isTaller = false;
	int median;
	BtreeNode* rightChild;
	bool found;

	insertBtree(root, insertItem, median,
		rightChild, isTaller, found);

	if (isTaller)

	{
		BtreeNode* tempRoot;
		tempRoot = new BtreeNode;
		tempRoot->recCount = 1;
		tempRoot->list[0] = median;
		tempRoot->children[0] = root;
		tempRoot->children[1] = rightChild;

		root = tempRoot;
	}
}

template<class T>
void Btree<T>::insertBtree(BtreeNode* current, const T& insertItem, int& median, BtreeNode*& rightChild, bool& isTaller, bool& found)
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

template<class T>
void Btree<T>::insertNode(BtreeNode* current, const T& insertItem, BtreeNode*& rightChild, int insertPosition)
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

template<class T>
void Btree<T>::splitNode(BtreeNode* current, const T& insertItem, BtreeNode* rightChild, int insertPosition, BtreeNode*& rightNode, int& median)
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

template<class T>
void Btree<T>::inOrder()
{
	recInorder(root);
}

template<class T>
void Btree<T>::recInorder(BtreeNode* current)
{
	if (current != nullptr)
	{
		recInorder(current->children[0]);

		for (int i = 0; i < current->recCount; i++)
		{
			cout << current->list[i] << " ";

			recInorder(current->children[i + 1]);
		}
	}
}

template<class T>
void Btree<T>::printLevel(int level)
{
	if (level < 1)
	{
		cout << "Level must be greater than 0";
		return;
	}
	bool exists = false;
	int count = 0;
	recPrintLevel(root, level, exists);
	if (!exists)
		cout << "empty";
}

template<class T>
void Btree<T>::recPrintLevel(BtreeNode* current, int level, bool& exists) // Currently prints only the first level
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
		for (int i = 0; i < ((current->recCount) + 1); i++)
			recPrintLevel(current->children[i], level - 1, exists);
	}
}

#endif
