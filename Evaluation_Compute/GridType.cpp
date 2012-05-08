#include "StdAfx.h"
#include "GridType.h"

GridType::GridType(void)
{
}

GridType::~GridType(void)
{
}
void GridType::SetDefault(){
	Set(default_grid_tablename,default_grid_field,default_grid_keyfield);
}
bool GridType::SelectTwoCondition(string minkeyvalue,string maxkeyvalue,vector<Grid_Type>&gridtype){
	gridtype.clear();
	PGresult* res=VSelectTwoCondition(minkeyvalue.c_str(),maxkeyvalue.c_str());
	int num=PQntuples(res);
	if(num<1)
		return false;
	gridtype.resize(num);
	for(int i=0;i<num;i++){
		gridtype[i].grid_id=atoi(PQgetvalue(res,i,0));
		gridtype[i].grid_type=atoi(PQgetvalue(res,i,1));
	}
	PQclear(res);
	return true;
}