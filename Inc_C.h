/*
文件名称：Inc.h
创建日期：2018-06-28
作者：王垠丁
文件说明：一些字符串操作函数和 双向链表
最后修改日期：2018-06-28
版本：0.0.7
*/
//节点结构体  包含数据、前驱后继指针
#ifndef _INC_C_H
#define _INC_C_H

#include <string>
using namespace std;

//下面是
struct WorkerInfo
{
	//这6个属性是要求用户输入的 
	string id;				//工号
	string name;			//姓名
	float postWages;		//岗位工资 
	float paySalary;		//薪级工资
	float jobAllowance;     //职务津贴
	float performancePay;   //绩效工资


	//这3个属性是根据上面的自动计算 
	float shouldPay;		//应发工资
	float tax;				//个人所得税
	float realWages;		//实发工资
};




typedef WorkerInfo ElementType;

struct NODE
{
	ElementType data;
	NODE *pre;
	NODE *next;
	int size;
};

//创建双向链表
inline void CreateList(NODE **ls)
{
	*ls = new NODE;
	(*ls)->pre = NULL;
	(*ls)->next = NULL;
	(*ls)->size = 0;
}

//返回最后一个节点指针
inline NODE * back(NODE *ls)
{
	NODE* L = ls->next;
	NODE *ret = NULL;
	while (L)
	{
		ret = L;
		L = L->next;
	}
	return ret;
}

//返回第一个节点指针
inline NODE * front(NODE *ls)
{
	return ls->next;
}
	
//链表头部加入数据
inline void push_front(NODE *ls,const WorkerInfo &data)
{
	//新节点
	NODE *node = new NODE;
	node->data = data;
	NODE *firstNode = ls->next;
	if (firstNode)
	{
		firstNode->pre = node;
		node->next = firstNode;
	}
	else
	{
		node->next = NULL;
	}

	ls->next = node;
	node->pre = ls;

	ls->size++;
}



//链表尾部加入数据
inline void push_back(NODE *ls,const WorkerInfo &data)
{
	//新节点
	NODE *node = new NODE;
	node->data = data;
	NODE *backnode = back(ls);
	if (backnode)
	{
		backnode->next = node;
		node->pre = backnode;
	}
	else
	{
		node->pre = ls;
		ls->next = node;
	}
	node->next = NULL;
	ls->size++;
}



//返回第一个节点指针
inline NODE * begin(NODE *ls)
{
	return ls->next;
}



inline NODE * end(NODE *ls)
{
	return NULL;
}

//返回链表元素个数
inline int size(NODE *ls)
{
	return ls->size;
}

//链表是否空
inline bool empty(NODE *ls)
{
	return ls->size == 0;
}

//删除链表的某一个节点
inline void erase(NODE *ls,NODE *node)
{
	if (ls->size == 0 || !node)
		return;

	auto pre = node->pre;
	auto next = node->next;

	if (next)
	{
		pre->next = next;
		next->pre = pre;
	}
	else
	{
		pre->next = NULL;
	}

	delete node;
	node = NULL;
	ls->size--;
}

//删除链表第一个节点
inline void pop_front(NODE *ls)
{
	erase(ls,begin(ls));
}

//删除链表最后一个节点
inline void pop_back(NODE *ls)
{
	erase(ls,back(ls));
}

//清空链表（不删除头结点）
inline void clear(NODE *ls)
{
	while (!empty(ls))
	{
		pop_front(ls);
	}
}

//析构函数自动清理链表占用的内存空间
inline void FreeList(NODE *ls)
{
	clear(ls);
	delete ls;
	ls = NULL;
}

#endif