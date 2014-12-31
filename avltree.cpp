#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct person{
private:
	string dob, ssn, fn, ln, st;
	int zip;
public:
	person(){
		dob="";
		ssn="";
		fn="";
		ln="";
		st="";
		zip=0;
	};
	person(string d, string ss, string f, string l, string s, int z){
		dob=d;
		ssn=ss;
		fn=f;
		ln=l;
		st=s;
		zip=z;
	};
	void print(ostream &out){
		out<<dob<<" "<<ssn<<" "<<fn<<" "<<ln<<" "<<st<<" "<<zip<<endl;
	};
	string getFname() {return fn;};
	string getLname(){return ln;};
	string getDOB(){return dob;};

};

int comparePerson(person a, person b){
  if (a.getLname()>b.getLname()) return 1;
  else if(a.getLname()<b.getLname()) return -1;
  else {
    if(a.getFname()>b.getFname()) return 1;
    else if(a.getFname()<b.getFname()) return -1;
    else {
      if(a.getDOB()>b.getDOB()) return 1;
      else if (a.getDOB()<b.getDOB()) return -1;
      else return 0;
    }
  }
}


struct avlNode{
private:
	person data;
	int height;
public:
	avlNode *left, *right;
	avlNode(person p){
		left=NULL;
		right=NULL;
		data=p;
		height=1;
	};
	avlNode(){
		left=NULL;
		right=NULL;
		data=person();
		height=0;
	};
	person getPerson(){return data;}
	int getHeight(){return height;}
	avlNode * getLeft(){return left;}
	avlNode * getRight(){return right;}
	int bal();
	void fixHeight();
};
//When the node is NULL
int heightNode(avlNode* n)
{	if (n==NULL) return 0;
	else return n->getHeight();
}


int avlNode::bal(){return(heightNode(right)- heightNode(left));}

//Updates the height to max(left height, right height) + 1
void avlNode::fixHeight(){
		int hl=heightNode(left);
		int hr=heightNode(right);
		if (hl>hr) height = hl+1;
		else height=hr+1;
	}
//help function--given root n, print the tree
void printTree (avlNode *n, ostream &out){
	if (n!=NULL){
		printTree(n->left, out);
		n->getPerson().print(out);
		printTree(n->right, out);
	}
}
class avlTree{
private:
	avlNode *root;
//	avlNode *rotateRight(avlNode *);
//	avlNode *rotateLeft(avlNode *);
//	avlNode *balance(avlNode *);
public:
	avlTree(){
		root=NULL;
	}
	void insert(person);
	//int treeHeight(); //the height of the root
	void print(ostream &out){printTree(root, out);}
};
//help function -- rotate right around node n, and return the new root
avlNode* rotateRight(avlNode *n){
	avlNode *q=n->left;
	n->left=q->right;
	q->right=n;
	n->fixHeight();
	q->fixHeight();
	return q;
}
//help function -- rotate left around node n, and return the new root
avlNode* rotateLeft(avlNode *n){
	avlNode *q=n->right;
	n->right=q->left;
	q->left=n;
	n->fixHeight();
	q->fixHeight();
	return q;
}

//make sure the node n has a balance of 0, 1, or -1
avlNode * balance(avlNode *n){
	n->fixHeight();
	if(n->bal()==2){
		if(n->right->bal() < 0)
			n->right = rotateRight(n->right);
		return rotateLeft(n);
	}
	if(n->bal()==-2){
		if(n->left->bal() > 0)
			n->left = rotateLeft(n->left);
		return rotateRight(n);
	}
	return n;
}

//add person p to the root n and return the new root 
avlNode* avlInsert(avlNode *n, person p){
	if (n==NULL){return new avlNode(p);}
	if (comparePerson(p,n->getPerson())<0)
		n->left=avlInsert(n->left, p);
	else n->right=avlInsert(n->right,p);
	return balance(n);
}

void avlTree::insert(person p){
	root=avlInsert(root, p);
}

void main(int argc, char* argv[]){
if (argc!=2) cout<<"Type in the input file name"<<endl;
else{
	ifstream fin;
	fin.open(argv[1], ios::in);
    if(!fin.is_open())
		cout<<"Error opening input file"<<endl;
	else {
		avlTree t;
		person p;
	    string d, sn, first, l, s;
	    int z;
		cout<<"Reading the input file "<<argv[1]<<".."<<endl;
	    while(!fin.eof()){
		  fin>>d>>sn>>first>>l>>s>>z;
		  p=person(d,sn,first,l,s,z);
		  t.insert(p);
		}
		fin.close();
		ofstream fout;
		fout.open("avlout.txt",ios::out|ios::trunc);
		if(!fout.is_open()){
			cout<<"Error opening output file"<<endl;}
		else {
			cout<<"Writing to the output file \"avlout.txt\""<<endl;
			t.print(fout);
		}
			fout.close();
	}
}
}
