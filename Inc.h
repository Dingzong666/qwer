/*
文件名称：Inc.h
创建日期：2018-06-28
作者：王垠丁
文件说明：一些字符串操作函数和 双向链表
最后修改日期：2018-06-28
版本：0.0.7
*/
#ifndef _INC_H_
#define _INC_H_
#include <string>
#include <iostream>
#include <vector>
#include <io.h>
#include <fstream>
using namespace std;


namespace Utility
{
	//字符串切割
	inline std::vector<std::string> SplitString(const std::string &str, const std::string &pattern)
	{
		std::vector<std::string> vec;
		std::string strs = str + pattern;

		size_t pos = strs.find(pattern);
		size_t size = strs.size();

		while (pos != std::string::npos)
		{
			std::string x = strs.substr(0, pos);
			vec.push_back(x);
			strs = strs.substr(pos + 1, size);
			pos = strs.find(pattern);
		}
		return vec;
	}

	//去掉字符串的空格
	inline std::string StringTrim(std::string str)
	{
		std::string temp;
		for (size_t i = 0; i < str.length(); i++)
		{
			if (str.at(i) != ' ' && str.at(i) != '\t')
			{
				temp += str.at(i);
			}
		}
		return temp;
	}

	//判断一个字符串是否保存数字
	inline bool StringIsNumber(std::string str)
	{
		for (size_t i = 0; i < str.length(); i++)
		{
			if (str.at(i) < '0' || str.at(i) > '9')
			{
				return false;
			}
		}
		return true;
	}

	//判断文件是否存在
	inline bool IsExistFile(string csPath)
	{
		int nRet = _access(csPath.c_str(), 0);
		return 0 == nRet || EACCES == nRet;
	}

	//字符串转float
	inline float StringToFloat(string str)
	{
		return (float)atof(str.c_str());
	}



	//节点结构体  包含数据、前驱后继指针
	template<typename T>
	struct NODE
	{
		T data;
		NODE *pre;
		NODE *next;
	};


	//双向链表（带头结点)  有增删改查等基本操作
	//使用C++ 类模板  可以保存任意数据类型：基本数据类型（如 float int char char* 等）和 构造类型(如 结构体struct 类class等)
	//使用超级简单方便
	/*
		示例代码
		//对于基本数据类型
		Utility::List<int> ls;
		ls.push_back(10);//插入数据到链表尾部
		ls.push_back(20);
		ls.push_front(99);//插入数据到链表首位置

		//遍历双向链表中所有数据 输出结果为 99	10	20
		for (auto it = ls.begin();it!=ls.end();it = it->next)
		{
			cout << it->data<<'\t';
		}


		// 构造类型
		struct Person
		{
			int id;
			string name;
		};
		Utility::List<Person> ls_person;
		Person p;
		p.id = 3; p.name = "张三"; ls_person.push_back(p);
		p.id = 4; p.name = "李四"; ls_person.push_back(p);
		p.id = 5; p.name = "王五"; ls_person.push_back(p);
		//遍历双向链表中所有数据
		for (auto it = ls_person.begin();it!=ls_person.end();it = it->next)
		{
			cout << it->data.id<<'\t'<<it->data.name<<endl;
		}
		输出结果为:
		3	张三
		4	李四
		5	王五
	*/

	template<typename T>
	class List
	{
	public:
		List()
		{
			ls = new NODE<T>;
			ls->pre = NULL;
			ls->next = NULL;
			count = 0;
		}

		//链表头部加入数据
		void push_front(const T &data)
		{
			//新节点
			NODE<T> *node = new NODE<T>;
			node->data = data;


			NODE<T> *firstNode = ls->next;
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

			count++;
		}

		//链表尾部加入数据
		void push_back(const T &data)
		{
			//新节点
			NODE<T> *node = new NODE<T>;
			node->data = data;


			NODE<T> *backnode = back();
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

			count++;
		}

		//返回第一个节点指针
		NODE<T> * front()
		{
			return ls->next;
		}

		//返回第一个节点指针
		NODE<T> * begin()
		{
			return ls->next;
		}

		//返回最后一个节点指针
		NODE<T> * back()
		{
			NODE<T>* L = ls->next;
			NODE<T> *ret = NULL;
			while (L)
			{
				ret = L;
				L = L->next;
			}
			return ret;
		}

		NODE<T> * end()
		{
			return NULL;
		}

		//返回链表元素个数
		int size()
		{
			return count;
		}

		//链表是否空
		bool empty()
		{
			return count == 0;
		}

		//删除链表的某一个节点
		void erase(NODE<T> *node)
		{
			if (count == 0 || !node)
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

			count--;
		}

		//删除链表第一个节点
		void pop_front()
		{
			this->erase(this->begin());
		}

		//删除链表最后一个节点
		void pop_back()
		{
			this->erase(this->back());
		}

		//清空链表（不删除头结点）
		void clear()
		{
			while (!empty())
			{
				pop_back();
			}
		}

		//析构函数自动清理链表占用的内存空间
		~List()
		{
			this->clear();
			delete ls;
			ls = NULL;
		}
	private:
		NODE<T> * ls;
		int count;
	};
}
#endif // _INC_H_
