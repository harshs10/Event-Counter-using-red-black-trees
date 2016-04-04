#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <string>

using namespace std;
enum color { RED, BLACK };

/*This structure contains color, id, cnt variables
and pointers to left child, right child and parent.*/

typedef struct rbtree_node
{
	enum color color;
	int id;
	int cnt;
	rbtree_node *lchild, *rchild, *parent;
}*node;

/*This structure contains root of type node.*/

typedef struct rbtree_t
{
	node root;
}*rbtree;

/*This class has all the functions under public view.*/

class EventCnt
{
public:
	typedef int(*compare_func)(int lchild, int rchild);
	void rbtree_insert(rbtree t, int, int, compare_func compare);
	void rbtree_delete(rbtree t, int, compare_func compare);
	node grandparent(node n);
	node sibling(node n);
	node uncle(node n);
	color node_color(node n);
	node new_node(int id, int, color, node, node);
	node findnode(rbtree t, int, compare_func compare);
	void rotate_lchild(rbtree t, node n);
	void rotate_rchild(rbtree t, node n);
	void replacenode(rbtree t, node oldn, node newn);
	void insert(rbtree t, node n);
	node maximum_node(node root);
	int sumtree(node k, int low, int high);
	void increase(rbtree t, int id, int m, compare_func compare);
	void reduce(rbtree t, int id, int m, compare_func compare);
	void count(rbtree t, int, compare_func compare);
	void InRange(rbtree t, int low, int high);
	void Next(rbtree t, int id, compare_func compare);
	void Previous(rbtree t, int id, compare_func compare);
	rbtree buildFromSorted(int size, std::vector<int>&, std::vector<int>&);
	node buildFromSortedhelp(int level, int lo, int hi, int redLevel, std::vector<int> &, std::vector<int> &);
	int compredlevel(int sz);
	void dlete(rbtree t, node n);
};

node EventCnt::grandparent(node n)
{
	return n->parent->parent;
}

node EventCnt::sibling(node n)
{
	if (n == n->parent->lchild)
		return n->parent->rchild;
	else
		return n->parent->lchild;
}

node EventCnt::uncle(node n)
{
	return sibling(n->parent);
}

color EventCnt::node_color(node n)
{
	return n == NULL ? BLACK : n->color;
}

//CREATE TREE

node EventCnt::new_node(int k, int v, color n_color, node lchild, node rchild)
{
	node result = new rbtree_node;
	result->id = k;
	result->cnt = v;
	result->color = n_color;
	result->lchild = lchild;
	result->rchild = rchild;
	if (lchild != NULL)
		lchild->parent = result;
	if (rchild != NULL)
		rchild->parent = result;
	result->parent = NULL;
	return result;
}

/*FIND NODE FUNCTION
Returns the node with the given id
*/
node EventCnt::findnode(rbtree t, int id, compare_func compare)
{
	node n = t->root;
	while (n != NULL)
	{
		int comp_result = compare(id, n->id);
		if (comp_result == 0)
		{
			return n;
		}
		else if (comp_result < 0)
		{
			n = n->lchild;
		}
		else
		{
			n = n->rchild;
		}
	}
	return n;
}

/*COUNT FUNCTION
Prints the count of theID.
If not present, prints 0*/

void EventCnt::count(rbtree t, int id, compare_func compare)
{
	node n = findnode(t, id, compare);
	if (n != NULL)
		cout << n->cnt << "\n";
	else
		cout << "0" << "\n";
}

//ROTATE LEFT from CLRS

void EventCnt::rotate_lchild(rbtree t, node n)
{
	node r = n->rchild;
	replacenode(t, n, r);
	n->rchild = r->lchild;
	if (r->lchild != NULL)
	{
		r->lchild->parent = n;
	}
	r->lchild = n;
	n->parent = r;
}

//ROTATE RIGHT from CLRS

void EventCnt::rotate_rchild(rbtree t, node n)
{
	node L = n->lchild;
	replacenode(t, n, L);
	n->lchild = L->rchild;
	if (L->rchild != NULL)
	{
		L->rchild->parent = n;
	}
	L->rchild = n;
	n->parent = L;
}

/*REPLACE NODE FUNCTION
Replaces old node with new node.*/

void EventCnt::replacenode(rbtree t, node oldn, node newn)
{
	if (oldn->parent == NULL)
	{
		t->root = newn;
	}
	else
	{
		if (oldn == oldn->parent->lchild)
			oldn->parent->lchild = newn;
		else
			oldn->parent->rchild = newn;
	}
	if (newn != NULL)
	{
		newn->parent = oldn->parent;
	}
}

//INSERT FUNCTION  from CLRS

void EventCnt::rbtree_insert(rbtree t, int id, int cnt, compare_func compare)
{
	node inserted_node = new_node(id, cnt, RED, NULL, NULL);
	if (t->root == NULL)
	{
		t->root = inserted_node;
	}
	else
	{
		node n = t->root;
		while (1)
		{
			int comp_result = compare(id, n->id);
			if (comp_result == 0)
			{
				n->cnt = cnt;
				return;
			}
			else if (comp_result < 0)
			{
				if (n->lchild == NULL)
				{
					n->lchild = inserted_node;
					break;
				}
				else
				{
					n = n->lchild;
				}
			}
			else
			{
				if (n->rchild == NULL)
				{
					n->rchild = inserted_node;
					break;
				}
				else
				{
					n = n->rchild;
				}
			}
		}
		inserted_node->parent = n;
	}
	insert(t, inserted_node);
}

void EventCnt::insert(rbtree t, node n)
{
	if (n->parent == NULL)
		n->color = BLACK;
	else if (node_color(n->parent) == BLACK)
			return;
		else if (node_color(uncle(n)) == RED)
			{
				n->parent->color = BLACK;
				uncle(n)->color = BLACK;
				grandparent(n)->color = RED;
				insert(t, grandparent(n));
			}
			else if (n == n->parent->rchild && n->parent == grandparent(n)->lchild)
			{
				rotate_lchild(t, n->parent);
				n = n->lchild;
			}
			else if (n == n->parent->lchild && n->parent == grandparent(n)->rchild)
			{
				rotate_rchild(t, n->parent);
				n = n->rchild;
			}
			n->parent->color = BLACK;
			grandparent(n)->color = RED;
			if (n == n->parent->lchild && n->parent == grandparent(n)->lchild)
			{
				rotate_rchild(t, grandparent(n));
			}
			else
			{
				rotate_lchild(t, grandparent(n));
			}
}

//DELETE FUNCTION from CLRS

void EventCnt::rbtree_delete(rbtree t, int id, compare_func compare)
{
	node child;
	node n = findnode(t, id, compare);
	if (n == NULL)
		return;
	if (n->lchild != NULL && n->rchild != NULL)
	{
		node pred = maximum_node(n->lchild);
		n->id = pred->id;
		n->cnt = pred->cnt;
		n = pred;
	}
	child = n->rchild == NULL ? n->lchild : n->rchild;
	if (node_color(n) == BLACK)
	{
		n->color = node_color(child);
		dlete(t, n);
	}
	replacenode(t, n, child);
	free(n);
}

node EventCnt::maximum_node(node n)
{
	while (n->rchild != NULL)
	{
		n = n->rchild;
	}
	return n;
}

void EventCnt::dlete(rbtree t, node n)
{
	if (n->parent == NULL)
		return;
	else if (node_color(sibling(n)) == RED)
		{
			n->parent->color = RED;
			sibling(n)->color = BLACK;
			if (n == n->parent->lchild)
				rotate_lchild(t, n->parent);
			else
				rotate_rchild(t, n->parent);
		}
	else if (node_color(n->parent) == BLACK && node_color(sibling(n)) == BLACK &&
		node_color(sibling(n)->lchild) == BLACK && node_color(sibling(n)->rchild) == BLACK)
	{
		sibling(n)->color = RED;
		dlete(t, n->parent);
	}
	else if (node_color(n->parent) == RED && node_color(sibling(n)) == BLACK &&
		node_color(sibling(n)->lchild) == BLACK && node_color(sibling(n)->rchild) == BLACK)
	{
		sibling(n)->color = RED;
		n->parent->color = BLACK;
	}
	else if (n == n->parent->lchild && node_color(sibling(n)) == BLACK &&
		node_color(sibling(n)->lchild) == RED && node_color(sibling(n)->rchild) == BLACK)
	{
		sibling(n)->color = RED;
		sibling(n)->lchild->color = BLACK;
		rotate_rchild(t, sibling(n));
	}
	else if (n == n->parent->rchild && node_color(sibling(n)) == BLACK &&
		node_color(sibling(n)->rchild) == RED && node_color(sibling(n)->lchild) == BLACK)
	{
		sibling(n)->color = RED;
		sibling(n)->rchild->color = BLACK;
		rotate_lchild(t, sibling(n));
	}
	sibling(n)->color = node_color(n->parent);
	n->parent->color = BLACK;
	if (n == n->parent->lchild)
	{
		sibling(n)->rchild->color = BLACK;
		rotate_lchild(t, n->parent);
	}
	else
	{
		sibling(n)->lchild->color = BLACK;
		rotate_rchild(t, n->parent);
	}
}

//COMPARE FUNCTION

int compare_int(int lchildp, int rchildp)
{
	int lchild = (int)lchildp;
	int rchild = (int)rchildp;
	if (lchild < rchild)
		return -1;
	else if (lchild > rchild)
		return 1;
	else
	{
		return 0;
	}
}

/*NEXT FUNCTION
Prints the ID and the count of the event with
the lowest ID that is greater than theID. Prints
“0 0”, if there is no next ID*/

void EventCnt::Next(rbtree t, int id, compare_func compare)
{
	node p = t->root;
	while (p != NULL)
	{
		int cmp = compare(id, p->id);
		if (cmp < 0)
		{
			if (p->lchild != NULL)
				p = p->lchild;
			else
				break;
		}
		else
		{
			if (p->rchild != NULL)
			{
				p = p->rchild;
			}
			else
			{
				node par = p->parent;
				node ch = p;
				while (par != NULL && ch == par->rchild)
				{
					ch = par;
					par = par->parent;

				}
				p=par;
				break;
			}
		}
	}
	if (p != NULL)
		cout << int(p->id) << " " << int(p->cnt)<<"\n";
	else
		cout << "0 0"<<"\n";

}

/*PREVIOUS FUNCTION
Prints the ID and the count of the event with
the greatest key that is less that theID. Prints
“0 0”, if there is no previous ID.*/

void EventCnt::Previous(rbtree t, int id, compare_func compare)
{
	node p = t->root;
	while (p != NULL)
	{
		int cmp = compare(id, p->id);
		if (cmp > 0)
		{
			if (p->rchild != NULL)
				p = p->rchild;
			else
				break;
		}
		else
		{
			if (p->lchild != NULL)
			{
				p = p->lchild;
			}
			else
			{
				node par = p->parent;
				node ch = p;
				while (par != NULL && ch == par->lchild)
				{
					ch = par;
					par = par->parent;

				}
				p = par;
				break;
			}
		}
	}
	if (p != NULL)
		cout << int(p->id) << " " << int(p->cnt)<< "\n";
	else
		cout << "0 0" << "\n";
}

/*INRANGE FUNCTION
Prints the total count for IDs between ID1 and
ID2 inclusively. ID1 ≤ ID2*/

void EventCnt::InRange(rbtree t, int low, int high)
{
	node k = t->root;
	int m;
	m = sumtree(k, low, high);
	cout <<m << "\n";
}

/*SUM FUNCTION
Helper function for InRange function, calls itself recursively*/

int EventCnt::sumtree(node k, int low, int high)
{
	int lsum=0,rsum=0,sum=0;
	if (!k) return 0;
	if (k->id <= high && k->id >= low)
	{
		sum = int(k->cnt);
	}
	if (k->id<high)
		lsum=sumtree(k->rchild, low, high);

	if(k->id>low)
		rsum=sumtree(k->lchild, low, high);
	return lsum+rsum+sum;
}

/*INCREASE FUNCTION
Increases the count of the event theID by m. If
theID is not present, inserts it. Prints the count
of theID after the addition*/

void EventCnt::increase(rbtree t, int id,int m, compare_func compare)
{
	node n = findnode(t, id, compare);
	if (n != NULL)
	{
		n->cnt = n->cnt + m;
		cout << n->cnt << "\n";
	}
	else
	{
		rbtree_insert(t, id, m, compare_int);
		cout << m << "\n";
	}
}

/*REDUCE FUNCTION
Decreases the count of theID by m. If theID’s
count becomes less than or equal to 0, removes
theID from the counter. Prints the count of
theID after the deletion, or 0 if theID is
removed or not present.*/

void EventCnt::reduce(rbtree t, int id, int m, compare_func compare)
{
	node n = findnode(t, id, compare);
	if (n != NULL)
	{
		n->cnt = n->cnt - m;
		if(n->cnt<=0)
		{
			rbtree_delete( t, id, compare);
			cout << "0"<<"\n";
		}
		else
		{
			cout << n->cnt << "\n";
		}
	}
	else
	{
		cout << "0" << "\n";
	}
}

/* SORTED BUILD FUNCTION
Builds tree from sorted input in O(n) time, calls buildFromSortedhelp*/

rbtree EventCnt:: buildFromSorted(int size, std::vector<int>& a, std::vector<int>& b)
{
	rbtree t = new rbtree_t;
	t->root = buildFromSortedhelp(0, 0, size - 1, compredlevel(size), a,b);
	return t;
}

/*Helper function for buildFromSorted , calls itself recursively*/

node EventCnt:: buildFromSortedhelp(int level, int lo, int hi, int redLevel, std::vector<int> &a, std::vector<int> &b)
{
	int key, value;
	if (hi < lo)
		return NULL;
	int mid = (lo + hi)/2;
	node left = NULL;
	if (lo < mid)
		left = buildFromSortedhelp(level + 1, lo, mid - 1, redLevel, a,b);
		key = a[mid];		
		value = b[mid];
	node middle = new_node(key, value, BLACK, NULL, NULL);
	if (level == redLevel)
		middle->color = RED;

	if (left != NULL) {
		middle->lchild = left;
		left->parent = middle;
	}
	node right = NULL;
	if (mid < hi) {
		right = buildFromSortedhelp(level + 1, mid + 1, hi, redLevel, a,b);
		middle->rchild = right;
		right->parent = middle;
	}
	return middle;
}

int EventCnt:: compredlevel(int sz)
{
	int level = 0;
	for (int m = sz - 1; m >= 0; m = m / 2 - 1)
		level++;
	return level;
}

/*MAIN PROGRAM
Reads input from the test file and stores no. of nodes in n,
all the ids in vector a and all the counts in vector b.
Calls the buildFromSorted function to build red black tree with n nodes.
Reads input from the terminal and calls the respective function inside
the while loop. Quits the program when input received is 'quit'.
*/

int main(int argc,char *argv[])
{
	ifstream input;
	input.open(argv[1]);
	int n;
	input >> n;
	std::vector<int> a;
	std::vector<int> b;
	for (int i = 0; i < n; i++)
	{
		int id;
		int cnt;
		input >> id >> cnt;
		a.push_back(id);
		b.push_back(cnt);
	}
	input.close();
	EventCnt rbt;
	rbtree t = rbt.buildFromSorted(n ,a, b);
	string s,k=" ",l,d;
	int v,p,i,c;
	
	while (k.compare("quit") != 0)
	{
		getline(cin, s);
		v = s.find(" ");
		k = s.substr(0, v);
		s = s.substr(v + 1);
		if (k.compare("increase") == 0)
		{
			p = s.find(" ");
			l = s.substr(0,p);
			d = s.substr(p + 1);
			i = atoi(l.c_str());
			c = atoi(d.c_str());
			rbt.increase(t, i, c, compare_int);
		}
		else if (k.compare("reduce") == 0)
		{
			p = s.find(" ");
			l = s.substr(0, p);
			d = s.substr(p + 1);
			i = atoi(l.c_str());
			c = atoi(d.c_str());
			rbt.reduce(t, i, c, compare_int);

		}
		else if (k.compare("count") == 0)
		{
			i = atoi(s.c_str());
			rbt.count(t, i, compare_int);
		}
		else if (k.compare("next") == 0)
		{
			i = atoi(s.c_str());
			rbt.Next(t, i, compare_int);
			
		}
		else if (k.compare("previous") == 0)
		{
			i = atoi(s.c_str());
			rbt.Previous(t, i, compare_int);
		}
		else if (k.compare("inrange") == 0)
		{
			p = s.find(" ");
			l = s.substr(0, p);
			d = s.substr(p + 1);
			i = atoi(l.c_str());
			c = atoi(d.c_str());
			rbt.InRange(t, i, c);
		}
	}
	return 0;
}