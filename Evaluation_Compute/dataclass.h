#pragma once
#include<vector>
#include<string>
using namespace std;
typedef unsigned __int64    UINT64;

//#define TO_BIG_MATRIX	false

//北京new  EPSG:32650
#define BIG_WIDTH 35520//地图每行格子数
#define BIG_HEIGHT 100000//地图格子行数
#define MIN_Longitude 364760
#define MIN_Latitude 4365890
#define GRID_SIZE 5 //格子的大小 单位为米

#define PI 3.1415926
#define POWER "-94"
#define DIRECTION 65
#define CANONNUM 12
#define POWER_CANON -94
#define COVER_CANON 0

#define DEFAULT_POWER -300
#define POOR_COVER_MARGIN -94
#define RXLEVEL_MIN -110
#define INTERF_MARGIN 12
#define INTERF_MARGIN_NEIGHBOR 0
#define GRID_TYPE_ROAD 1
#define GRID_TYPE_INDOOR 4
#define CELLID_MAX 65536

#define EVALUATION_COMPUTE_INFO_FILE_NAME  "evaluation_compute_info.txt"

#define default_maincells_tablename   "field_cells"  //默认主控数据表名称
#define default_maincells_field   "grid_id,cellname,receivepower"  //默认主控数据所用的域名
#define default_maincells_keyfield_gridid   " grid_id " //默认主控数据所用的关键域名
#define default_maincells_keyfield_cellname    "cellname "//默认天线数据表用的关键域名
#define default_maincells_avg_field  "receivepower"

#define default_cell_tablename    "cells"//默认天线的数据表名
#define default_cell_field    "cell_id,grid_id,celltype,bcchno,tch,direction,height,vange,type_code"//默认天线数据表的域名
#define default_cell_keyfield_cellid    "cell_id "//默认天线数据表用的关键域名
#define default_cell_keyfield_gridid "grid_id"

#define default_grid_tablename "grid_0_1"//默认网格数据表名称
#define default_grid_field "grid_id,grid_type"//默认网格数据表所需的属性
#define default_grid_keyfield "grid_id"//默认网格数据表的关键属性

#define default_inter_tablename   "field_cells"  //默认主控数据表名称
#define default_inter_field   "grid_id,interf_cells_num,total_interference,bcchno_interf_cells_num,bcchno_interference"  //默认主控数据所用的域名
#define default_inter_keyfield  "grid_id " //默认主控数据所用的关键域名

#define default_MR_Count_Drop_tablename "mr_count"//默认的mr掉话网格数量数据库表
#define default_MR_Count_Drop_field "grid_id,count"//默认所需的属性名
#define default_MR_Count_Drop_keyfield "grid_id"//默认关键属性名

#define default_MR_Count_tablename  "traffic_20110704_mathnew_all"//默认MR网格数量数据库表
#define default_MR_Count_field "grid_id,traffic_val"//默认MR所需的属性名
#define default_MR_Count_keyfield "grid_id"//默认MR关键属性名

struct Grid_Type{//格子的类型信息，用于读取数据库
	UINT64 grid_id;
	int grid_type;

};
struct Cellinfo{//天线的信息，用于读取数据库
	string cell_id;
	UINT64 grid_id;
	string celltype;
	int bcchno;
	string tch;//字符串形式存储的tch频点
	vector<int> tchno;//tch频点
	float direction;
	float tilt;
	float height;
	int frequency;
};
struct  Point{//点信息，用于标识左下角和右上角
	int x;
	int y;
}; 
//******************************************************************
struct Cover_GridPower{//某个格子中某个天线的场强，用于读取小区文件
	UINT64 grid_id;
	float receivepower;
};
struct Cover_GridPower_new{//新的field文件存储格式

};
struct Grid_MR{//某个格子某个小区的话务量，用于读取MR文件
	UINT64 grid_id;
	double mrcount;
};
struct GeographyInfo{//某个格子某个小区的场景,用于读取场景信息文件
	UINT64 grid_id;
	int is_visual;
	int is_reflection;
	int is_diffraction;
};
struct TCH_GridNum{//某个格子中TCH同频的小区数，用于读取TCH文件时的预读，取得TCHnum
	UINT64 grid_id;
	int TCHnum;
};
struct TCH_GridVec{//某个格子中TCH同频小区的向量，用于读取TCH文件后的结果保存
	UINT64 grid_id;
	int TCHnum;
	vector<float> TCHvec;
};
//*********************************************************************
//*************** by xudayong******************************************
struct Grid_Power_GeographyInfo{//某个小区的某个格子信号强度和地理信息
	UINT64 grid_id;
	float receivepower;
	int is_visual;
	int is_reflection;
	int is_diffraction;
};
struct Cell_CoverInfo{//某个小区的覆盖全部格子信息
	string cellid;
	vector<Grid_Power_GeographyInfo> grids;
	Point maxpoint;//左下角
	Point minpoint;//右上角
	UINT64 mingrid_id;//最小grid_id

	float main_conti;//主控小区连续覆盖率
	float main_overcover;//主控过覆盖率
	float main_cross;//主控交叉覆盖率
	float main_reverse;//主控反向覆盖率

	int main_area;	//主控区域的格子数
	int main_boundarea;//主控区域边界格子数
	int main_road_area;//主控区域内的道路相关格子数

	float main_power_avg;//主控区域的平均场强
	float main_boundpower_avg;//主控区域的边缘格子平均场强
	float main_indoor_outdoor_power_avg;//主控区域内的室内外交界处平均长枪
	float main_roadpower_avg;//主控区域内的道路相关格子平均场强

	float main_distance_max;//主控区域的最远覆盖距离
	float main_distance_avg;//主控区域的平均覆盖距离
	
	//float Same_main_Noise;//主控区域内同频底噪
	//int Same_main_nointer_area;//主控区域内的同频C/I达标格子数
	int main_samebcch_interf_area;//主控区域内的同频C/I不达标格子数
	int main_samebcch_interf_nocover_area;//主控区域内的同频C/I不达标且主控覆盖不达标的格子数
	//float main_diffbcch_Noise;//主控区域内异频底噪
	//int Diff_main_nointer_area;//主控区域内的BCCH C/I达标格子数
	int main_diffbcch_interf_area;//主控区域内的BCCH C/I不达标格子数
	int main_diffbcch_interf_nocover_area;//主控区域内的BCCH C/I不达标且主控覆盖不达标的格子数
	
	float main_samebcch_interf_cellnum;//主控区域内平均同频有干扰小区数
	float Same_main_inter_power;//主控区域内平均同频干扰强度
	float Diff_main_inter_cellnum;//主控区域内平均BCCH有干扰小区数
	float Diff_main_inter_power;//主控区域内平均BCCH干扰强度
	//float main_conti_area;//主控区域内连续覆盖的格子数（即边界格子数）
	//float main_overcover_area;//主控区域内过覆盖面积（？？？）
	

	//float main_cover_area_ratio;//主控与覆盖面积比
	//float main_cover_dist_ratio;//主控与覆盖距离比
	
	float main_LOS;//主控区域内场景为LOS的格子数
	float main_NLOS_1;//主控区域内场景为NLOS一次反射或绕射的格子数
	float main_NLOS;//主控区域内场景为NLOS多次反射或绕射的格子数
	

	/*
	//KPI
	float maincover_main_area_ratio;//主控覆盖率
	float Same_main_CI;//主控区域内同频C/I达标率
	float Diff_main_CI;//主控区域内BCCH C/I达标率
	float Same_main_roadCI;//主控区域内的道路相关格子的同频C/I达标率
	float Diff_main_roadCI;//主控区域内的道路相关格子的异频C/I达标率

	int cover_area;//小区覆盖面积的格子数
	float cover_power_avg;//平均覆盖场强
	float cover_distance_max;//最远覆盖距离
	float cover_distance_avg;//平均覆盖距离

	//float cover_LOS;//覆盖范围内主控小区的LOS格子数
	//float cover_NLOS;//覆盖范围内主控小区的NLOS多次反射与绕射的格子数
	//float cover_NLOS_1;//覆盖范围内主控小区的NLOS一次反射或绕射的格子数

	double cover_MR_CI;//MR中C/I达标的数量

	
	int maincover_area;//主控覆盖的格子数
	float maincover_distance_max;//主控覆盖区域的最远距离
	float maincover_distance_avg;//主控覆盖区域的边界格子的平均距离
	
	
	//float maincover_power_avg;//主控覆盖区域内的平均场强
	//float maincover_boundpower_avg;//主控覆盖区域的边界格子的平均场强
	
	
	int maincover_bound_area;//主控覆盖区域内的边界格子数
	float maincover_roadpower_avg;//主控覆盖区域内的道路相关格子的平均场强
	float Same_maincover_roadCI;//主控覆盖范围内的道路相关格子的平均同频C/I
	float Diff_maincover_roadCI;//主控覆盖范围内的道路相关格子的平均BCCH C/I
	int  maincover_road_area;//主控覆盖范围的道路相关格子数
	int Same_maincover_nointer_area;//主控覆盖范围的同频C/I达标的格子数
	int Same_maincover_inter_area;//主控覆盖范围内的同频C/I不达标的格子数
	
	//float Same_maincover_MRdropCI;
	//float Same_maincover_MRCI;
	//float Diff_maincover_MRdropCI;
	//float Diff_maincover_MRCI;
	
	float Same_maincover_Noise;//主控覆盖区域的同频底噪
	int Diff_maincover_nointer_area;//主控覆盖区域的BCCH C/I达标的格子数
	int Diff_maincover_inter_area;//主控覆盖区域的BCCH C/I不达标的格子数
	float Diff_maincover_Noise;//主控覆盖区域的异频底缲
	float Same_maincover_inter_cellnum;//主控覆盖区域的平均同频有干扰小区数
	float Same_maincover_inter_power;//主控覆盖区域的平均同频干扰强度
	float Diff_maincover_inter_cellnum;//主控覆盖区域的平均BCCH有干扰小区数
	float Diff_maincover_inter_power;//主控覆盖区域的平均BCCH干扰强度
	//float maincover_conti_area;//主控覆盖区域内的连续覆盖的格子数
	//float maincover_overcover_area;//主控覆盖区域内的过覆盖的格子数(???)
	//float Same_maincover_CI;//主控覆盖区域内的同频平均干扰强度
	//float Diff_maincover_CI;//主控覆盖区域内的BCCH平均干扰强度
	
	//float maincover_cover_area_ratio;//主控覆盖与覆盖面积比
	//float maincover_cover_dist_ratio;//主控覆盖与覆盖距离比
	//float maincover_cross_avg;//主控覆盖区域的交叉覆盖（？？？）
	//float reverse_avg;//反向覆盖（？？？）

	//float maincover_LOS;//主控覆盖区域内的LOS场景的格子数
	//float maincover_NLOS_1;//主控覆盖区域内的ＮＬＯＳ一次反射或绕射场景的格子数
	//float maincover_NLOS;//主控覆盖区域内的ＮＬＯＳ多次反射或绕射场景的格子数
	

	int Same_covernotmain_inter_area;//非主控覆盖区域内的有同频干扰的格子数
	int Same_covernotmain_nointer_area;//非主控覆盖区域内的无同频干扰的格子数
	float Same_covernotmain_inter_cellnum;//非主控覆盖区域内的平均同频干扰小区个数
	float Same_covernotmain_inter_power;//非主控覆盖区域内的平均同频干扰强度
	int Diff_covernotmain_inter_area;//非主控覆盖区域内的平均有BCCH干扰的格子数
	int Diff_covernotmain_nointer_area;//非主控覆盖区域内的平均无BCCH干扰的格子数
	float Diff_covernotmain_inter_cellnum;//非主控覆盖区域内的平均BCCH干扰小区数
	float Diff_covernotmain_inter_power;//非主控覆盖区域内的平均BCCH干扰强度
	
	float Bias_Height;//天线高度分布差
	float Bias_Distance;//天线距离分布差
	//*/
};

struct Cells_Power_GeographyInfo{//某个格子的某个小区信号强度和地理信息
	string cellid;
	bool isValid;//是否有效数据
	float receivepower;
	int is_visual;
	int is_reflection;
	int is_diffraction;
};
struct Grid_CellsInfo{//某个格子的所有小区和强度信号信息
	UINT64 grid_id;
	int grid_type;
	vector<Cells_Power_GeographyInfo> cells;
	
	string maincellid;//主控小区cellid
	float maincellpower;//主控小区在此格子中的场强
	int is_visual;//主控小区在此格子中是否为可视场景
	int is_reflection;//主控小区在此格子中是否为反射场景
	int is_diffraction;//主控小区在此格子中是否为绕射场景
	bool isBorder;//此格子是否为主控小区的边界格子
	bool isIndoorOutdoorMargin;//此格子是否为室内外交界处
	int Same_inter_num;//与主控小区场强差在12dB以内的小区个数
	float Same_inter_power;//同频干扰强度
	int Diff_inter_num;//与主控小区相同BCCH频率且场强差在12dB以内的小区个数
	float Diff_inter_power;//BCCH干扰强度
	int TCH_Same_Num;//与主控小区存在TCH相同频点，且场强差在12dB以内的小区个数
	vector<float> TCH_Same_Vec;//每个TCH频点的干扰强度
	int TCH_Neighbor_Num;//与主控小区存在TCH邻频频点，且场强差在12dB以内的小区个数
	vector<float> TCH_Neighbor_Vec;//每个TCH邻频频点的干扰强度
//	double mrtraffic;
	string top1ncellid;
	string top2ncellid;
	string top3ncellid;
	string top4ncellid;
	string top5ncellid;
	string top6ncellid;
	float top1ncellpower;
	float top2ncellpower;
	float top3ncellpower;
	float top4ncellpower;
	float top5ncellpower;
	float top6ncellpower;

};
struct Region_Result{
	float cover_ratio;//区域内主控小区覆盖达标的比例
	float cover_road_ratio;//区域内道路相关格子主控小区覆盖达标的比例 
	float cover_indoor_outdoor_ratio;//区域内室内外交界处主控小区覆盖达标的比例

	float Same_cover_rate;//区域内平均同频重叠覆盖度
	float Same_cover_rate_road;//区域内道路相关格子的平均同频重叠覆盖度
	float Diff_cover_rate;//区域内平均BCCH同频重叠覆盖度
	float Diff_cover_rate_road;//区域内道路相关格子的平均BCCH同频重叠覆盖度

	float Same_C2I_rate;//区域内同频 C/I达标的格子比例
	float Diff_C2I_rate;//区域内BCCH C/I达标的格子比例
	float TCH_C2I_rate;//区域内TCH C/I达标的格子比例

	float Same_C2I_rate_road;//区域内道路同频 C/I达标的格子比例
	float Diff_C2I_rate_road;//区域内道路BCCH C/I达标的格子比例
	float TCH_C2I_rate_road;//区域内道路TCH C/I达标的格子比例

	float main_LOS;//区域内主控小区的LOS格子数
	float main_NLOS;//区域内主控小区的NLOS多次反射与绕射的格子数
	float main_NLOS_1;//区域内主控小区的NLOS一次反射或绕射的格子数

};
struct GridsInfo_Column{
	vector<Grid_CellsInfo> gi;
};

//*************** by xudayong******************************************
//**********************************************************************
struct Main_GridPower{//某个格子的主控小区号和对应场强
	UINT64 grid_id;
	string cellid;
	float receivepower;
};
struct Grid_Inter{//某个格子主控小区的BCCH干扰情况
	UINT64 grid_id;
	int Same_internum;
	float Same_interpower;
	int Diff_internum;
	float Diff_interpower;
};
struct Grid_Info{
	UINT64 grid_id;
	float cover_power;
	float main_power;
	string maincellid;
	int grid_type;
	int Same_internum;
	float Same_interpower;
	int Diff_internum;
	float Diff_interpower;
	//float mrcount;
	//float mrcount_drop;
	int TCH_Same_Num;
	vector<float> TCH_Same_Vec;
	int TCH_Neighbor_Num;
	vector<float> TCH_Neighbor_Vec;
	double mrtraffic;
	GeographyInfo geoinfo;

};
struct VEC{//某个小区的格子集合
	vector<UINT64>gridid_vec;
};
