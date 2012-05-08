#include "StdAfx.h"
#include "Cellinfo_Class.h"

Cellinfo_Class::Cellinfo_Class(void)
{
}

Cellinfo_Class::~Cellinfo_Class(void)
{
}
void Cellinfo_Class::setdefault(){
	//cout<<"cell set"<<endl;
	Set(default_cell_tablename,default_cell_field,default_cell_keyfield_cellid);
}
bool Cellinfo_Class::selectonecondition(string value, Cellinfo &cell){
	//cout<<"select ok\n";
	PGresult* res=VSelectOneCondition(value.c_str());
	//cout<<"PQ select is ok\n";
	return FormatConver(res,cell);
}
bool Cellinfo_Class::FormatConver(PGresult *res,Cellinfo & cell){
	//cout<<"this format is ok\n";
	if(PQntuples(res)<1)
		return false;
	cell.cell_id=(string)PQgetvalue(res,0,0);
	cell.grid_id=atoi(PQgetvalue(res,0,1));
	cell.celltype=(string)PQgetvalue(res,0,2);
	cell.bcchno=atoi(PQgetvalue(res,0,3));
	//cout<<1<<endl;
	if(cell.bcchno>=512 && cell.bcchno<=998){
		cell.frequency=1800;
	}
	else{
		cell.frequency=900;
	}
//	cout<<2<<endl;
	cell.height=(float)atof(PQgetvalue(res,0,5));
	int type_code=atoi(PQgetvalue(res,0,6));
	if(type_code==2 ||type_code==3){//二、三功分天线
		std::string tempstr=(std::string)PQgetvalue(res,0,4);
		size_t k=0,record_index=0;
		int direction=0;
		for(k=0;k<tempstr.length();k++){
			if(tempstr[k]=='/'){
				direction=direction*400+atoi((tempstr.substr(record_index,k-record_index)).c_str());
				record_index=k+1;
			}
		}
		direction=direction*400+atoi((tempstr.substr(record_index,k-record_index)).c_str());
		cell.direction=(float)direction;
	}
	else{
		cell.direction=(float)atof(PQgetvalue(res,0,4));
	}
//	cout<<3<<endl;
	PQclear(res);
	return true;
}

bool Cellinfo_Class::selectonecondition(string value, vector<Cellinfo> &cell){
	PGresult* res=VSelectOneCondition(value.c_str());
	return FormatConver(res,cell);
}
bool Cellinfo_Class::selectall(vector<Cellinfo>&cell){
	std::string query=(std::string)"Select "+Vfield+(std::string)" From "+Vtablename;
    PGresult* res=PQexec(Vconn,query.c_str());
    return FormatConver(res,cell);
}
bool Cellinfo_Class::FormatConver(PGresult *res,vector<Cellinfo> & cell){
	if(PQntuples(res)<1)
		return false;
	cell.clear();
	cell.resize(PQntuples(res));
	for(int i=0;i<PQntuples(res);i++){
		cell[i].cell_id=(string)PQgetvalue(res,i,0);
		cell[i].grid_id=atoi(PQgetvalue(res,i,1));
		cell[i].celltype=(string)PQgetvalue(res,i,2);
		cell[i].bcchno=atoi(PQgetvalue(res,i,3));	
		if(cell[i].bcchno>=512 && cell[i].bcchno<=998){
			cell[i].frequency=1800;
		}
		else{
			cell[i].frequency=900;
		}
		cell[i].height=(float)atof(PQgetvalue(res,i,5));
		int type_code=atoi(PQgetvalue(res,i,6));
		if(type_code==2 ||type_code==3){//二、三功分天线
			std::string tempstr=(std::string)PQgetvalue(res,i,4);
			size_t k=0,record_index=0;
			int direction=0;
			for(k=0;k<tempstr.length();k++){
				if(tempstr[k]=='/'){
					direction=direction*400+atoi((tempstr.substr(record_index,k-record_index)).c_str());
					record_index=k+1;
				}
			}
			direction=direction*400+atoi((tempstr.substr(record_index,k-record_index)).c_str());
			cell[i].direction=(float)direction;
		}
		else{
			cell[i].direction=(float)atof(PQgetvalue(res,i,4));
		}
	}
	PQclear(res);
	return true;
}
	