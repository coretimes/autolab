#include "custommath.h"
#include <iostream>
#include <netinet/in.h>
#include "stdio.h"
#include<vector>

using namespace std;
int sub(int a, int b)
{
	int sum = 0;
	for(int i = a; i < b; i++)
	{
		sum +=i;
	}
	return sum;
}

int add(int a, int b)
{
	int sum = 0;
	for(int i = a; i < b; i++)
	{
		sum -= i;
	}
	return sum;
}

int main()
{
	int a = 0;
	int b = 5;
	for(int i = 0; i < 20; i++)
	{
		a = b + i;
		b = a - i;
	}
	printf("sum(a,b)=%d", sub(a,b));
	printf("add(a,b)=%d", add(a,b));

	cout << "hello world" << endl;
	fun oFun;
	cout << oFun.add(1,3) << endl;
	cout << oFun.sub(1,3) << endl;
	cout << oFun.add(1,3) << endl;
	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(13);

	servaddr.sin_family = htons(13);
	std::vector<int> aa;
	aa.push_back(3);
	aa.push_back(4);
	cout << aa.at(0) << aa.at(1);
}
