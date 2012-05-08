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
	bool Readinfo(string compute_path_name);//��ȡ������Ϣ
	//void GetCellinfo(Cellinfo tempcellinfo);//���������Ϣcellinfo
	//bool GetCellsInRegion();//�����ݿ��ж�ȡ������Ƶ��Ϊfrequency,����Ϊcelltype������
	bool GetCells();//�����ݿ��ж�ȡ���������и����漰����С���б�Ƶ��Ϊfrequency,����Ϊcelltype
	bool GetGridsType();//�����ݿ��ж�ȡ���и��ӵĵ�ò����

	bool GetCells_CoverInfo();//��ȡCellsAll�б��е�С��������С���ĸ���bin�ļ�
	//bool Getcell_coverinfo();//�������߸������������ƽ����ǿֵ�����и��ǣ�û��-94���ƣ���minpoint,maxpoint,mingrid_id
	bool GetCell_CoverInfo(Cellinfo cell,int index);//��ȡָ��С���ĸ���bin�ļ�
	bool GetGeoInformation(Cellinfo cell,int index);//��ȡָ��С���ĸ��ǵ�òbin�ļ�

	bool GetCells_CoverInfo_BigMatrix();//��ȡCellsAll�б��е�С��������С���ĸ���bin�ļ�
	bool GetCell_CoverInfo_BigMatrix(Cellinfo cell,int index);//��ȡָ��С���ĸ���bin�ļ�
	bool GetGeoInformation_BigMatrix(Cellinfo cell,int index);//��ȡָ��С���ĸ��ǵ�òbin�ļ�
	bool GetGridsKPI_BigMatrix();//��������С������ǿ��6���������Լ�TCHͬƵ��BCCHͬƵ���š�

	//bool Getgrid_info();//�������ߵĸ��ǣ����أ����ţ�TCHͬƵ��TCH��Ƶ��MR����Ϣ
	bool GetGridsInfo();//����CellsInfo_Region����<����,<С��1,power1,С��2,power2,...>�ṹ��GridsInfo_Region
	bool GetGridsKPI(int i,int j);//��������С������ǿ��6���������Լ�TCHͬƵ��BCCHͬƵ���š�
	bool GetCellsKPI();//����С������ָ��
	bool GetRegionKPI();//��������KPI
	
	//bool GetInter();
	//bool Grid_Compute();//�������ߵĸ���ָ��
	//bool Get_Crossever();//�������ߵĽ��渲����
	//bool Get_Bias();//�������ߵĸ߶Ⱥ;����׼����
	
	bool Verify_MainPower();//��֤����С���ĳ�ǿ
	bool Verify_Inter();//��֤ͬƵ��BCCH����
	bool Verify_TCHInter();//��֤TCH����


	bool WriteRegionResult(string temp_result_path_name);
	bool WriteCellsResult(string temp_result_path_name);
	bool WriteGridsInfo(string temp_result_path_name);

	bool Eval();
	
	Point IdToCoordinate(UINT64 grid_id);//grididתΪȫ������
	Point IdToCoordinate_Region(UINT64 grid_id,UINT64 minregionid);//grididת��Ϊregion�ڵ����꣬����ֱ�Ӷ�ȡ��Ӧ��GridsInfo_Region
	float Distance(UINT64 gridid_one,UINT64 gridid_two);//����gridid֮��ľ���
	bool ListOfFreqsSameTCH(string cellid1,string cellid2,vector<int> &tchlist);//��ѯָ������С��TCH����ͬƵ��Ƶ���б�
	bool ListOfFreqsNeighbourTCH(string cellid1,string cellid2,vector<int> &tchlist);//��ѯָ������С��TCH������Ƶ��Ƶ���б�
	bool IsFreqSameBCCH(string cellid1,string cellid2);//��ѯָ������ָ��С��BCCH�Ƿ�ͬƵ
	float PowerAdd(float p1,float p2);//��������dBmΪ��λ�Ĺ���ֵ���ת��ΪdBm�Ľ��

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
	string time;//����ʱ��
	string sToBigMatrix;//�Ƿ���ô����ģʽ�����죬��ռ���ڴ��
	//Cellinfo cellinfo;
	//UINT64 mingrid_id;
	//Point minpoint;
	//Point maxpoint;
	UINT64 regionminid;
	Point regionminpoint;
	int height;
	int width;
	int frequency;

	
	vector<int> CellsInRegion;//�����ڵ�����,�����ݿ��ж�ȡ,ΪCellsAll�е�index
	vector<Cellinfo> CellsAll;//ȫ�����ߵ��б�,�����ݿ��ж�ȡ by xudayong
	//vector<vector<int> > gridtype;//���������и��ӵĵ�ò����,�����ݿ��ж�ȡ

	vector<Cell_CoverInfo> CellsInfo_Region ;//���������и����漰�����ߵĸ�����Ϣ����ȡ�ļ� by xudayong

	//vector<vector<float> > mainpower;//�����ڸ��ӵ����س�ǿ����,����õ�
	//vector<vector<Grid_Inter> >interpower;//���������и��ӵ�����С����BCCH�������������õ�
	//vector<vector<GeographyInfo> >gridgeoinfo;//���������и��ӵ�����С���ĵ�ò��Ϣ������õ�
	
	vector<GridsInfo_Column> GridsInfo_Region;//���������и��ӵ������Ϣ������õ���by xudayong
	//vector<vector<Grid_Info> > gridinfo;//���������и��ӵ������Ϣ������õ� by wangshenna

	Region_Result result_region;



};
