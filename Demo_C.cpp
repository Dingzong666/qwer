#include "Inc_C.h"
//����C���Ĵ���
/*
�ļ����ƣ�demo.cpp
�������ڣ�2018-06-28
���ߣ�����
�ļ�˵�������ʹ�����WagesManager�Ϳ���̨�������
����޸����ڣ�2018-06-28
�汾��0.1.0
*/

//ʹ��C���汾������

#include "Inc.h"


void PrintInfo(const WorkerInfo &wi)
{
	cout << "-----------------------------------------------------------------------------\r\n";
	cout << "���ţ�" << wi.id << " ������" << wi.name << endl;
	cout << "��λ���ʣ�" <<wi.postWages << " н�����ʣ�" << wi.paySalary << " ְ�������" << wi.jobAllowance << " ��Ч���ʣ�" << wi.performancePay << endl;
	cout << "Ӧ�����ʣ�" << wi.shouldPay << " ��������˰��" << wi.tax << " ʵ�����ʣ�" << wi.realWages << endl;
	cout << "-----------------------------------------------------------------------------\r\n";
}



//���ʹ�����
//ֻ�������ݵĲ�ѯ���޸ġ���ӡ�ɾ�������桢��ʾ����������
//���漰���û�����Ĳ���  �����߶ȷ���
//������ɾ�� ��� �޸Ĳ���ʱ��ʱ�������ݵ��ļ�
class WagesManager
{
public:
	WagesManager()
	{
		CreateList(&ls);
		this->Read();
	}

	//�Ƿ���ڸ�Ա��  �����������ݵ�ʱ���ж�ʹ��
	bool IsExistWorker(string id)
	{
		return this->Get(id) != end(ls);
	}

	//4.1��ȡְ���������ݺ�����read()
	void Read()
	{
		clear(ls);
		if (Utility::IsExistFile("gx.txt"))  //�����ļ�����
		{
			fstream fs;
			fs.open("gx.txt", ios::in | ios::out);
			char buff[1024] = { 0 };
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

				push_back(ls,wi);
			}
			fs.close();
		}
	}
	//4.2����ְ���������ݺ�����write()
	void Save()
	{   
		ofstream fs("gx.txt");  //һ����д�뵽�����ļ� �Ḳ�Ǿ�����
		for (auto it = begin(ls); it != end(ls); it = it->next)
		{
			fs << it->data.id << "=" << it->data.name << "=" << it->data.postWages << "=" << it->data.paySalary << "=" <<
				it->data.jobAllowance << "=" << it->data.performancePay << "=" << it->data.shouldPay << "=" << it->data.tax << "=" << it->data.realWages << endl;
		}
		fs.close();
	}


	//4.3��ѯְ���������ݺ�����find()
	void Find(const string &id)
	{
		auto result = this->Get(id);
		if (result != end(ls))
			PrintInfo(result->data);
	}
	//4.4���ְ���������ݺ�����list()
	void ListInfo()
	{
		for (auto it = begin(ls); it != end(ls); it = it->next)
		{
			PrintInfo(it->data);
		}
	}
	//4.5�޸�ְ���������ݺ�����modify()
	//ֻ��Ҫ�޸���4�����ԣ���λ���ʡ�н�����ʡ�ְ���������Ч���� ����
	//Ӧ�����ʡ���������˰��ʵ�����ʻ��Զ�����
	void Modify(const string &id, float postWages, float paySalary, float jobAllowance, float performancePay)
	{
		auto result = this->Get(id);
		if (result != end(ls))
		{
			result->data.postWages = postWages;
			result->data.paySalary = paySalary;
			result->data.jobAllowance = jobAllowance;
			result->data.performancePay = performancePay;
			CalcTax(id);  //���¼���Ӧ�����ʡ���������˰��ʵ������

			Save(); //�����ݽ������޸Ĳ��� Ҫ��ʱ���浽�����ļ�
		}
	}

	//4.6ɾ��ְ���������ݺ�����del()
	void Delete(const string &id)
	{
		auto result = this->Get(id);
		if (result != end(ls))     //���ھ�ִ��˫�������eraseɾ������ 
		{
			erase(ls,result);

			Save(); //�����ݽ������޸Ĳ��� Ҫ��ʱ���浽�����ļ�
		}
	}
	//4.7���ְ���������ݺ�����add()
	//// ����  ���� ��λ����   н������ ְ����� ��Ч����)
	bool Add(const string &id, const string &name, float postWages, float paySalary, float jobAllowance, float performancePay)
	{
		if (!Utility::StringIsNumber(id))  //id���汣���������ַ� ����ȫ���� 
			return false;

		auto result = this->Get(id);
		if (result == end(ls))  //���������ID������ӣ������ظ�
		{
			WorkerInfo wi;
			wi.id = id;								//����
			wi.name = name;							//����
			wi.postWages = postWages;				//��λ���� 
			wi.paySalary = paySalary;				//н������
			wi.jobAllowance = jobAllowance;			//ְ�����
			wi.performancePay = performancePay;     //��Ч����

			push_back(ls,wi);			//���뵽˫������β��
			this->CalcTax(id);				//����˰�ʵ�

			Save(); //�����ݽ�������Ӳ��� Ҫ��ʱ���浽�����ļ�
			return true;
		}
		return false;
	}


	//4.8�����������˰������grsds()
	//������˰��Ϊ��׼
	//Ӧ�ɸ�������˰˰��=Ӧ��˰���ö������˰��-����۳���

	//����	Ӧ��˰���ö�(��˰)			Ӧ��˰���ö�(����˰)			˰��(%)	   ����۳���
	//1	������1500Ԫ��					������1455Ԫ��					3			0
	//2	����1500Ԫ��4,500Ԫ�Ĳ���		����1455Ԫ��4,155Ԫ�Ĳ���		10			105
	//3	����4,500Ԫ��9,000Ԫ�Ĳ���		����4,155Ԫ��7,755Ԫ�Ĳ���		20			555
	//4	����9,000Ԫ��35,000Ԫ�Ĳ���		����7,755Ԫ��27,255Ԫ�Ĳ���		25			1,005
	//5	����35,000Ԫ��55,000Ԫ�Ĳ���	����27,255Ԫ��41,255Ԫ�Ĳ���	30			2,755
	//6	����55,000Ԫ��80,000Ԫ�Ĳ���	����41,255Ԫ��57,505Ԫ�Ĳ���	35			5,505
	//7	����80,000Ԫ�Ĳ���				����57,505�Ĳ���				45			13,505
	
	void CalcTax(const string &id)
	{
		auto w = this->Get(id);
		if (w != end(ls))
		{
			//Ӧ������ = ��λ���� +н������+ְ�����+��Ч����
			w->data.shouldPay = w->data.postWages + w->data.paySalary + w->data.jobAllowance + w->data.performancePay;

			//3500������
			float a = w->data.shouldPay - 3500;

			//����3500
			if (a <= 0)
			{
				w->data.tax = 0;
				w->data.realWages = w->data.shouldPay;
			}
			else
			{
				//Ӧ�ɸ�������˰˰��=Ӧ��˰���ö������˰��-����۳���
				if (a > 0 && a <= 1500)
					w->data.tax = a * 0.03f - 0;
				else if (a > 1500 && a <= 4500)
					w->data.tax = a * 0.1f - 105;
				else if (a > 4500 && a <= 9000)
					w->data.tax = a * 0.2f - 555;
				else if (a > 9000 && a <= 35000)
					w->data.tax = a * 0.25f - 1005;
				else if (a > 35000 && a <= 55000)
					w->data.tax = a * 0.3f - 2755;
				else if (a > 55000 && a <= 80000)
					w->data.tax = a * 0.35f - 5505;
				else
					w->data.tax = a * 0.45f - 13505;

				//ʵ�չ��� = Ӧ������ - ˰
				w->data.realWages = w->data.shouldPay - w->data.tax;
			}
		}
	}
private:
	//ͨ��ְ������id����˫�����������ָ��
	NODE *Get(string id)
	{
		NODE *result = end(ls);
		for (auto it = begin(ls); it != end(ls); it = it->next)
		{
			if (id == it->data.id)
			{
				result = it;
				break;
			}
		}
		return result;
	}

	//˫��������ְ����Ϣ
	NODE *ls;
};






//�������û��������  
void ShowMainMenu()
{
	system("cls");
	cout<<"\t###	��ӭʹ�ù��������ѧ�������Ϣ��ȫְ�����ʹ���ϵͳ	###"<<endl<<endl;
	cout<<"\t��ѡ��<1 -- 7>: "<<endl;
	cout<<"\t============================================================="<<endl;
	cout<<"\t|\t1����ѯְ�����ʼ�¼	                      	    |"<<endl;
	cout<<"\t|\t2���޸�ְ�����ʼ�¼	                      	    |"<<endl;
	cout<<"\t|\t3�����ְ�����ʼ�¼	                      	    |"<<endl;
	cout<<"\t|\t4��ɾ��ְ�����ʼ�¼	                      	    |"<<endl;
	cout<<"\t|\t5���������ݵ��ļ�	                      	    |"<<endl;
	cout<<"\t|\t6�����ְ����¼		                      	    |"<<endl;
	cout<<"\t|\t7���˳�ϵͳ	                      	            |"<<endl;
	cout<<"\t============================================================="<<endl<<endl;
	cout<<"\t���ѡ���ǣ�";
}



int MainLoop()
{
	WagesManager wm;	//��������ʱ���ù��캯���Զ���ȡ�����ļ�
	while (true)
	{
		ShowMainMenu();
		string id;				//����
		string name;			//����
		float postWages;		//��λ���� 
		float paySalary;		//н������
		float jobAllowance;     //ְ�����
		float performancePay;   //��Ч����

		string isDelete;		//ȷ���Ƿ�ɾ��

		int flag = -1;
		cin >> flag;
		switch (flag)
		{
		case 1:			//1����ѯ
			cout << "�������ѯ�Ĺ���" << endl;
			cin >> id;
			if (!wm.IsExistWorker(id))
			{
				cout << "�����ڸ�Ա��,��ѯʧ��" << endl;
				break;
			}
			wm.Find(id);
			break;
		case 2:			//2���޸�
			cout << "�������޸ĵĹ���" << endl;
			cin >> id;
			if (!wm.IsExistWorker(id))
			{
				cout << "�����ڸ�Ա��,�޸�ʧ��" << endl;
				break;
			}
			cout << "�����������λ����" << endl;
			cin >> postWages;
			cout << "����������н������" << endl;
			cin >> paySalary;
			cout << "����������ְ�����" << endl;
			cin >> jobAllowance;
			cout << "���������뼨Ч����" << endl;
			cin >> performancePay;
			wm.Modify(id, postWages, paySalary, jobAllowance, performancePay);
			break;
		case 3:			//3�����
			cout << "��������ӵĹ���" << endl;
			cin >> id;
			if (wm.IsExistWorker(id))
			{
				cout << "���ʧ��,Ա��ID�Ѿ�����" << endl;
				break;
			}
			cout << "��������ӵ�����" << endl;
			cin >> name;
			cout << "�������λ����" << endl;
			cin >> postWages;
			cout << "������н������" << endl;
			cin >> paySalary;
			cout << "������ְ�����" << endl;
			cin >> jobAllowance;
			cout << "�����뼨Ч����" << endl;
			cin >> performancePay;

			wm.Add(id, name, postWages, paySalary, jobAllowance, performancePay);
			break;
		case 4:		//4��ɾ��
			cout << "������ɾ���Ĺ���" << endl;
			cin >> id;
			if (wm.IsExistWorker(id))
			{
				cout << "ȷ��ɾ����Y\tȡ��ɾ��N" << endl;
				cin >> isDelete;
				if (isDelete == "Y" || isDelete == "y")
				{
					wm.Delete(id);
					cout << "ɾ���ɹ�" << endl;
				}
			}
			else
				cout << "ɾ��ʧ�ܣ������ڸ�Ա��" << endl;

			break;
		case 5:		//5������
			wm.Save();
			break;
		case 6:		//6�����������Ϣ
			wm.ListInfo();
			break;
		case 7:		//7���˳�����
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