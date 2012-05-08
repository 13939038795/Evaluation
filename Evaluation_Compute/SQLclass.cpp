#include "StdAfx.h"
#include "SQLclass.h"


SQLclass::SQLclass(void)
{
}

SQLclass::~SQLclass(void)
{
}


void SQLclass::SetConn(PGconn *conn){
	Vconn=conn;
}

void SQLclass::Set(const char* tablename, const char* field, const char* keyfield){
	Vtablename=(std::string)tablename;
	Vfield=(std::string)field;
	Vkeyfield=(std::string)keyfield;
	VOrderfield=(std::string)keyfield;
	VOrder=(std::string)"ASC";
}

void SQLclass::Set(const char* field, const char* keyfield){
	Vfield=(std::string)field;
	Vkeyfield=(std::string)keyfield;
}

void SQLclass::Set(const char* keyfield){
	Vkeyfield=(std::string)keyfield;
}

void SQLclass::SetOrder(const char* Orderfield, const char* Order){
	VOrderfield=(std::string)Orderfield;
	VOrder=(std::string)Order;
}

void SQLclass::SetTableName(const char* tablename){
	Vtablename=(std::string)tablename;
}

PGresult* SQLclass::VSelectOneCondition(const char* keyvalue){
	std::string query;
	if(keyvalue==NULL)
		query=(std::string)"Select "+Vfield+(std::string)" From "+Vtablename+(std::string)" Order By "+VOrderfield+(std::string)" "+VOrder;
	else
		query=(std::string)"Select "+Vfield+(std::string)" From "+Vtablename+(std::string)" Where "+Vkeyfield+(std::string)"=\'"+(std::string)keyvalue+(std::string)"\' Order By "+VOrderfield+(std::string)" "+VOrder;
	//cout<<query<<endl;
	return PQexec(Vconn,query.c_str());
}

PGresult* SQLclass::VSelectOneCondition(const char* keyvalue,std::string &query){
	if(keyvalue==NULL)
		query=(std::string)"Select "+Vfield+(std::string)" From "+Vtablename+(std::string)" Order By "+VOrderfield+(std::string)" "+VOrder;
	else
		query=(std::string)"Select "+Vfield+(std::string)" From "+Vtablename+(std::string)" Where "+Vkeyfield+(std::string)"=\'"+(std::string)keyvalue+(std::string)"\' Order By "+VOrderfield+(std::string)" "+VOrder;
	return PQexec(Vconn,query.c_str());
}

PGresult* SQLclass::VSelectTwoCondition(const char* minkeyvalue, const char* maxkeyvalue){
	std::string query=(std::string)"Select "+Vfield+(std::string)" From "+Vtablename+(std::string)" Where "+Vkeyfield+(std::string)">="+(std::string)minkeyvalue+(std::string)" And "+Vkeyfield+(std::string)"<="+(std::string)maxkeyvalue +(std::string)"  Order By "+VOrderfield+(std::string)" "+VOrder;
	return PQexec(Vconn,query.c_str());
}
PGresult* SQLclass::VSelect_avg(string field,string keyvalue){
	string query;
	query=(string)"SELECT AVG("+field+(string)") FROM "+Vtablename+(std::string)" Where "+Vkeyfield+(std::string)"="+(string)"\'"+keyvalue+(string)"\'";
	return PQexec(Vconn,query.c_str());
}