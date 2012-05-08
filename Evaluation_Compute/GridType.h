#pragma once
#include "SQLclass.h"
class GridType:public SQLclass
{
public:
	GridType(void);
	~GridType(void);
	void SetDefault();
	bool SelectTwoCondition(string minkeyvalue,string maxkeyvalue,vector<Grid_Type>&gridtype);
};
