#include<vector>
#include "bintree.h"
#include<iostream>
using namespace std;
class A
{
	public:
	void func()
	{
		cout << "Class A" << endl;
		std::vector<int> vect;
	}
};

class B
{
	public:
	void func()
	{
		cout << "Class B" << endl;
	}
};		

class C : public A, public B
{
	public:
	virtual void func()
	{
		cout << "Class C" << endl;
	}
};

int main()
{
	cout<<"Hello"<<endl;
    BinaryTree bt;
    bt.print();
    bt.insert(100);
    bt.insert(32);
    bt.insert(2);
    bt.insert(3);
    bt.insert(1);
    bt.insert(32);
    bt.insert(20);
    bt.print();
    
}
 
