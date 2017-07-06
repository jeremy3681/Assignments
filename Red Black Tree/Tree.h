#pragma once
#include <string>
#include <iostream>
using namespace std;
class Tree
{
public:
	Tree();
	~Tree();
	void add(string & newword);
	void walk()const;
	bool isEmpty()const;
	void display()const;
	void verify()const;
private:
	struct Node
	{
		Node *left=NULL;
		Node *right=NULL;
		Node *parent=NULL;
		unsigned int count=1;
		bool red = true;
		string word;
	};
	Node *root;
	void balance(Node* added);
	void debugbalance(Node * target);
	void rightrotate(Node*);
	void leftrotate(Node*);
	void addRecursive(Node* newnode, Node* subtree);
	void displayRecursive(Node* subtree, int indent)const;
	void walkInOrderRecursive(Node* subtree)const;
	int verifyRecursive(Node* subtree)const;
	void destroyTree(Node* subtree);
};



