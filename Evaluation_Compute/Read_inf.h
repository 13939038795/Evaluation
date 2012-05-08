#pragma once
#include <string>
#include "dataclass.h"
//#include "GridRegion.h"
#include <vector>
#include"Cellinfo_Class.h"
#include<fstream>
#include<math.h>
#include"Evaluation.h"
using namespace std;
class Read_info
{
public:
	Read_info(void);
	~Read_info(void);
	bool ReadComputeInfo(string compute_path_name);//从文档中读取所需信息
	bool GetCellsInRegion();//读取区域内频率为fre 类型为type的天线
    bool WriteFactors(string resultpathname);
//	bool AddResultToFile(string resultpathname,Evaluation celleva);
	Point   IdToCoordinate(UINT64 grid_id);//将id转为在全北京的坐标

	string connection;//数据库信息
	string grids_tablename;//网格信息数据库表名
	string cellsinfo_tablename;//天线信息数据库表名
	string maincells_tablename;//天线主控数据库表名
	string effect_cells_tablename;
	string time;//计算时间
//	string result_file_name;//计算结果存储路径
	string cellid_file_name;//所需计算的天线id所在文档
	string cellcoverinfo_file_name;//天线覆盖信息所在的二进制文档的路径
	string cell_compute_type;//天线计算类型
	string DiffInterTablename;//异频干扰数据库
	string SameInterTablename;//同频干扰数据库
	string MrCountDroptablename;//MR掉话网格统计数据表
	string MrCounttablename;//MR网格统计数据表
	UINT64 regionminid;
	int height;
	int width;
	string celltype;
	int frequency;
	string networksort;
	vector<Cellinfo> CellsInRegion;//区域内的天线
//	vector<Cellinfo> CellsAffectRegion;


};
