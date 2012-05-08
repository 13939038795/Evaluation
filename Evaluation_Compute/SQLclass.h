#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "libpq-fe.h"
#include "dataclass.h"
using namespace std;
class SQLclass
{
public:
	SQLclass(void);
	~SQLclass(void);
protected:
	PGconn   *Vconn;
	std::string Vtablename,Vfield,Vkeyfield,VOrderfield,VOrder;

public:
	void SetConn(PGconn *conn);
	void Set(const char* tablename, const char* field, const char* keyfield);
	void Set(const char* field, const char* keyfield);
	void Set(const char* keyfield);
	void SetTableName(const char* tablename);
	void SetOrder(const char* Orderfield, const char* Order);
	PGresult* VSelectOneCondition(const char* keyvalue);
	PGresult* VSelectOneCondition(const char* keyvalue,std::string &query);
	PGresult* VSelectTwoCondition(const char* minkeyvalue, const char* maxkeyvalue);
	PGresult* VSelect_avg(string field,string keyvalue);

	virtual void SetDefault(){};
};
