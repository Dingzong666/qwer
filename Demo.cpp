/*
	�ļ����ƣ�Demo.cpp
	�������ڣ�2018-06-21
	���ߣ�����
	�ļ�˵�������ݲ�����WagesManager�Ϳ���̨�������
	����޸����ڣ�2018-06-20
	�汾��0.0.3
*/

#include "Inc.h"

//ְ����Ϣ�ṹ��
//���� + ���� + ��λ���� + н������ + ְ����� + ��Ч���� + Ӧ������ + ��������˰ + ʵ������
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

	void Print()
	{
		cout << "-----------------------------------------------------------------------------\r\n";
		cout << "���ţ�" << id << " ������" << name << endl;
		cout << "��λ���ʣ�" << postWages << " н�����ʣ�" << paySalary << " ְ�������" << jobAllowance << " ��Ч���ʣ�" << performancePay << endl;
		cout << "Ӧ�����ʣ�" << shouldPay << " ��������˰��" << tax << " ʵ�����ʣ�" << realWages << endl;
		cout << "-----------------------------------------------------------------------------\r\n";
	}
};




//���ʹ�����
//ֻ�������ݵĲ�ѯ���޸ġ���ӡ�ɾ�������桢��ʾ����������
//���漰���û�����Ĳ���  �����߶ȷ���
//������ɾ�� ��� �޸Ĳ���ʱ��ʱ�������ݵ��ļ�
class WagesManager
{
public:
	WagesManager()
	{
		this->Read();
	}

	//�Ƿ���ڸ�Ա��  �����������ݵ�ʱ���ж�ʹ��
	bool IsExistWorker(string id)
	{
		return this->Get(id) != _works.end();
	}

	//4.1��ȡְ���������ݺ�����read()
	void Read()
	{
		_works.clear();
		if (Utility::IsExistFile("gx.txt"))  //�����ļ�����
		{
			fstream fs;
			fs.open("gx.txt", ios::in | ios::out);
			char buff[1024] = {0};
			while (fs.getline(buff, 1024))   //��ȡ��
			{
				auto vec = Utility::SplitString(buff, "=");   //�ַ����ָ�
	
				WorkerInfo wi;
				wi.id = vec.at(0);								//����
				wi.name = vec.at(1);							//����
				wi.postWages = Utility::StringToFloat(vec.at(2));			//��λ���� 
				wi.paySalary = Utility::StringToFloat(vec.at(3));			//н������
				wi.jobAllowance = Utility::StringToFloat(vec.at(4));		//ְ�����
				wi.performancePay = Utility::StringToFloat(vec.at(5));    //��Ч����
				wi.shouldPay = Utility::StringToFloat(vec.at(6));			//Ӧ������
				wi.tax = Utility::StringToFloat(vec.at(7));				//��������˰
				wi.realWages = Utility::StringToFloat(vec.at(8));			//ʵ������

				_works.push_back(wi);
			}
			fs.close();
		}
	}
	//4.2����ְ���������ݺ�����write()
	void Save()
	{
		ofstream fs("gx.txt");  //һ����д�뵽�����ļ� �Ḳ�Ǿ�����
		for (auto it = _works.begin(); it != _works.end(); it++)
		{
			fs << it->id << "=" << it->name << "=" << it->postWages << "=" << it->paySalary << "=" <<
				it->jobAllowance << "=" << it->performancePay << "=" << it->shouldPay << "=" << it->tax << "=" << it->realWages << endl;
		}
		fs.close();
	}


	//4.3��ѯְ���������ݺ�����find()
	void Find(const string &id)
	{
		auto result = this->Get(id);
		if (result != _works.end())
			result->Print();
	}
	//4.4���ְ���������ݺ�����list()
	void ListInfo()
	{
		for (auto it = _works.begin(); it != _works.end(); it++)
		{
			it->Print();
		}
	}
	//4.5�޸�ְ���������ݺ�����modify()
	//ֻ��Ҫ�޸���4�����ԣ���λ���ʡ�н�����ʡ�ְ���������Ч���� ����
	//Ӧ�����ʡ���������˰��ʵ�����ʻ��Զ�����
	void Modify(const string &id, float postWages, float paySalary, float jobAllowance, float performancePay)
	{
		auto result = this->Get(id);
		if (result != _works.end())
		{
			result->postWages = postWages;
			result->paySalary = paySalary;
			result->jobAllowance = jobAllowance;
			result->performancePay = performancePay;
			CalcTax(id);  //���¼���Ӧ�����ʡ���������˰��ʵ������

			Save(); //�����ݽ������޸Ĳ��� Ҫ��ʱ���浽�����ļ�
		}
	}

	//4.6ɾ��ְ���������ݺ�����del()
	void Delete(const string &id)
	{
		auto result = this->Get(id);
		if (result != _works.end())     //���ھ�ִ��˫�������eraseɾ������
		{
			_works.erase(result);

			Save(); //�����ݽ������޸Ĳ��� Ҫ��ʱ���浽�����ļ�
		}
	}

	