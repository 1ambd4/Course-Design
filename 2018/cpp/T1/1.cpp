/*************************************************************************
	> File Name:		1.cpp
	> File Category: Course Design
	> Author: 		lttzz
	> Mail: 			3344517687@qq.com 
 ************************************************************************/
 
#include <iostream>
using namespace std;
 
 //一开始想错了,以为 n 报数,最后会剩下 n-1 个人,事实上仍然只会剩下一个人
const int N = 13;
int L1 = 13, P = 0;
int del[N+5] = {0};

int L2 = 13;
struct Node
{
	int data;
	struct Node* next;
};
struct Node *head, *ptr, *tail;

void addOne();
void removeOne();
void show();
void solve();
void initList(int t);
void removeNode();
void showList();
void array();
void list();


int main(void)
{
	array();
	list();
	
	return 0;
}

/**
 * 编号有效加1
 */
void addOne()
{
	while (1)
	{
		P = (P + 1) % N;
		if (!del[P])
		{
			break;
		}
	}
}

/**
 * 删掉某一个,并将剩余个数相应减1
 */
void removeOne()
{
	del[P] = 1;
	L1--;
}

void show()
{
	for (int i = 0; i < N; i++)
	{
		if (!del[i])
		{
			cout << i+1 << " ";
		}
	}
	cout << endl;
}

void solve()
{
	while (L1 > 1)	
	{
		addOne();
		addOne();
		removeOne();

		addOne();
	}
}

/**
 * 初始化链表,构造有t个节点的链表,并将尾指针指向首结点
 * @param t 节点个数
 */
void initList(int t)
{
	for (int i = 1; i <= t; i++)
	{
		struct Node *newnode = new struct Node;
		newnode->data = i;
		newnode->next = NULL;
		if (NULL == head)
		{
			head = tail = newnode;
		}
		else
		{
			tail->next = newnode;
			tail = newnode;
		}
	}
	tail->next = head;
}

/**
 * 删掉节点,只要剩余节点个数大于1,就继续删除下去
 * 删除节点的时候顺便释放内存
 * 删除完毕后将尾节点指向NULL,以便调用showList()函数
 */
void removeNode()
{
	struct Node *p = head;
	struct Node *t = NULL;
	while (L2 > 1)
	{
		p = p->next;
		t = p->next;
		p->next = p->next->next;
		delete t;
		p = p->next;
		L2--;
	}
	head = p;
	tail->next = NULL;
}

void showList()
{
	for (ptr = head; ptr != NULL; ptr = ptr->next)
	{
		cout << ptr->data << " ";
	}
	cout << endl;
}

void array()
{
	solve();
	show();
}

void list()
{
	initList(N);
	removeNode();
	showList();
}
