#pragma once
#include"SQLclass.h"

class Cellinfo_Class:public SQLclass
{
public:
	Cellinfo_Class(void);
	~Cellinfo_Class(void);
	void setdefault();//设定天线信息默认表名 属性名 关键属性名
	bool selectonecondition(string value,Cellinfo &cell);//读取关键属性值为value的天线信息存入cell
	bool selectall(vector<Cellinfo> &cell);
	bool FormatConver(PGresult* res,Cellinfo &cell);
	bool selectonecondition(string value,vector<Cellinfo> &cell);//读取关键属性值为value的天线信息存入cell
	bool FormatConver(PGresult* res,vector<Cellinfo> &cell);

};
