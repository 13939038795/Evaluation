#pragma once
#include "SQLclass.h"
class MR_Class:public SQLclass
{
public:
	MR_Class(void);
	~MR_Class(void);
	void SetDefault_MR_Drop();
	void SetDefault_MR();
	bool SelectTwoCondition(string minkeyvalue,string maxkeyvalue,vector<Grid_MR>&gridmrinfo);
};
