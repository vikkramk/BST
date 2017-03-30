#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

/*Node in BST*/
struct Node {
	Node(int val) {	//Constructor
		value = val;
	}

	int value;	//Node value
	Node *left = NULL, *right = NULL;	//Pointers to children
};

/*Print the tree*/
void printTree(Node* parent) {
	queue<Node*> q;	//Nodes to look through
	q.push(parent);
	
	cout << "*********Tree*********" << endl;
	
	bool moreonlevel = true;
	while (moreonlevel) {	//Keep going while more to look through
		int levelCount = q.size();	//Number of nodes in this level of the tree
		moreonlevel = false;
		while (levelCount > 0) {	//While more nodes in this level
			Node* p = q.front();	//Take out a node
			q.pop();
			if (p == NULL) {	//Display empty spaces
				cout << "- " << flush;
				q.push(NULL);	//Null left
				q.push(NULL);	//Null right
			}
			else {
				moreonlevel = true;
				cout << p->value << " " << flush;	//Display it
				q.push(p->left);	//Push left child
				q.push(p->right);	//Push right child
			}
			levelCount--;	//Decrement number left in level
		}
		cout << endl;	//Newline to end level
	}
}

/*Find the min node*/
Node* findMinNode(Node* root) {
	Node* minnode = NULL;
	queue<Node*> q;	//Nodes to look through
	q.push(root);
	
	if (root == NULL)
		return root;

	while (q.size() != 0) {	//While more nodes to go through
		Node* p = q.front();	//Take out a node
		q.pop();
		
		if (minnode == NULL || p->value < minnode->value)
			minnode = p;
		
		if (p->left != NULL)	//If a left child
			q.push(p->left);	//Add child to queue
		if (p->right != NULL)	//If a right child
			q.push(p->right);	//Add child to queue
	}
	
	return minnode;
}

/*Find the parent of node by value*/
Node* findParentNode(int val, Node* root) {
	queue<Node*> q;	//Nodes to look through
	q.push(root);
	
	while (q.size() != 0) {	//While more nodes to go through
		Node* p = q.front();	//Take out a node
		q.pop();
		if (p->left != NULL) {	//If a left child
			if (p->left->value == val)	//Return current node if left child matches
				return p;
			q.push(p->left);	//Add child to queue
		}
		if (p->right != NULL) {	//If a right child
			if (p->right->value == val)	//Return current node if right child matches
				return p;
			q.push(p->right);	//Add child to queue
		}
	}
	
	return NULL;	//Not found
}

/*Delete a tree*/
void deleteTree(Node* root) {
	queue<Node*> q;	//Nodes to look through
	q.push(root);
	
	while (q.size() != 0) {	//While more nodes to go through
		Node* p = q.front();	//Take out a node
		q.pop();
		if (p->left != NULL) {	//If a left child
			q.push(p->left);	//Add child to queue
		}
		if (p->right != NULL) {	//If a right child
			q.push(p->right);	//Add child to queue
		}
		delete p;
	}
}


/*Add a new node, return the root*/
Node* addNode(int toadd, Node* current) {
	if (current == NULL)
		return new Node(toadd);
	
	if (toadd > current->value) {
		if (current->right != NULL)
			addNode(toadd, current->right);
		else
			current->right = new Node(toadd);
	}
	else {
		if (current->left != NULL)
			addNode(toadd, current->left);
		else
			current->left = new Node(toadd);
	}
	
	return current;
}

/*Remove a node with given value, returns the root, can be NULL*/
Node* removeNode(int toremove, Node* root, vector<int>* vals) {
	if (root == NULL)
		return root;
	
	//Get lazy, delete the tree, and make a new one
	deleteTree(root);
	root = NULL;
	for (vector<int>::iterator it = vals->begin(); it != vals->end(); )
		if (*it == toremove)
			it = vals->erase(it);
		else
			++it;
		
	for (vector<int>::iterator it = vals->begin(); it != vals->end(); ++it)
		root = addNode(*it, root);
	
	return root;
}

int main(int argc, char* argv[]) {
	Node* root = NULL;
	vector<int>* vals = new vector<int>();

	
	//Manual input of numbers
	if (argc == 1) {
		bool done = false;
		while (!done) {
			int input;
			cout << "Enter next number to add (0 to quit): " << flush;
			cin >> input;
			if (input == 0)
				done = true;
			else {
				root = addNode(input, root);
				vals->push_back(input);
			}
		}
	}
	
	//File input
	else if (argc == 2) {
		ifstream numfile;

    numfile.open(argv[1]);
    if (!numfile.good()) {
      cout << "Useage: " << argv[0] << " <filepath>" << endl;
      return 1;
    }
				
		bool done = false;
		while (!done) {
			int input;
			numfile >> input;
			
			if (input == 0)
				done = true;
			else {
				root = addNode(input, root);
				vals->push_back(input);
			}
		}
	}
	
	else {
		cout << "Useage: " << argv[0] << " <filepath>" << endl;
    return 1;
	}
	
	//Print tree
	printTree(root);
	
	//Remove numbers
	bool finished = false;
	while (!finished) {
		int toremove;
		cout << "Enter the number to remove (0 to quit):" << flush;
		cin >> toremove;
		if (toremove == 0)
			finished = true;
		else
			root = removeNode(toremove, root, vals);
		
		printTree(root);
	}

	return 0;
}