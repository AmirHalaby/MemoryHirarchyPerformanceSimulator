#include "memorySimulator.h"

struct Node* newNode(bitset<1> value)
{
	Node* n = new Node;
	n->key = value;
	n->left = NULL;
	n->right = NULL;
	return n;
}

struct Node* insertValue(struct Node* root, bitset<1> value,queue<Node*>& q)
{
	Node* node = newNode(value);
	if (root == NULL)
		root = node;

	// The left child of the current Node is
	// used if it is available.
	else if (q.front()->left == NULL)
		q.front()->left = node;

	// The right child of the current Node is used
	// if it is available. Since the left child of this
	// node has already been used, the Node is popped
	// from the queue after using its right child.
	else {
		q.front()->right = node;
		q.pop();
	}

	// Whenever a new Node is added to the tree, its
	// address is pushed into the queue.
	// So that its children Nodes can be used later.
	q.push(node);
	return root;
}

// This function mainly calls insertValue() for
// all array elements. All calls use same queue.
Node* createTree(vector<bitset<1>> arr, int n)
{
	Node* root = NULL;
	queue<Node*> q;
	for (int i = 0; i < n; i++)
		root = insertValue(root, arr[i], q);
	return root;
}

// ттт
struct Node* UpDateValue(int n, int x, struct Node* root, vector<bitset<1>>& arr1)
{
	if (root->key == false) {
		root->key = true;
		arr1[n] = true;
		n++;

		if (root->left == NULL)
		{
			return root;
		}
		UpDateValue(n, x, root->right, arr1);
		return root;
	}

	if (root->key == true) {
		root->key = false;
		arr1[n] = false;
		n++;
		if (root->left == NULL)
		{
			return root;
		}
		UpDateValue(n, x, root->left, arr1);
		return root;
	}
}

// ттт
Node* UpDatePLRUArray(struct Node* root, vector<bitset<1>>& arr1, int Way)
{
	root = UpDateValue(0, Way - 1, root, arr1);

	return root;
}
// ттт
void UpDateLRUArray(vector< vector<unsigned long> >& CounterLRU, int num, int Way,
	long& addressIndexL, unsigned long& Row_LCache) {
	vector< long> temp, array;

	int index = (int)addressIndexL / Row_LCache;
	int X0 = 0;
	int X1 = 1;
	temp.resize(Way);
	array.resize(Way);

	for (int i = 0; i < Way; i++)
	{
		temp[i] = CounterLRU[index][i];
		array[i] = CounterLRU[index][i];
		if (CounterLRU[index][i] == 0)
		{
			X0 = i;
		}
		if (CounterLRU[index][i] == 1)
		{
			X1 = i;
		}
	}


	for (int j = 0; j < Way; j++)
	{
		array[j] = array[j] - 1;
	}
	array[num] = Way - 1;

	if (array[X0] == -1 && array[X1] == 0)
	{
		array[X0] = array[X0] + 1;
		array[X1] = array[X1] + 1;
	}
	if (array[X1] == (Way - 1))
	{
		array[X0] = array[X0] + 1;
	}
	for (int i = 0; i < Way; i++)
	{
		CounterLRU[index][i] = array[i];
	}


}

int checkReplacementPolicy(unsigned long& Row_LCache, long& addressIndexL, unsigned long& Way,
	unsigned long& addressTag_L, vector< vector<bitset<1>> >& ValidBit_L, ReplacementPolicy  Policy,
	vector< vector< unsigned long> >& CounterLRU_L, vector < bitset<1>> arr) {

	int TagInRow = (int)addressIndexL / Row_LCache;
	int x = 0;
	int counter = 0;
	int power = log2(Way) - 1;
	if (Policy == ReplacementPolicy::Random) {
		int randomNumber = rand() % Way;

		// randomNumber in the range 0 to Way num
		return randomNumber;
	}

	if (Policy == ReplacementPolicy::LRU) {
		for (int i = 0; i < Way; i++)
		{
			if (CounterLRU_L[TagInRow][i] == 0) {
				return i;
			}

		}
	}

	if (Policy == ReplacementPolicy::PseudoLRU) {

		for (int i = 0; i < log2(Way); i++)
		{
			if (arr[i] == true)
			{
				x = pow(2, power);

				counter = counter + x;
			}
			power--;

		}
		return counter;

	}

}


void firstDateLRU(vector< vector<unsigned long> >& CounterLRU_L1, vector< vector<unsigned long> >& CounterLRU_L2,
	vector< vector<unsigned long> >& CounterLRU_L3,
	unsigned long& Way1, unsigned long& Way2, unsigned long& Way3,
	unsigned long Row_L1Cache, unsigned long Row_L2Cache, unsigned long Row_L3Cache)
{
	for (int i = 0; i < Row_L1Cache; i++)
	{
		for (int j = 0; j < Way1; j++)
		{
			CounterLRU_L1[i][j] = j;
		}
	}
	for (int i = 0; i < Row_L2Cache; i++)
	{
		for (int j = 0; j < Way2; j++)
		{
			CounterLRU_L2[i][j] = j;
		}
	}
	for (int i = 0; i < Row_L3Cache; i++)
	{
		for (int j = 0; j < Way3; j++)
		{
			CounterLRU_L3[i][j] = j;
		}
	}
}
//ттт
void createTreePLRU(vector<struct Node*>& root1, vector<struct Node*>& root2, vector<struct Node*>& root3,
	unsigned long Row_L1Cache, unsigned long Row_L2Cache, unsigned long Row_L3Cache, long way1, long way2, long way3)
{

	root1.resize(Row_L1Cache);
	root2.resize(Row_L2Cache);
	root3.resize(Row_L3Cache);
	vector<bitset<1>> arr1, arr2, arr3;
	arr1.resize(way1 - 1);
	arr2.resize(way2 - 1);
	arr3.resize(way3 - 1);

	for (int i = 0; i < Row_L1Cache; i++) {
		root1[i] = createTree(arr1, way1 - 1);
	}

	for (int i = 0; i < Row_L2Cache; i++) {
		root2[i] = createTree(arr2, way2 - 1);
	}

	for (int i = 0; i < Row_L3Cache; i++) {
		root3[i] = createTree(arr3, way3 - 1);
	}

}

//ттт
void getCounterPLRU_L(vector< vector<bitset<1>> >& CounterPLRU_L1, long way, unsigned long Row_L1Cache,
	vector< vector<bitset<1>> >& CounterPLRU_L2, unsigned long Row_L2Cache,
	vector< vector<bitset<1>> >& CounterPLRU_L3, unsigned long Row_L3Cache) {


	CounterPLRU_L1.resize(Row_L1Cache);
	CounterPLRU_L2.resize(Row_L2Cache);
	CounterPLRU_L3.resize(Row_L3Cache);

	for (int i = 0; i < Row_L1Cache; i++) {
		CounterPLRU_L1[i].resize(way - 1);
	}

	for (int i = 0; i < Row_L2Cache; i++) {
		CounterPLRU_L2[i].resize(way - 1);
	}
	for (int i = 0; i < Row_L3Cache; i++) {
		CounterPLRU_L3[i].resize(way - 1);
	}


}
