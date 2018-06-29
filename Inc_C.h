/*
�ļ����ƣ�Inc.h
�������ڣ�2018-06-28
���ߣ�����
�ļ�˵����һЩ�ַ������������� ˫������
����޸����ڣ�2018-06-28
�汾��0.0.7
*/
//�ڵ�ṹ��  �������ݡ�ǰ�����ָ��
#ifndef _INC_C_H
#define _INC_C_H

#include <string>
using namespace std;

//������
struct WorkerInfo
{
	//��6��������Ҫ���û������ 
	string id;				//����
	string name;			//����
	float postWages;		//��λ���� 
	float paySalary;		//н������
	float jobAllowance;     //ְ�����
	float performancePay;   //��Ч����


	//��3�������Ǹ���������Զ����� 
	float shouldPay;		//Ӧ������
	float tax;				//��������˰
	float realWages;		//ʵ������
};




typedef WorkerInfo ElementType;

struct NODE
{
	ElementType data;
	NODE *pre;
	NODE *next;
	int size;
};

//����˫������
inline void CreateList(NODE **ls)
{
	*ls = new NODE;
	(*ls)->pre = NULL;
	(*ls)->next = NULL;
	(*ls)->size = 0;
}

//�������һ���ڵ�ָ��
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

//���ص�һ���ڵ�ָ��
inline NODE * front(NODE *ls)
{
	return ls->next;
}
	
//����ͷ����������
inline void push_front(NODE *ls,const WorkerInfo &data)
{
	//�½ڵ�
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



//����β����������
inline void push_back(NODE *ls,const WorkerInfo &data)
{
	//�½ڵ�
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



//���ص�һ���ڵ�ָ��
inline NODE * begin(NODE *ls)
{
	return ls->next;
}



inline NODE * end(NODE *ls)
{
	return NULL;
}

//��������Ԫ�ظ���
inline int size(NODE *ls)
{
	return ls->size;
}

//�����Ƿ��
inline bool empty(NODE *ls)
{
	return ls->size == 0;
}

//ɾ�������ĳһ���ڵ�
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

//ɾ�������һ���ڵ�
inline void pop_front(NODE *ls)
{
	erase(ls,begin(ls));
}

//ɾ���������һ���ڵ�
inline void pop_back(NODE *ls)
{
	erase(ls,back(ls));
}

//���������ɾ��ͷ��㣩
inline void clear(NODE *ls)
{
	while (!empty(ls))
	{
		pop_front(ls);
	}
}

//���������Զ���������ռ�õ��ڴ�ռ�
inline void FreeList(NODE *ls)
{
	clear(ls);
	delete ls;
	ls = NULL;
}

#endif