/*************************************************************************
	> File Name:		2.cpp
	> File Category: Course Design
	> Author: 		lttzz
	> Mail: 			3344517687@qq.com 
 ************************************************************************/

//注: 此程序以整形存储数字,故当字符串中的数字串过长时不可避免的会溢出
//fdf907 089fa4899f 79fd80 7997fjdls6869
//fdf907 089fa4899f 79fd80 7997fjdls
//有数字串6个, 分别是
//907 89 79 80 7997 6869
#include <iostream>
#include <cstdio>
using namespace std;

const int MAXN = 1000+5; 
char tmp[MAXN];
int cnt, n;
bool flag;

struct Node
{
	int num;
	struct Node *next;
};

struct Node *head = NULL, *p;

void init();
void addNode(int n);
void initStatus();
void readNumString();
void showNum();


int main(void)
{
    //freopen("./in.txt", "r", stdin);
    init();
    readNumString();
    showNum();
    return 0;
}

/**
 * 初始化状态值
 */
void init()
{
    cnt = 0, n = 0;
    flag = false;                  //标记是否找到数字
}

/**
 * 增加数据域为n的节点
 * @param n 新增加节点的数据域
 */
void addNode(int n)
{
	if (head == NULL)
    {
        p = new struct Node;
        head = p;
    }
    else
    {
        p->next = new struct Node;
        p = p->next;
    }
    p->num = n;
    p->next = NULL;
}

/**
 * 重置状态值
 */
void initStatus()
{
	flag = false;
     n = 0;
}

/**
 * 读取字符串,当是数字串时将其插到链表尾
 */
void readNumString()
{
    cout << "Enter a long string: ";
    cin.getline(tmp, 1000, '\n');
    for(int i = 0; tmp[i]; i++)
    {
        if ('0' <= tmp[i] && tmp[i] <= '9')
        {
        	  flag = true;
            n = n * 10 + tmp[i] - '0';     //计算数字串的值(行首零自动过滤掉)
        }                   
        else if (flag)
        {
        	  addNode(n);
        	  initStatus();
            cnt++;                 //统计数字串个数
        }
    }
    if (flag)                       //当字符串以数字结尾的时候,进这儿的if里,添加节点
    {
    	   addNode(n);
        cnt++;
    }
}

/**
 * 输出链表内容,顺便释放内存
 */
void showNum()
{
	cout << "有数字串" << cnt << "个, 分别是: " << endl;
	while (head)
    {
        cout << head->num << endl;
        p = head;
        head = head->next;
        delete p;
    }
}

