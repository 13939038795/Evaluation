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
	bool ReadComputeInfo(string compute_path_name);//���ĵ��ж�ȡ������Ϣ
	bool GetCellsInRegion();//��ȡ������Ƶ��Ϊfre ����Ϊtype������
    bool WriteFactors(string resultpathname);
//	bool AddResultToFile(string resultpathname,Evaluation celleva);
	Point   IdToCoordinate(UINT64 grid_id);//��idתΪ��ȫ����������

	string connection;//���ݿ���Ϣ
	string grids_tablename;//������Ϣ���ݿ����
	string cellsinfo_tablename;//������Ϣ���ݿ����
	string maincells_tablename;//�����������ݿ����
	string effect_cells_tablename;
	string time;//����ʱ��
//	string result_file_name;//�������洢·��
	string cellid_file_name;//������������id�����ĵ�
	string cellcoverinfo_file_name;//���߸�����Ϣ���ڵĶ������ĵ���·��
	string cell_compute_type;//���߼�������
	string DiffInterTablename;//��Ƶ�������ݿ�
	string SameInterTablename;//ͬƵ�������ݿ�
	string MrCountDroptablename;//MR��������ͳ�����ݱ�
	string MrCounttablename;//MR����ͳ�����ݱ�
	UINT64 regionminid;
	int height;
	int width;
	string celltype;
	int frequency;
	string networksort;
	vector<Cellinfo> CellsInRegion;//�����ڵ�����
//	vector<Cellinfo> CellsAffectRegion;


};
