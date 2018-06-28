/*
�ļ����ƣ�Inc.h
�������ڣ�2018-06-28
���ߣ�����
�ļ�˵����һЩ�ַ������������� ˫������
����޸����ڣ�2018-06-28
�汾��0.0.7
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
	//�ַ����и�
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

	//ȥ���ַ����Ŀո�
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

	//�ж�һ���ַ����Ƿ񱣴�����
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

	//�ж��ļ��Ƿ����
	inline bool IsExistFile(string csPath)
	{
		int nRet = _access(csPath.c_str(), 0);
		return 0 == nRet || EACCES == nRet;
	}

	//�ַ���תfloat
	inline float StringToFloat(string str)
	{
		return (float)atof(str.c_str());
	}



	//�ڵ�ṹ��  �������ݡ�ǰ�����ָ��
	template<typename T>
	struct NODE
	{
		T data;
		NODE *pre;
		NODE *next;
	};


	//˫��������ͷ���)  ����ɾ�Ĳ�Ȼ�������
	//ʹ��C++ ��ģ��  ���Ա��������������ͣ������������ͣ��� float int char char* �ȣ��� ��������(�� �ṹ��struct ��class��)
	//ʹ�ó����򵥷���
	/*
		ʾ������
		//���ڻ�����������
		Utility::List<int> ls;
		ls.push_back(10);//�������ݵ�����β��
		ls.push_back(20);
		ls.push_front(99);//�������ݵ�������λ��

		//����˫���������������� ������Ϊ 99	10	20
		for (auto it = ls.begin();it!=ls.end();it = it->next)
		{
			cout << it->data<<'\t';
		}


		// ��������
		struct Person
		{
			int id;
			string name;
		};
		Utility::List<Person> ls_person;
		Person p;
		p.id = 3; p.name = "����"; ls_person.push_back(p);
		p.id = 4; p.name = "����"; ls_person.push_back(p);
		p.id = 5; p.name = "����"; ls_person.push_back(p);
		//����˫����������������
		for (auto it = ls_person.begin();it!=ls_person.end();it = it->next)
		{
			cout << it->data.id<<'\t'<<it->data.name<<endl;
		}
		������Ϊ:
		3	����
		4	����
		5	����
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

		//����ͷ����������
		void push_front(const T &data)
		{
			//�½ڵ�
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

		//����β����������
		void push_back(const T &data)
		{
			//�½ڵ�
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

		//���ص�һ���ڵ�ָ��
		NODE<T> * front()
		{
			return ls->next;
		}

		//���ص�һ���ڵ�ָ��
		NODE<T> * begin()
		{
			return ls->next;
		}

		//�������һ���ڵ�ָ��
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

		//��������Ԫ�ظ���
		int size()
		{
			return count;
		}

		//�����Ƿ��
		bool empty()
		{
			return count == 0;
		}

		//ɾ�������ĳһ���ڵ�
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

		//ɾ�������һ���ڵ�
		void pop_front()
		{
			this->erase(this->begin());
		}

		//ɾ���������һ���ڵ�
		void pop_back()
		{
			this->erase(this->back());
		}

		//���������ɾ��ͷ��㣩
		void clear()
		{
			while (!empty())
			{
				pop_back();
			}
		}

		//���������Զ���������ռ�õ��ڴ�ռ�
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
