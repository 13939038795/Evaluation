#pragma once
#include "SQLclass.h"
class MainCell_Grid_Power:public SQLclass
{
public:
	MainCell_Grid_Power(void);
	~MainCell_Grid_Power(void);
	void SetDefault();
	void Setkeyfield_gridid();
	bool SelectOneCondition_Main(string keyvalue,vector<Main_GridPower>&maingridpower);
	bool SelectTwoCondition(string minkeyvalue,string maxkeyvalue,vector<Main_GridPower>&maingridpower);
	bool FormatConver(PGresult* res,vector<Main_GridPower>&maingridpower);
	
	bool SelectOneCondition_Ever_Main(string keyvalue,string ever_field_name,float&fieldever);
};
