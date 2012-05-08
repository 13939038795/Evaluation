#pragma once
#include "SQLclass.h"
class Inter_Info_Class:public SQLclass
{
public:
	Inter_Info_Class(void);
	~Inter_Info_Class(void);
	void SetDefault();
	bool SelectTwoCondition(string minkeyvalue,string maxkeyvalue,vector<Grid_Inter>&gridinter);

};

