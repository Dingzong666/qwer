/*
文件名称：demo.cpp
创建日期：2018-07-06
作者：王垠丁
文件说明：工资管理类WagesManager和控制台界面操作
最后修改日期：2018-07-04
版本：0.1.4
*/

//使用C风格版本的链表

#include "Inc.h"
#include "Array.h"
#include <Windows.h>
#include <string>
#include <iostream>
using namespace std;


//结构体
struct WorkerInfo
{
	//这6个属性是要求用户输入的 
	char id[10];			//工号
	char name[20];			//姓名
	float postWages;		//岗位工资 
	float paySalary;		//薪级工资
	float jobAllowance;     //职务津贴
	float performancePay;   //绩效工资


	//这3个属性是根据上面的自动计算 
	float shouldPay;		//应发工资
	float tax;				//个人所得税
	float realWages;		//实发工资
};


void PrintInfo(const WorkerInfo &wi)
{
	cout << "-----------------------------------------------------------------------------\r\n";
	cout << "工号：" << wi.id << " 姓名：" << wi.name << endl;
	cout << "岗位工资：" <<wi.postWages << " 薪级工资：" << wi.paySalary << " 职务津贴：" << wi.jobAllowance << " 绩效工资：" << wi.performancePay << endl;
	cout << "应发工资：" << wi.shouldPay << " 个人所得税：" << wi.tax << " 实发工资：" << wi.realWages << endl;
	cout << "-----------------------------------------------------------------------------\r\n";
}



//工资管理类
//只包含数据的查询、修改、添加、删除、保存、显示、输入数据
//不涉及与用户界面的操作  与界面高度分离
//对数据删除 添加 修改操作时及时更新数据到文件
class WagesManager
{
public:
	WagesManager()
	{
		Read();  //this 是本类的指针  指向自己
	}

	/*
		this->Get(id)  根据员工编号返回链表节点指针 
	*/

	//是否存在该员工  方便输入数据的时候判断使用
	bool IsExistWorker(string id)
	{
		return this->Get(id) != NULL;
	}

	//4.1读取职工工资数据函数：read()
	void Read()
	{
		//从文件中读取数据
		if (Utility::IsExistFile("gx.txt"))  //数据文件存在
		{
			WorkerInfo wi;
			FILE *pFile = fopen("gx.txt","r");  //打开文件 read   返回一个文件句柄pFile  FILE *是文件指针 
			while(fread(&wi,sizeof(wi),1,pFile))  //循环读取  fread(&wi,sizeof(wi),1,pFile)    取wi地址 sizeof计算某个东西的大小
			{
				arr.Add(wi);
			}
			fclose(pFile);  //file close 
		}
	}


	//4.2保存职工工资数据函数：write()  保存
	void Save()
	{   
		DeleteFileA("gx.txt");  //删除
		FILE *pFile = fopen("gx.txt","a+");  //write （a+） 是追加  如果写入结构体 自动二进制保存  a+  = append
		
		for (int i = 0; i<arr.Size(); i++)
		{
			fwrite(&arr.at(i),sizeof(arr.at(i)),1,pFile);  //往文件写一个员工信息
		}
		fclose(pFile);
	}


	//4.3查询职工工资数据函数：find()
	void Find(const string &id)
	{
		auto result = this->Get(id);  //auto 是自动类型推导
		if (result != NULL)
			PrintInfo(*result);
	}


	//4.4浏览职工工资数据函数：list()
	void ListInfo()
	{
		for (int i = 0; i<arr.Size(); i++)
		{
			PrintInfo(arr.at(i));
		}
	}


	//4.5修改职工工资数据函数：modify()
	//只需要修改这4个属性：岗位工资、薪级工资、职务津贴、绩效工资 即可
	//应发工资、个人所得税、实发工资会自动计算
	void Modify(const string &id, float postWages, float paySalary, float jobAllowance, float performancePay)
	{
		auto result = this->Get(id);
		if (result != NULL)
		{
			result->postWages = postWages;  //赋值
			result->paySalary = paySalary;
			result->jobAllowance = jobAllowance;
			result->performancePay = performancePay;
			CalcTax(id);  //重新计算应发工资、个人所得税、实发工资

			Save(); //对数据进行了修改操作 要及时保存到数据文件
		}
	}

	//4.6删除职工工资数据函数：del()
	void Delete(const string &id)
	{
		int index= -1;
		for (int i=0;i<arr.Size();i++)
		{
			if (arr[i].id == id)
			{
				index = i;
				break;
			}
		}
	
		if (index != -1)   
		{
			arr.RemoveAt(index);
			Save(); //对数据进行了修改操作 要及时保存到数据文件
		}
	}

	//1212112 fassf
	//4.7添加职工工资数据函数：add()
	//// 工号  姓名 岗位工资   薪级工资 职务津贴 绩效工资)
	bool Add(const string &id, const string &name, float postWages, float paySalary, float jobAllowance, float performancePay)
	{
		if (!Utility::StringIsNumber(id))  //id里面保存了其他字符 不是全数字 
			return false;

		auto result = this->Get(id);
		if (result == NULL)  //不存在这个ID才能添加，避免重复
		{
			WorkerInfo wi;
			strcpy(wi.id,id.c_str());				//工号
			strcpy(wi.name,name.c_str());			//姓名
			wi.postWages = postWages;				//岗位工资 
			wi.paySalary = paySalary;				//薪级工资
			wi.jobAllowance = jobAllowance;			//职务津贴
			wi.performancePay = performancePay;     //绩效工资

			CalcTax(id);				//计算税率等
			arr.Add(wi);				//插入到双向链表尾部

			Save(); //对数据进行了添加操作 要及时保存到数据文件
			return true;
		}
		return false;
	}


		//4.8计算个人所得税函数：grsds()
	//以现行税率为标准
	//应纳个人所得税税额=应纳税所得额×适用税率

	//级数	应纳税所得额(含税)				 税率(%)	   
	//1	不超过500元的							5			
	//2	超过500元至2000元的部分				    10			
	//3	超过2000元至5000元的部分				15			
	//4	超过5000元至200000元的部分				20			
	//5	超过20000元至40000元的部分		        25			
	//6	超过40000元至60000元的部分       	    30			
	//7	超过60000元至80000元的部分      	    35			
	//8	超过80000元至100000元的部分     	    40			
	//9 超过100000的部分                        45           
	
	void CalcTax(const string &id)
	{
		auto w = this->Get(id);
		if (w != NULL)
		{
			//应发工资 = 岗位工资 +薪级工资+职务津贴+绩效工资
			w->shouldPay = w->postWages + w->paySalary + w->jobAllowance + w->performancePay;

			//3500起增点
			float a = w->shouldPay - 3500;

			//不够3500
			if (a <= 0)
			{
				w->tax = 0;
				w->realWages = w->shouldPay;
			}
			else
			{
				//
				////所得税税额=应纳税所得额×适用税率
				if (a > 0 && a <= 500)
					w->tax = a * 0.05f;
				else if (a > 500 && a <= 2000)
					w->tax = a * 0.1 ;
				else if (a > 2000 && a <= 5000)
					w->tax = a * 0.15 ;
				else if (a > 5000 && a <= 20000)
					w->tax = a * 0.2;
				else if (a > 20000 && a <= 40000)
					w->tax = a * 0.25;
				else if (a > 40000 && a <= 60000)
					w->tax = a * 0.3;
				else if (a > 60000 && a <= 80000)
					w->tax = a * 0.35;
				else if (a > 80000 && a <= 100000)
					w->tax = a * 0.4;
				else if (a>100000)
					w->tax = a * 0.45;

				//实收工资 = 应付工资 - 税
				w->realWages = w->shouldPay - w->tax;
			}
		}
	}
private:
	//通过职工工号id返回数组元素的指针
	// WorkerInfo*  是员工结构体指针 指向员工信息 
	WorkerInfo*  Get(string id)
	{
		for (int i = 0; i<arr.Size(); i++)
		{
			if (id == arr[i].id)
			{
				return &arr[i]; //&取地址  返回员工信息结构体指针  指针指向员工信息
			}
		}
		return NULL;
	}

	//数组保存职工信息
	Array<WorkerInfo> arr;
};






//下面是用户界面操作  
void ShowMainMenu()
{
	system("cls");
	cout<<"\t###	欢迎使用广西民族大学软件与信息安全职工工资管理系统	###"<<endl<<endl;
	cout<<"\t请选择<1 -- 7>: "<<endl;
	cout<<"\t============================================================="<<endl;
	cout<<"\t：\t1、查询职工工资记录	                      	    ："<<endl;
	cout<<"\t：\t2、修改职工工资记录	                      	    ："<<endl;
	cout<<"\t：\t3、添加职工工资记录	                      	    ："<<endl;
	cout<<"\t：\t4、删除职工工资记录	                      	    ："<<endl;
	cout<<"\t：\t5、保存数据到文件	                      	    ："<<endl;
	cout<<"\t：\t6、浏览职工记录		                      	    ："<<endl;
	cout<<"\t：\t7、退出系统	                      	            ："<<endl;
	cout<<"\t============================================================="<<endl<<endl;
	cout<<"\t你的选择是：";
}



int MainLoop()
{
	WagesManager wm;	//创建对象时调用构造函数自动读取数据文件
	
	while (true)
	{
		ShowMainMenu();
		string id;				//工号
		string name;			//姓名
		float postWages;		//岗位工资 
		float paySalary;		//薪级工资
		float jobAllowance;     //职务津贴
		float performancePay;   //绩效工资

		string isDelete;		//确认是否删除

		int flag = -1;
		cin >> flag;
		switch (flag)
		{
		case 1:			//1、查询
			cout << "请输入查询的工号" << endl;
			cin >> id;
			if (!wm.IsExistWorker(id))
			{
				cout << "不存在该员工,查询失败" << endl;
				break;
			}
			wm.Find(id);
			break;
		case 2:			//2、修改
			cout << "请输入修改的工号" << endl;
			cin >> id;
			if (!wm.IsExistWorker(id))
			{
				cout << "不存在该员工,修改失败" << endl;
				break;
			}
			cout << "请重新输入岗位工资" << endl;
			cin >> postWages;
			cout << "请重新输入薪级工资" << endl;
			cin >> paySalary;
			cout << "请重新输入职务津贴" << endl;
			cin >> jobAllowance;
			cout << "请重新输入绩效工资" << endl;
			cin >> performancePay;
			wm.Modify(id, postWages, paySalary, jobAllowance, performancePay);
			break;
		case 3:			//3、添加
			cout << "请输入添加的工号" << endl;
			cin >> id;
			if (wm.IsExistWorker(id))
			{
				cout << "添加失败,员工ID已经存在" << endl;
				break;
			}
			cout << "请输入添加的姓名" << endl;
			cin >> name;
			cout << "请输入岗位工资" << endl;
			cin >> postWages;
			cout << "请输入薪级工资" << endl;
			cin >> paySalary;
			cout << "请输入职务津贴" << endl;
			cin >> jobAllowance;
			cout << "请输入绩效工资" << endl;
			cin >> performancePay;

			wm.Add(id, name, postWages, paySalary, jobAllowance, performancePay);
			break;
		case 4:		//4、删除
			cout << "请输入删除的工号" << endl;
			cin >> id;
			if (wm.IsExistWorker(id))
			{
				cout << "确认删除：Y\t取消删除N" << endl;
				cin >> isDelete;
				if (isDelete == "Y" || isDelete == "y")
				{
					wm.Delete(id);
					cout << "删除成功" << endl;
				}
			}
			else
				cout << "删除失败，不存在该员工" << endl;

			break;
		case 5:		//5、保存
			wm.Save();
			break;
		case 6:		//6、浏览所有信息
			wm.ListInfo();
			break;
		case 7:		//7、退出程序
			return 0;
		default:
			break;
		}
		system("pause");
	}
	return 0;
}

int main()
{
	return MainLoop();
}
