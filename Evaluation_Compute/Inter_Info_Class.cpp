#include "StdAfx.h"
#include "Inter_Info_Class.h"

Inter_Info_Class::Inter_Info_Class(void)
{
}

Inter_Info_Class::~Inter_Info_Class(void)
{
}
void Inter_Info_Class::SetDefault(){
	Set(default_inter_tablename,default_inter_field,default_inter_keyfield);
}
bool Inter_Info_Class::SelectTwoCondition(string minkeyvalue,string maxkeyvalue,vector<Grid_Inter>&gridinter){
	//PGresult* res=VSelectTwoCondition(minkeyvalue.c_str(),maxkeyvalue.c_str());
	string query=(std::string)"Select "+Vfield+(std::string)" From "+Vtablename+(std::string)" Where "+Vkeyfield+(std::string)">="+(std::string)minkeyvalue+(std::string)" And "+Vkeyfield+(std::string)"<="+(std::string)maxkeyvalue +(std::string)"  Order By "+VOrderfield+(std::string)" "+VOrder;
//	cout<<query<<endl;
	PGresult* res=PQexec(Vconn,query.c_str());
	int num=PQntuples(res);
	if(num<1)
		return false;
	gridinter.clear();
	gridinter.resize(num);
	for(int i=0;i<num;i++){
		gridinter[i].grid_id=atoi(PQgetvalue(res,i,0));
		gridinter[i].Same_internum=atoi(PQgetvalue(res,i,1));
		gridinter[i].Same_interpower=(float)atof(PQgetvalue(res,i,2));
		gridinter[i].Diff_internum=atoi(PQgetvalue(res,i,3));
		gridinter[i].Diff_interpower=(float)atof(PQgetvalue(res,i,4));
	}
	PQclear(res);
	return true;
}