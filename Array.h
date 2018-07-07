#pragma once

//������
template<typename T>  //C++ ģ��  T�� ��ȷ�������� ���Դ��κ����͵�����
class Array
{
public:
	//���� ��ʼ��
	Array()
	{
		index = 0;
		startLenth = 10; 
		arr = (T*)malloc(sizeof(T)*startLenth);  //c�����ڴ����  ��newһ��   arr =  new T[startLenth];
	}

	//�������һ������  const ����   &�����÷���  ���ݵ�ַ   t��һ������  
	void Add(const T &t)
	{
		if (index == startLenth)
		{
			startLenth+=20;
			arr = (T*)realloc(arr,sizeof(T)*startLenth);  //��newһ��  
		}

		arr[index++] = t;
	}

	
	T &operator[](int i)
	{
		return arr[i];
	}


	//�����±��Ԫ�� 
	T &at(int i)
	{
		return arr[i];
	}

	//�Ƴ�
	void RemoveAt(int pos)
	{
		for (int i=pos+1;i<Size();i++)
		{
			arr[i-1] = arr[i];
		}

		index--;
	}

	//�������鵱ǰ�ж��ٸ�Ԫ��
	int Size()
	{
		return index;
	}

	//��������  �ͷ��ڴ�
	~Array()
	{
		free(arr);
	}
private:
	T *arr;         //ָ���ʾ����
	int index;		//�����±�
	int startLenth;  //���鳤��
};
