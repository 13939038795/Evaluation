#pragma once
#include <string>
#include "dataclass.h"
#include <vector>
#include"Cellinfo_Class.h"
#include<fstream>
#include<math.h>
#include"Evaluation.h"
using namespace std;
class Read
{
public:
	Read(void);
	~Read(void);
	bool ReadComputeInfo(string compute_path_name);//从文档中读取所需信息
	bool GetCellsInRegion();//读取区域内频率为fre 类型为type的天线
    bool WriteFactors(string resultpathname);//写入指标名称
	bool WriteFactors_another(string resultpathname);
	bool AddResultToFile(string resultpathname,Evaluation celleva);//添加指标具体
	bool AddResultToFile_another(string resultpathname,Evaluation celleva);
	Point   IdToCoordinate(UINT64 grid_id);//将id转为在全北京的坐标

	string connection;
	string cellinfo_tablename;
	string result_path_name;
	string celltype;
	string networksort;
	string time;//计算时间
	UINT64 regionminid;
	int height;
	int width;
	int frequency;
	vector<Cellinfo> CellsInRegion;//区域内的天线


};
