/*
	文件名称：Demo.cpp
	创建日期：2018-06-20
	作者：王垠丁
	文件说明：数据操作类WagesManager和控制台界面操作
	最后修改日期：2018-06-20
	版本：0.0.1
*/

#include "Inc.h"

//职工信息结构体
//工号 + 姓名 + 岗位工资 + 薪级工资 + 职务津贴 + 绩效工资 + 应发工资 + 个人所得税 + 实发工资
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

	void Print()
	{
		cout << "-----------------------------------------------------------------------------\r\n";
		cout << "工号：" << id << " 姓名：" << name << endl;
		cout << "岗位工资：" << postWages << " 薪级工资：" << paySalary << " 职务津贴：" << jobAllowance << " 绩效工资：" << performancePay << endl;
		cout << "应发工资：" << shouldPay << " 个人所得税：" << tax << " 实发工资：" << realWages << endl;
		cout << "-----------------------------------------------------------------------------\r\n";
	}
};




//工资管理类
//只包含数据的查询、修改、添加、删除、保存、显示、输入数据
//不涉及与用户界面的操作  与界面高度分离
//对数据删除 添加 修改操作时及时更新数据到文件
class WagesManager
{
public:
	WagesManager()
	{
		this->Read();
	}

	//是否存在该员工  方便输入数据的时候判断使用
	bool IsExistWorker(string id)
	{
		return this->Get(id) != _works.end();
	}

	//4.1读取职工工资数据函数：read()
	void Read()
	{
		_works.clear();
		if (Utility::IsExistFile("gx.txt"))  //数据文件存在
		{
			fstream fs;
			fs.open("gx.txt", ios::in | ios::out);
			char buff[1024] = {0};
			while (fs.getline(buff, 1024))   //读取行
			{
				auto vec = Utility::SplitString(buff, "=");   //字符串分割
	
				WorkerInfo wi;
				wi.id = vec.at(0);								//工号
				wi.name = vec.at(1);							//姓名
				wi.postWages = Utility::StringToFloat(vec.at(2));			//岗位工资 
				wi.paySalary = Utility::StringToFloat(vec.at(3));			//薪级工资
				wi.jobAllowance = Utility::StringToFloat(vec.at(4));		//职务津贴
				wi.performancePay = Utility::StringToFloat(vec.at(5));    //绩效工资
				wi.shouldPay = Utility::StringToFloat(vec.at(6));			//应发工资
				wi.tax = Utility::StringToFloat(vec.at(7));				//个人所得税
				wi.realWages = Utility::StringToFloat(vec.at(8));			//实发工资

				_works.push_back(wi);
			}
			fs.close();
		}
	}

	