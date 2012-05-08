#pragma once
#include<vector>
#include<string>
using namespace std;
typedef unsigned __int64    UINT64;

//#define TO_BIG_MATRIX	false

//����new  EPSG:32650
#define BIG_WIDTH 35520//��ͼÿ�и�����
#define BIG_HEIGHT 100000//��ͼ��������
#define MIN_Longitude 364760
#define MIN_Latitude 4365890
#define GRID_SIZE 5 //���ӵĴ�С ��λΪ��

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

#define default_maincells_tablename   "field_cells"  //Ĭ���������ݱ�����
#define default_maincells_field   "grid_id,cellname,receivepower"  //Ĭ�������������õ�����
#define default_maincells_keyfield_gridid   " grid_id " //Ĭ�������������õĹؼ�����
#define default_maincells_keyfield_cellname    "cellname "//Ĭ���������ݱ��õĹؼ�����
#define default_maincells_avg_field  "receivepower"

#define default_cell_tablename    "cells"//Ĭ�����ߵ����ݱ���
#define default_cell_field    "cell_id,grid_id,celltype,bcchno,tch,direction,height,vange,type_code"//Ĭ���������ݱ������
#define default_cell_keyfield_cellid    "cell_id "//Ĭ���������ݱ��õĹؼ�����
#define default_cell_keyfield_gridid "grid_id"

#define default_grid_tablename "grid_0_1"//Ĭ���������ݱ�����
#define default_grid_field "grid_id,grid_type"//Ĭ���������ݱ����������
#define default_grid_keyfield "grid_id"//Ĭ���������ݱ�Ĺؼ�����

#define default_inter_tablename   "field_cells"  //Ĭ���������ݱ�����
#define default_inter_field   "grid_id,interf_cells_num,total_interference,bcchno_interf_cells_num,bcchno_interference"  //Ĭ�������������õ�����
#define default_inter_keyfield  "grid_id " //Ĭ�������������õĹؼ�����

#define default_MR_Count_Drop_tablename "mr_count"//Ĭ�ϵ�mr���������������ݿ��
#define default_MR_Count_Drop_field "grid_id,count"//Ĭ�������������
#define default_MR_Count_Drop_keyfield "grid_id"//Ĭ�Ϲؼ�������

#define default_MR_Count_tablename  "traffic_20110704_mathnew_all"//Ĭ��MR�����������ݿ��
#define default_MR_Count_field "grid_id,traffic_val"//Ĭ��MR�����������
#define default_MR_Count_keyfield "grid_id"//Ĭ��MR�ؼ�������

struct Grid_Type{//���ӵ�������Ϣ�����ڶ�ȡ���ݿ�
	UINT64 grid_id;
	int grid_type;

};
struct Cellinfo{//���ߵ���Ϣ�����ڶ�ȡ���ݿ�
	string cell_id;
	UINT64 grid_id;
	string celltype;
	int bcchno;
	string tch;//�ַ�����ʽ�洢��tchƵ��
	vector<int> tchno;//tchƵ��
	float direction;
	float tilt;
	float height;
	int frequency;
};
struct  Point{//����Ϣ�����ڱ�ʶ���½Ǻ����Ͻ�
	int x;
	int y;
}; 
//******************************************************************
struct Cover_GridPower{//ĳ��������ĳ�����ߵĳ�ǿ�����ڶ�ȡС���ļ�
	UINT64 grid_id;
	float receivepower;
};
struct Cover_GridPower_new{//�µ�field�ļ��洢��ʽ

};
struct Grid_MR{//ĳ������ĳ��С���Ļ����������ڶ�ȡMR�ļ�
	UINT64 grid_id;
	double mrcount;
};
struct GeographyInfo{//ĳ������ĳ��С���ĳ���,���ڶ�ȡ������Ϣ�ļ�
	UINT64 grid_id;
	int is_visual;
	int is_reflection;
	int is_diffraction;
};
struct TCH_GridNum{//ĳ��������TCHͬƵ��С���������ڶ�ȡTCH�ļ�ʱ��Ԥ����ȡ��TCHnum
	UINT64 grid_id;
	int TCHnum;
};
struct TCH_GridVec{//ĳ��������TCHͬƵС�������������ڶ�ȡTCH�ļ���Ľ������
	UINT64 grid_id;
	int TCHnum;
	vector<float> TCHvec;
};
//*********************************************************************
//*************** by xudayong******************************************
struct Grid_Power_GeographyInfo{//ĳ��С����ĳ�������ź�ǿ�Ⱥ͵�����Ϣ
	UINT64 grid_id;
	float receivepower;
	int is_visual;
	int is_reflection;
	int is_diffraction;
};
struct Cell_CoverInfo{//ĳ��С���ĸ���ȫ��������Ϣ
	string cellid;
	vector<Grid_Power_GeographyInfo> grids;
	Point maxpoint;//���½�
	Point minpoint;//���Ͻ�
	UINT64 mingrid_id;//��Сgrid_id

	float main_conti;//����С������������
	float main_overcover;//���ع�������
	float main_cross;//���ؽ��渲����
	float main_reverse;//���ط��򸲸���

	int main_area;	//��������ĸ�����
	int main_boundarea;//��������߽������
	int main_road_area;//���������ڵĵ�·��ظ�����

	float main_power_avg;//���������ƽ����ǿ
	float main_boundpower_avg;//��������ı�Ե����ƽ����ǿ
	float main_indoor_outdoor_power_avg;//���������ڵ������⽻�紦ƽ����ǹ
	float main_roadpower_avg;//���������ڵĵ�·��ظ���ƽ����ǿ

	float main_distance_max;//�����������Զ���Ǿ���
	float main_distance_avg;//���������ƽ�����Ǿ���
	
	//float Same_main_Noise;//����������ͬƵ����
	//int Same_main_nointer_area;//���������ڵ�ͬƵC/I��������
	int main_samebcch_interf_area;//���������ڵ�ͬƵC/I����������
	int main_samebcch_interf_nocover_area;//���������ڵ�ͬƵC/I����������ظ��ǲ����ĸ�����
	//float main_diffbcch_Noise;//������������Ƶ����
	//int Diff_main_nointer_area;//���������ڵ�BCCH C/I��������
	int main_diffbcch_interf_area;//���������ڵ�BCCH C/I����������
	int main_diffbcch_interf_nocover_area;//���������ڵ�BCCH C/I����������ظ��ǲ����ĸ�����
	
	float main_samebcch_interf_cellnum;//����������ƽ��ͬƵ�и���С����
	float Same_main_inter_power;//����������ƽ��ͬƵ����ǿ��
	float Diff_main_inter_cellnum;//����������ƽ��BCCH�и���С����
	float Diff_main_inter_power;//����������ƽ��BCCH����ǿ��
	//float main_conti_area;//�����������������ǵĸ����������߽��������
	//float main_overcover_area;//���������ڹ������������������
	

	//float main_cover_area_ratio;//�����븲�������
	//float main_cover_dist_ratio;//�����븲�Ǿ����
	
	float main_LOS;//���������ڳ���ΪLOS�ĸ�����
	float main_NLOS_1;//���������ڳ���ΪNLOSһ�η��������ĸ�����
	float main_NLOS;//���������ڳ���ΪNLOS��η��������ĸ�����
	

	/*
	//KPI
	float maincover_main_area_ratio;//���ظ�����
	float Same_main_CI;//����������ͬƵC/I�����
	float Diff_main_CI;//����������BCCH C/I�����
	float Same_main_roadCI;//���������ڵĵ�·��ظ��ӵ�ͬƵC/I�����
	float Diff_main_roadCI;//���������ڵĵ�·��ظ��ӵ���ƵC/I�����

	int cover_area;//С����������ĸ�����
	float cover_power_avg;//ƽ�����ǳ�ǿ
	float cover_distance_max;//��Զ���Ǿ���
	float cover_distance_avg;//ƽ�����Ǿ���

	//float cover_LOS;//���Ƿ�Χ������С����LOS������
	//float cover_NLOS;//���Ƿ�Χ������С����NLOS��η���������ĸ�����
	//float cover_NLOS_1;//���Ƿ�Χ������С����NLOSһ�η��������ĸ�����

	double cover_MR_CI;//MR��C/I��������

	
	int maincover_area;//���ظ��ǵĸ�����
	float maincover_distance_max;//���ظ����������Զ����
	float maincover_distance_avg;//���ظ�������ı߽���ӵ�ƽ������
	
	
	//float maincover_power_avg;//���ظ��������ڵ�ƽ����ǿ
	//float maincover_boundpower_avg;//���ظ�������ı߽���ӵ�ƽ����ǿ
	
	
	int maincover_bound_area;//���ظ��������ڵı߽������
	float maincover_roadpower_avg;//���ظ��������ڵĵ�·��ظ��ӵ�ƽ����ǿ
	float Same_maincover_roadCI;//���ظ��Ƿ�Χ�ڵĵ�·��ظ��ӵ�ƽ��ͬƵC/I
	float Diff_maincover_roadCI;//���ظ��Ƿ�Χ�ڵĵ�·��ظ��ӵ�ƽ��BCCH C/I
	int  maincover_road_area;//���ظ��Ƿ�Χ�ĵ�·��ظ�����
	int Same_maincover_nointer_area;//���ظ��Ƿ�Χ��ͬƵC/I���ĸ�����
	int Same_maincover_inter_area;//���ظ��Ƿ�Χ�ڵ�ͬƵC/I�����ĸ�����
	
	//float Same_maincover_MRdropCI;
	//float Same_maincover_MRCI;
	//float Diff_maincover_MRdropCI;
	//float Diff_maincover_MRCI;
	
	float Same_maincover_Noise;//���ظ��������ͬƵ����
	int Diff_maincover_nointer_area;//���ظ��������BCCH C/I���ĸ�����
	int Diff_maincover_inter_area;//���ظ��������BCCH C/I�����ĸ�����
	float Diff_maincover_Noise;//���ظ����������Ƶ����
	float Same_maincover_inter_cellnum;//���ظ��������ƽ��ͬƵ�и���С����
	float Same_maincover_inter_power;//���ظ��������ƽ��ͬƵ����ǿ��
	float Diff_maincover_inter_cellnum;//���ظ��������ƽ��BCCH�и���С����
	float Diff_maincover_inter_power;//���ظ��������ƽ��BCCH����ǿ��
	//float maincover_conti_area;//���ظ��������ڵ��������ǵĸ�����
	//float maincover_overcover_area;//���ظ��������ڵĹ����ǵĸ�����(???)
	//float Same_maincover_CI;//���ظ��������ڵ�ͬƵƽ������ǿ��
	//float Diff_maincover_CI;//���ظ��������ڵ�BCCHƽ������ǿ��
	
	//float maincover_cover_area_ratio;//���ظ����븲�������
	//float maincover_cover_dist_ratio;//���ظ����븲�Ǿ����
	//float maincover_cross_avg;//���ظ�������Ľ��渲�ǣ���������
	//float reverse_avg;//���򸲸ǣ���������

	//float maincover_LOS;//���ظ��������ڵ�LOS�����ĸ�����
	//float maincover_NLOS_1;//���ظ��������ڵģΣ̣ϣ�һ�η�������䳡���ĸ�����
	//float maincover_NLOS;//���ظ��������ڵģΣ̣ϣӶ�η�������䳡���ĸ�����
	

	int Same_covernotmain_inter_area;//�����ظ��������ڵ���ͬƵ���ŵĸ�����
	int Same_covernotmain_nointer_area;//�����ظ��������ڵ���ͬƵ���ŵĸ�����
	float Same_covernotmain_inter_cellnum;//�����ظ��������ڵ�ƽ��ͬƵ����С������
	float Same_covernotmain_inter_power;//�����ظ��������ڵ�ƽ��ͬƵ����ǿ��
	int Diff_covernotmain_inter_area;//�����ظ��������ڵ�ƽ����BCCH���ŵĸ�����
	int Diff_covernotmain_nointer_area;//�����ظ��������ڵ�ƽ����BCCH���ŵĸ�����
	float Diff_covernotmain_inter_cellnum;//�����ظ��������ڵ�ƽ��BCCH����С����
	float Diff_covernotmain_inter_power;//�����ظ��������ڵ�ƽ��BCCH����ǿ��
	
	float Bias_Height;//���߸߶ȷֲ���
	float Bias_Distance;//���߾���ֲ���
	//*/
};

struct Cells_Power_GeographyInfo{//ĳ�����ӵ�ĳ��С���ź�ǿ�Ⱥ͵�����Ϣ
	string cellid;
	bool isValid;//�Ƿ���Ч����
	float receivepower;
	int is_visual;
	int is_reflection;
	int is_diffraction;
};
struct Grid_CellsInfo{//ĳ�����ӵ�����С����ǿ���ź���Ϣ
	UINT64 grid_id;
	int grid_type;
	vector<Cells_Power_GeographyInfo> cells;
	
	string maincellid;//����С��cellid
	float maincellpower;//����С���ڴ˸����еĳ�ǿ
	int is_visual;//����С���ڴ˸������Ƿ�Ϊ���ӳ���
	int is_reflection;//����С���ڴ˸������Ƿ�Ϊ���䳡��
	int is_diffraction;//����С���ڴ˸������Ƿ�Ϊ���䳡��
	bool isBorder;//�˸����Ƿ�Ϊ����С���ı߽����
	bool isIndoorOutdoorMargin;//�˸����Ƿ�Ϊ�����⽻�紦
	int Same_inter_num;//������С����ǿ����12dB���ڵ�С������
	float Same_inter_power;//ͬƵ����ǿ��
	int Diff_inter_num;//������С����ͬBCCHƵ���ҳ�ǿ����12dB���ڵ�С������
	float Diff_inter_power;//BCCH����ǿ��
	int TCH_Same_Num;//������С������TCH��ͬƵ�㣬�ҳ�ǿ����12dB���ڵ�С������
	vector<float> TCH_Same_Vec;//ÿ��TCHƵ��ĸ���ǿ��
	int TCH_Neighbor_Num;//������С������TCH��ƵƵ�㣬�ҳ�ǿ����12dB���ڵ�С������
	vector<float> TCH_Neighbor_Vec;//ÿ��TCH��ƵƵ��ĸ���ǿ��
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
	float cover_ratio;//����������С�����Ǵ��ı���
	float cover_road_ratio;//�����ڵ�·��ظ�������С�����Ǵ��ı��� 
	float cover_indoor_outdoor_ratio;//�����������⽻�紦����С�����Ǵ��ı���

	float Same_cover_rate;//������ƽ��ͬƵ�ص����Ƕ�
	float Same_cover_rate_road;//�����ڵ�·��ظ��ӵ�ƽ��ͬƵ�ص����Ƕ�
	float Diff_cover_rate;//������ƽ��BCCHͬƵ�ص����Ƕ�
	float Diff_cover_rate_road;//�����ڵ�·��ظ��ӵ�ƽ��BCCHͬƵ�ص����Ƕ�

	float Same_C2I_rate;//������ͬƵ C/I���ĸ��ӱ���
	float Diff_C2I_rate;//������BCCH C/I���ĸ��ӱ���
	float TCH_C2I_rate;//������TCH C/I���ĸ��ӱ���

	float Same_C2I_rate_road;//�����ڵ�·ͬƵ C/I���ĸ��ӱ���
	float Diff_C2I_rate_road;//�����ڵ�·BCCH C/I���ĸ��ӱ���
	float TCH_C2I_rate_road;//�����ڵ�·TCH C/I���ĸ��ӱ���

	float main_LOS;//����������С����LOS������
	float main_NLOS;//����������С����NLOS��η���������ĸ�����
	float main_NLOS_1;//����������С����NLOSһ�η��������ĸ�����

};
struct GridsInfo_Column{
	vector<Grid_CellsInfo> gi;
};

//*************** by xudayong******************************************
//**********************************************************************
struct Main_GridPower{//ĳ�����ӵ�����С���źͶ�Ӧ��ǿ
	UINT64 grid_id;
	string cellid;
	float receivepower;
};
struct Grid_Inter{//ĳ����������С����BCCH�������
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
struct VEC{//ĳ��С���ĸ��Ӽ���
	vector<UINT64>gridid_vec;
};
