#pragma once

//数组类
template<typename T>  //C++ 模板  T是 不确定的类型 可以存任何类型的数据
class Array
{
public:
	//构造 初始化
	Array()
	{
		index = 0;
		startLenth = 10; 
		arr = (T*)malloc(sizeof(T)*startLenth);  //c语言内存分配  和new一样   arr =  new T[startLenth];
	}

	//数组添加一个数据  const 常量   &是引用符号  传递地址   t是一个数据  
	void Add(const T &t)
	{
		if (index == startLenth)
		{
			startLenth+=20;
			arr = (T*)realloc(arr,sizeof(T)*startLenth);  //和new一样  
		}

		arr[index++] = t;
	}

	
	T &operator[](int i)
	{
		return arr[i];
	}


	//返回下标的元素 
	T &at(int i)
	{
		return arr[i];
	}

	//移除
	void RemoveAt(int pos)
	{
		for (int i=pos+1;i<Size();i++)
		{
			arr[i-1] = arr[i];
		}

		index--;
	}

	//返回数组当前有多少个元素
	int Size()
	{
		return index;
	}

	//析构函数  释放内存
	~Array()
	{
		free(arr);
	}
private:
	T *arr;         //指针表示数组
	int index;		//数组下标
	int startLenth;  //数组长度
};
