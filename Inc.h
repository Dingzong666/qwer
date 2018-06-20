/*
	文件名称：Inc.h
	创建日期：2018-06-20
	作者：王垠丁
	文件说明：一些字符串操作函数，全局函数
	最后修改日期：2018-06-20
	版本：0.0.1
*/
#ifndef _INC_H_
#define _INC_H_
#include <string>
#include <iostream>
#include <vector>
#include <io.h>
#include <iostream>
#include <list>
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
}
#endif // _INC_H_
