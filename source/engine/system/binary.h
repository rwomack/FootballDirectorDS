//	********************************************************
//	Binary Tree Generic Class
//
//	Darren Garside - 03/09/96
//
//	********************************************************


#include <iostream.h>
#include <stdlib.h>


template <class DataT> class tree {
protected:
	DataT info;
	tree *left;
	tree *right;
public:
	tree *root;
	tree() {root = NULL;}

	DataT* get_info(void) { return &info; }
	tree *get_left(void) { return left; }
	tree *get_right(void) { return right; }

	void stree(tree *r, tree *previous, DataT info);		// Create Tree
	tree *dtree(tree *r, DataT key);						// Delete Item
	void inorder(tree *r);									// Display InOrder
	void preorder(tree *r);								// Display PreOrder
	void postorder(tree *r);								// Display PostOrder
	void print_tree(tree *r, int l);						// Display Tree
	tree *search_tree(tree *r, DataT key);					// Search Tree
};


template <class DataT> void tree<DataT>::stree(tree *r, tree *previous, DataT info)
{
	if (!r) {
		r = FD_ENGINE_NEW tree;
/*		if (!r) {
			cout << "Out of Memory. \n";
			exit(1);
		}*/
		r->left = NULL;
		r->right = NULL;
		r->info = info;
		if (!root) root = r;	// first entry
		else {
			if (info < previous->info) previous->left = r;
			else previous->right = r;
		}
		return ;
	}

	if (info < r->info)
		stree(r->left, r, info);
	else
		stree(r->right, r, info);
}


template <class DataT> tree <DataT> *tree <DataT>::dtree(tree *r, DataT key)
{
	tree *p, *p2;

	if (!r) return r; // not found

	if (r->info == key) {	// delete root
		// this means an empty tree
		if (r->left == r->right) {
			free(r);
			if (r==root) root = NULL;
			return NULL;
		}
		// or if one subtree is null
		else if (r->left == NULL) {
			p = r->right;
			free(r);
			if (r==root) root = p;
			return p;
		}
		else if(r->right == NULL) {
			p = r->left;
			free(r);
			if (r==root) root = p;
			return p;
		}
		// or both subtree present
		else {
			p2 = r->right;
			p = r->right;
			while(p->left) p = p->left;
			p->left = r->left;
			free(r);
			if (r==root) root = p2;
			return p2;
		}
	}

	if (r->info < key) r->right = dtree(r->right, key);
	else r->left = dtree(r->left, key);
	return r;
}


template <class DataT> void tree<DataT>::inorder(tree *r)
{
	if (!r) return;

	inorder(r->left);
	if (r->info) cout << r->info << " ";
	inorder(r->right);
}


template <class DataT> void tree<DataT>::preorder(tree *r)
{
	if (!r) return;

	if (r->info) cout << r->info << " ";
	preorder(r->left);
	preorder(r->right);
}


template <class DataT> void tree<DataT>::postorder(tree *r)
{
	if (!r) return;

	postorder(r->left);
	postorder(r->right);
	if (r->info) cout << r->info << " ";
}


template <class DataT> void tree<DataT>::print_tree(tree *r, int l)
{
	int i;

	if (!r) return;

	print_tree(r->right, l+1);
	for (i=0; i<l; ++i) cout << " ";
	cout << r->info << endl;
	print_tree(r->left, l+1);
}


template <class DataT> tree <DataT> *tree <DataT>::search_tree(tree *r, DataT key)
{
	if (!r) return r;	// empty tree
	while(r->info != key) {
		if (key < r->info) r = r->left;
		else r = r->right;
		if (r==NULL) break;
	}
	return r;
}
