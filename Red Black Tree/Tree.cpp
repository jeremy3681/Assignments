#include "Tree.h"
Tree::Tree()
{
	root = NULL;
}

Tree::~Tree()
{
	this->destroyTree(root);
}

void Tree::add(string & newword)
{
	Node* newnode = new Node;
	newnode->word = newword;

	if (isEmpty())
	{
		newnode->red = false;
		root = newnode;
		return;
	}
	addRecursive(newnode, root);
}

void Tree::addRecursive(Node * newnode, Node * subtree)
{
	if (subtree->word == newnode->word)
	{
		subtree->count++;
		delete newnode;
		newnode = NULL;
		return;
	}
	else if (subtree->word > newnode->word)
	{
		if (subtree->left == NULL)
		{
			newnode->parent = subtree;
			subtree->left = newnode;
			balance(newnode);
		}
		else
			addRecursive(newnode, subtree->left);
	}
	else
	{
		if (subtree->right == NULL)
		{
			newnode->parent = subtree;
			subtree->right = newnode;
			balance(newnode);
		}
		else
			addRecursive(newnode, subtree->right);
	}
}

void Tree::walk()const
{
	walkInOrderRecursive(root);
}

bool Tree::isEmpty()const
{
	if (root == NULL)
	{
		return true;
	}
	return false;
}

void Tree::display()const
{
	displayRecursive(root, 0);
}

void Tree::verify()const
{
	verifyRecursive(root);
}

void Tree::balance(Node * target)
{
	Node * uncle;
	Node * gparent;
	while (target != root && target->red && target->parent->red)
	{
		gparent = target->parent->parent;
		//left from gparent
		if (target->parent == gparent->left)
		{
			uncle = gparent->right;
			//red uncle
			if (uncle != NULL && uncle->red)
			{
				gparent->red = true;
				uncle->red = target->parent->red = false;
				target = gparent;
			}
			//black uncle
			else
			{
				// left right from gparent
				if (target == target->parent->right)
				{
					leftrotate(target->parent);
					target = target->left;
				}
				//left left from gparent
				rightrotate(gparent);
				target->parent->red = false;
				gparent->red = true;
				target = target->parent;
			}
		}
		//right from gparent
		else
		{
			uncle = gparent->left;
			//red uncle
			if (uncle != NULL && uncle->red)
			{
				gparent->red = true;
				uncle->red = target->parent->red = false;
				target = gparent;
			}
			else //black uncle
			{
				// right left from gparent
				if (target == target->parent->left)
				{
					rightrotate(target->parent);
					target = target->right;
				}
				//right right from gparent
				leftrotate(gparent);
				target->parent->red = false;
				gparent->red = true;
				target = target->parent;
			}
		}
		root->red = false;
	}
}

void Tree::debugbalance(Node * target)
{
	Node * uncle;
	Node * gparent;
	while (target != root && target->red && target->parent->red)
	{
		gparent = target->parent->parent;
		//left from gparent
		if (target->parent == gparent->left)
		{
			uncle = gparent->right;
			//red uncle
			if (uncle != NULL && uncle->red)
			{
				gparent->red = true;
				uncle->red = target->parent->red = false;
				target = gparent;
			}
			//black uncle
			else
			{
				// left right from gparent
				if (target == target->parent->right)
				{
					leftrotate(target->parent);
					target = target->left;
				}
				//left left from gparent
				rightrotate(gparent);
				target->parent->red = false;
				gparent->red = true;
				target = target->parent;
			}
		}
		//right from gparent
		else
		{
			uncle = gparent->left;
			//red uncle
			if (uncle != NULL && uncle->red)
			{
				gparent->red = true;
				uncle->red = target->parent->red = false;
				target = gparent;
			}
			else //black uncle
			{
				// right left from gparent
				if (target == target->parent->left)
				{
					rightrotate(target->parent);
					target = target->right;
				}
				//right right from gparent
				leftrotate(gparent);
				target->parent->red = false;
				gparent->red = true;
				target = target->parent;
			}
		}
		root->red = false;
	}
}

void Tree::rightrotate(Node * target)
{
	Node * oldlefttree = target->left;

	if (oldlefttree == NULL)
		return;

	target->left = oldlefttree->right;

	if (target->left != NULL)
		target->left->parent = target;

	oldlefttree->parent = target->parent;

	if (target->parent == NULL)
		root = oldlefttree;
	else if (target == target->parent->left)
		target->parent->left = oldlefttree;
	else
		target->parent->right = oldlefttree;

	oldlefttree->right = target;
	target->parent = oldlefttree;
}

void Tree::leftrotate(Node *target)
{
	Node * oldrighttree = target->right;

	if (oldrighttree == NULL)
		return;

	target->right = oldrighttree->left;

	if (target->right != NULL)
		target->right->parent = target;

	oldrighttree->parent = target->parent;

	if (target->parent == NULL)
		root = oldrighttree;
	else if (target == target->parent->left)
		target->parent->left = oldrighttree;
	else
		target->parent->right = oldrighttree;

	oldrighttree->left = target;
	target->parent = oldrighttree;
}

void Tree::displayRecursive(Node * subtree, int level)const
{
	if (subtree == NULL)
		return;

	displayRecursive(subtree->right, level + 1);

	if (level != 0)
	{
		for (int i = 0;i < level - 1;i++)
			cout << " |  ";

		if (subtree->red)
			cout << " |--(R)";
		else
			cout << " |--(B)";
		cout << subtree->word << endl;
	}
	else
	{
		if (subtree->red)
			cout << "(R)";
		else
			cout << "(B)";
		cout << subtree->word << endl;
	}
	displayRecursive(subtree->left, level + 1);
}

void Tree::walkInOrderRecursive(Node * subtree)const
{
	if (subtree != NULL)
	{
		walkInOrderRecursive(subtree->left);
		cout << subtree->count << " " << subtree->word << endl;
		walkInOrderRecursive(subtree->right);
	}
}

int Tree::verifyRecursive(Node * subtree)const
{
	if (subtree == NULL)
		return 0;
			
	if (subtree->left != NULL)
	{
		if (subtree->left->parent != subtree)
			cerr << "Broken Parent Link:" << subtree->word << endl;
		if (subtree->red&&subtree->left->red)
			cerr << "Double Red Nodes:" << subtree->word << endl;
	}
	if (subtree->right != NULL)
	{
		if (subtree->right->parent != subtree)
			cerr << "Broken Parent Link:" << subtree->word << endl;
		if (subtree->red&&subtree->right->red)
			cerr << "Double Red Nodes:" << subtree->word << endl;
	}
		
	int rblack = verifyRecursive(subtree->right);
	int lblack = verifyRecursive(subtree->left);
	
	if (rblack != lblack)
	{
		cerr << "Black Height Different:" << subtree->word << " right blacks:" << rblack << " left blacks:"<< lblack << endl;
	}

	if (!subtree->red)
		return lblack+1;
	else
		return lblack;


}

void Tree::destroyTree(Node * subtree)
{
	if (subtree != NULL)
	{
		destroyTree(subtree->left);
		destroyTree(subtree->right);
		delete subtree;
	}
}


