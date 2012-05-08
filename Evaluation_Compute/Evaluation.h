#pragma once
#include <vector>
#include <string>
#include <fstream>
#include "dataclass.h"
#include "MainCell_Grid_Power.h"
#include "Cellinfo_Class.h"
#include "GridType.h"
#include "Inter_Info_Class.h"
#include "MR_Class.h"

#include<math.h>
#include<omp.h>
using namespace std;
class Evaluation
{
public:
	Evaluation(void);
	~Evaluation(void);
	bool Readinfo(string compute_path_name);//读取所需信息
	//void GetCellinfo(Cellinfo tempcellinfo);//获得天线信息cellinfo
	//bool GetCellsInRegion();//从数据库中读取区域内频率为frequency,类型为celltype的天线
	bool GetCells();//从数据库中读取区域内所有格子涉及到的小区列表，频率为frequency,类型为celltype
	bool GetGridsType();//从数据库中读取所有格子的地貌类型

	bool GetCells_CoverInfo();//读取CellsAll列表中的小区中所有小区的覆盖bin文件
	//bool Getcell_coverinfo();//计算天线覆盖面积，覆盖平均场强值，所有覆盖（没有-94限制）的minpoint,maxpoint,mingrid_id
	bool GetCell_CoverInfo(Cellinfo cell,int index);//读取指定小区的覆盖bin文件
	bool GetGeoInformation(Cellinfo cell,int index);//读取指定小区的覆盖地貌bin文件

	bool GetCells_CoverInfo_BigMatrix();//读取CellsAll列表中的小区中所有小区的覆盖bin文件
	bool GetCell_CoverInfo_BigMatrix(Cellinfo cell,int index);//读取指定小区的覆盖bin文件
	bool GetGeoInformation_BigMatrix(Cellinfo cell,int index);//读取指定小区的覆盖地貌bin文件
	bool GetGridsKPI_BigMatrix();//计算主控小区和最强的6个邻区，以及TCH同频，BCCH同频干扰。

	//bool Getgrid_info();//计算天线的覆盖，主控，干扰，TCH同频，TCH邻频，MR的信息
	bool GetGridsInfo();//根据CellsInfo_Region计算<格子,<小区1,power1,小区2,power2,...>结构的GridsInfo_Region
	bool GetGridsKPI(int i,int j);//计算主控小区和最强的6个邻区，以及TCH同频，BCCH同频干扰。
	bool GetCellsKPI();//计算小区级的指标
	bool GetRegionKPI();//计算区域级KPI
	
	//bool GetInter();
	//bool Grid_Compute();//计算天线的各个指标
	//bool Get_Crossever();//计算天线的交叉覆盖率
	//bool Get_Bias();//计算天线的高度和距离标准方差
	
	bool Verify_MainPower();//验证主控小区的场强
	bool Verify_Inter();//验证同频和BCCH干扰
	bool Verify_TCHInter();//验证TCH干扰


	bool WriteRegionResult(string temp_result_path_name);
	bool WriteCellsResult(string temp_result_path_name);
	bool WriteGridsInfo(string temp_result_path_name);

	bool Eval();
	
	Point IdToCoordinate(UINT64 grid_id);//gridid转为全局坐标
	Point IdToCoordinate_Region(UINT64 grid_id,UINT64 minregionid);//gridid转化为region内的坐标，用于直接读取对应的GridsInfo_Region
	float Distance(UINT64 gridid_one,UINT64 gridid_two);//两个gridid之间的距离
	bool ListOfFreqsSameTCH(string cellid1,string cellid2,vector<int> &tchlist);//查询指定两个小区TCH存在同频的频点列表
	bool ListOfFreqsNeighbourTCH(string cellid1,string cellid2,vector<int> &tchlist);//查询指定两个小区TCH存在邻频的频点列表
	bool IsFreqSameBCCH(string cellid1,string cellid2);//查询指定两个指定小区BCCH是否同频
	float PowerAdd(float p1,float p2);//计算两个dBm为单位的功率值相加转化为dBm的结果

public:

	string connection;
	string grid_tablename;
	string maincell_tablename;
	string cellinfo_tablename;
	string inter_tablename;
	string cellcover_path_name;
	string geo_path_name;
	string mrinfo_path_name;
	string tchinter_neighbor_path_name;
	string tchinter_same_path_name;
	string result_path_name;
	string celltype;
	string networksort;
	string time;//计算时间
	string sToBigMatrix;//是否采用大矩阵模式，更快，但占用内存大
	//Cellinfo cellinfo;
	//UINT64 mingrid_id;
	//Point minpoint;
	//Point maxpoint;
	UINT64 regionminid;
	Point regionminpoint;
	int height;
	int width;
	int frequency;

	
	vector<int> CellsInRegion;//区域内的天线,从数据库中读取,为CellsAll中的index
	vector<Cellinfo> CellsAll;//全部天线的列表,从数据库中读取 by xudayong
	//vector<vector<int> > gridtype;//区域内所有格子的地貌类型,从数据库中读取

	vector<Cell_CoverInfo> CellsInfo_Region ;//区域内所有格子涉及的天线的覆盖信息，读取文件 by xudayong

	//vector<vector<float> > mainpower;//区域内格子的主控场强矩阵,计算得到
	//vector<vector<Grid_Inter> >interpower;//区域内所有格子的主控小区的BCCH干扰情况，计算得到
	//vector<vector<GeographyInfo> >gridgeoinfo;//区域内所有格子的主控小区的地貌信息。计算得到
	
	vector<GridsInfo_Column> GridsInfo_Region;//区域内所有格子的相关信息，计算得到，by xudayong
	//vector<vector<Grid_Info> > gridinfo;//区域内所有格子的相关信息，计算得到 by wangshenna

	Region_Result result_region;



};
