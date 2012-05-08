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
	bool ReadComputeInfo(string compute_path_name);//���ĵ��ж�ȡ������Ϣ
	bool GetCellsInRegion();//��ȡ������Ƶ��Ϊfre ����Ϊtype������
    bool WriteFactors(string resultpathname);//д��ָ������
	bool WriteFactors_another(string resultpathname);
	bool AddResultToFile(string resultpathname,Evaluation celleva);//���ָ�����
	bool AddResultToFile_another(string resultpathname,Evaluation celleva);
	Point   IdToCoordinate(UINT64 grid_id);//��idתΪ��ȫ����������

	string connection;
	string cellinfo_tablename;
	string result_path_name;
	string celltype;
	string networksort;
	string time;//����ʱ��
	UINT64 regionminid;
	int height;
	int width;
	int frequency;
	vector<Cellinfo> CellsInRegion;//�����ڵ�����


};
