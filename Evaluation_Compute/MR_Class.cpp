#include "StdAfx.h"
#include "MR_Class.h"

MR_Class::MR_Class(void)
{
}

MR_Class::~MR_Class(void)
{
}
void MR_Class::SetDefault_MR(){
	Set(default_MR_Count_tablename,default_MR_Count_field,default_MR_Count_keyfield);
}
void MR_Class::SetDefault_MR_Drop(){
	Set(default_MR_Count_Drop_tablename,default_MR_Count_Drop_field,default_MR_Count_Drop_keyfield);
}
bool MR_Class::SelectTwoCondition(string minkeyvalue,string maxkeyvalue,vector<Grid_MR>&gridmrinfo){
	gridmrinfo.clear();
	PGresult* res=VSelectTwoCondition(minkeyvalue.c_str(),maxkeyvalue.c_str());
	int num=PQntuples(res);
	if(num<1)
		return false;
	gridmrinfo.resize(num);
	for(int i=0;i<num;i++){
		gridmrinfo[i].grid_id=atoi(PQgetvalue(res,i,0));
		gridmrinfo[i].mrcount=atoi(PQgetvalue(res,i,1));
	}
	PQclear(res);
	return true;
}