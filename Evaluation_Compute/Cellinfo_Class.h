#pragma once
#include"SQLclass.h"

class Cellinfo_Class:public SQLclass
{
public:
	Cellinfo_Class(void);
	~Cellinfo_Class(void);
	void setdefault();//�趨������ϢĬ�ϱ��� ������ �ؼ�������
	bool selectonecondition(string value,Cellinfo &cell);//��ȡ�ؼ�����ֵΪvalue��������Ϣ����cell
	bool selectall(vector<Cellinfo> &cell);
	bool FormatConver(PGresult* res,Cellinfo &cell);
	bool selectonecondition(string value,vector<Cellinfo> &cell);//��ȡ�ؼ�����ֵΪvalue��������Ϣ����cell
	bool FormatConver(PGresult* res,vector<Cellinfo> &cell);

};
