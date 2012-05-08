#include "StdAfx.h"
#include "MainCell_Grid_Power.h"

MainCell_Grid_Power::MainCell_Grid_Power(void)
{
}

MainCell_Grid_Power::~MainCell_Grid_Power(void)
{
}
void MainCell_Grid_Power::SetDefault(){
	Set(default_maincells_tablename,default_maincells_field,default_maincells_keyfield_cellname);
}
void MainCell_Grid_Power::Setkeyfield_gridid(){
	Set(default_maincells_keyfield_gridid);
}
bool MainCell_Grid_Power::SelectOneCondition_Main(string keyvalue,vector<Main_GridPower>&maingridpower){
	maingridpower.clear();
	PGresult* res=VSelectOneCondition(keyvalue.c_str());
	return FormatConver(res,maingridpower);
}
bool MainCell_Grid_Power::SelectTwoCondition(string minkeyvalue,string maxkeyvalue,vector<Main_GridPower>&maingridpower){
	maingridpower.clear();
	PGresult* res=VSelectTwoCondition(minkeyvalue.c_str(),maxkeyvalue.c_str());
	return FormatConver(res,maingridpower);

}
bool MainCell_Grid_Power::FormatConver(PGresult* res,vector<Main_GridPower>&maingridpower){
	int num=PQntuples(res);
	if(num<1)
		return false;
	maingridpower.clear();
	maingridpower.resize(num);
	for(int i=0;i<num;i++){
		maingridpower[i].grid_id=atoi(PQgetvalue(res,i,0));
		maingridpower[i].cellid=(string)PQgetvalue(res,i,1);
		maingridpower[i].receivepower=(float)atof(PQgetvalue(res,i,2));
	}
	PQclear(res);
	return true;
}

bool MainCell_Grid_Power::SelectOneCondition_Ever_Main(string keyvalue,string ever_field_name,float&fieldever){
	fieldever=0;
	PGresult* res=VSelect_avg(ever_field_name,keyvalue);
	int num=PQntuples(res);
	if(num<1)
		return false;
	fieldever=(float)atof(PQgetvalue(res,0,0));
	PQclear(res);
	return true;
}

