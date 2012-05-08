#include "StdAfx.h"
#include "Read.h"

Read::Read(void)
{
}

Read::~Read(void)
{
}
bool Read::ReadComputeInfo(std::string compute_path_name){
	ifstream fi(compute_path_name.c_str());
	if(!fi){
		cout<<"No evaluation compute information!"<<endl;
		return false;
	}
	string str;
	while(getline(fi,str)){		
		if(str.find("connection") != string::npos){//���ҵ�CONNECTION�����ݿ���Ϣ����Ȼ���ȡ���ݿ���Ϣ
			int j=0;
			while(str[j]!=':'){
				j++;
			}
			j++;
			while(str[j]==' '){
				j++;
			}
			connection=str.substr(j,str.length()-j);
				cout<<connection<<endl;
			continue;
		}
		if(str.find("cellinfo_tablename") != string::npos){
			int j=0;
			while(str[j]!=':'){
				j++;
			}
			j++;
			while(str[j]==' '){
				j++;
			}
			cellinfo_tablename=str.substr(j,str.length()-j);
			cout<<cellinfo_tablename<<endl;
			continue;
		}
		if(str.find("result_path_name") != string::npos){
			int j=0;
			while(str[j]!=':'){
				j++;
			}
			j++;
			while(str[j]==' '){
				j++;
			}
			result_path_name=str.substr(j,str.length()-j);
			cout<<result_path_name<<endl;
			continue;
		}
		if(str.find("networksort") != string::npos){
			unsigned j=0;
			while(str[j]!=':'){
				j++;
			}
			j++;
			while(str[j]==' '){
				j++;
			}
			networksort=str.substr(j,str.length()-j);
				cout<<networksort<<endl;
			continue;
		}
		if(str.find("time") != string::npos){//��ȡ"TIME"
			unsigned j=0;
			while(str[j]!=':'){
				j++;
			}
			j++;
			while(str[j]==' '){
				j++;
			}
			time=str.substr(j,str.length()-j);
				cout<<time<<endl;
			continue;
		}
		if(str.find("celltype") != string::npos){
			unsigned j=0;
			while(str[j]!=':'){
				j++;
			}
			j++;
			while(str[j]==' '){
				j++;
			}
			celltype=str.substr(j,str.length()-j);
			cout<<celltype<<endl;
			continue;
		}
		if(str.find("frequency") != string::npos){
			unsigned j=0;
			while(str[j]!=':'){
				j++;
			}
			j++;
			while(str[j]==' '){
				j++;
			}
			frequency=atoi(str.substr(j,str.length()-j).c_str());
				cout<<frequency<<endl;
			continue;
		}

		if(str.find("region_info") != string::npos){
			unsigned j=0;
			while(str[j]!=':'){
				j++;
			}
			j++;
			int k;
			int index=0;//�ڼ������֣�0��minid��1��height��2��width��
			while(j<str.length()){
				while(str[j]==' '){
					j++;
				}
				k=j;//�����������һ���ǿո�Ԫ�أ����֣���λ��
				while(str[j]!=' ' &&  j<str.length()){//Ѱ����һ���ո���ߵ����ַ�����β��
					j++;
				}
				switch(index){
					case 0:regionminid=atoi(str.substr(k,j-k).c_str()); break;
					case 1:height=atoi(str.substr(k,j-k).c_str()); break;
					case 2:width=atoi(str.substr(k,j-k).c_str()); break;
				}
				index++;
			}

			continue;
		}

	}
	regionminid=max(regionminid,(UINT64)1);
	fi.close();
	return true;
}
bool Read::GetCellsInRegion(){
	PGconn *conn=PQconnectdb(connection.c_str());//�����ݿ�
	if (PQstatus(conn) != CONNECTION_OK){
		fprintf(stderr, "Connection to database failed: %s",PQerrorMessage(conn));
		PQfinish(conn);
		return false;
	}
	Cellinfo_Class ce;
	ce.SetConn(conn);
	ce.setdefault();
	ce.SetTableName(cellinfo_tablename.c_str());
	ce.Set(default_cell_keyfield_gridid);
	vector<Cellinfo> tempcellvector;
	tempcellvector.clear();
	ce.selectall(tempcellvector);// ������ݱ������е�������Ϣ
	PQfinish(conn);
	CellsInRegion.clear();
	Point regionpoint=IdToCoordinate(regionminid);
	for(int i=0;i<(int)tempcellvector.size();i++){//��ȫ��������ѡȡ�������ڵ�������Ϣ������CellsInRegion��
		if(tempcellvector[i].grid_id<=0)
			continue;
		if((tempcellvector[i].frequency==frequency||frequency==0)&&(tempcellvector[i].celltype==celltype||celltype=="")){
			Cellinfo tempcellinfo;
			tempcellinfo.bcchno=tempcellvector[i].bcchno;
			tempcellinfo.cell_id=tempcellvector[i].cell_id;
			tempcellinfo.celltype=tempcellvector[i].celltype;
			tempcellinfo.grid_id=tempcellvector[i].grid_id;
			tempcellinfo.direction=tempcellvector[i].direction;
			tempcellinfo.height=tempcellvector[i].height;
			tempcellinfo.frequency=tempcellvector[i].frequency;
			Point cellpoint=IdToCoordinate(tempcellvector[i].grid_id);
			if(cellpoint.x<regionpoint.x||cellpoint.x>regionpoint.x+height||cellpoint.y<regionpoint.y||cellpoint.y>regionpoint.y+width)
				continue;
			else
				CellsInRegion.push_back(tempcellinfo);
		}
	}
	tempcellvector.clear();
	cout<<"�������߸���:"<<CellsInRegion.size()<<endl;
	return true;
}
Point Read::IdToCoordinate(UINT64 grid_id){//grid_idתΪȫ������
	Point temppoint;
	temppoint.x=((int)grid_id-1)/BIG_WIDTH;
	temppoint.y=((int)grid_id-1)%BIG_WIDTH;
	return temppoint;
}
bool Read::WriteFactors(std::string resultpathname){//д��ָ������
	ofstream fo(resultpathname.c_str(),'w');
	if (fo.is_open()==false){
		return false;
	}
	fo<<"��������"<<","<<"����ƽ����ǿ"<<","<<"����ƽ����ǿ"<<","<<"���ظ���ƽ����ǿ"<<","<<"���ر߽�ƽ����ǿ"<<","<<"���ظ��Ǳ߽�ƽ����ǿ"<<","<<"���ص�·ƽ����ǿ"<<","<<"���ظ��ǵ�·ƽ����ǿ"\
		<<","<<"�������"<<","<<"�������"<<","<<"���ظ������"<<","<<"��Զ���Ǿ���"<<","<<"��Զ���ؾ���"<<","<<"��Զ���ظ��Ǿ���"<<","<<"ƽ�����Ǿ���"<<","<<"ƽ�����ؾ���"<<","<<"ƽ�����ظ��Ǿ���"\
		<<","<<"����С������������"<<","<<"���ظ���С������������"<<","<<"���ع�������"<<","<<"���ظ��ǹ�������"<<","<<"���ظ��Ƿ��򸲸���"<<","<<"���ؽ��渲����"<<","<<"���ظ��ǽ��渲����"\
		<<","<<"�����븲�������"\
		<<","<<"���ظ����븲�������"<<","<<"�����븲�Ǿ����"<<","<<"���ظ����븲�Ǿ����"<<","<<"ͬƵ�����޸������"<<","<<"ͬƵ�����и������"<<","<<"ͬƵ���ظ����޸������"<<","<<"ͬƵ���ظ����и������"\
		<<","<<"��Ƶ�����޸������"<<","<<"��Ƶ�����и������"<<","<<"��Ƶ���ظ����޸������"<<","<<"��Ƶ���ظ����и������"<<","<<"ͬƵ���Ƿ������޸������"<<","<<"ͬƵ���Ƿ������и������"\
		<<","<<"��Ƶ���Ƿ������޸������"<<","<<"��Ƶ���Ƿ������и������"<<","<<"ͬƵ����C\\I�����"<<","<<"��Ƶ����C\\I�����"<<","<<"ͬƵ���ظ���C\\I�����"<<","<<"��Ƶ���ظ���C\\I�����"\
		<<","<<"ͬƵ���ص�·C\\I�����"<<","<<"��Ƶ���ص�·C\\I�����"<<","<<"ͬƵ���ظ��ǵ�·C\\I�����"<<","<<"��Ƶ���ظ��ǵ�·C\\I�����"<<","<<"MR��ȨTCH�����"\
		<<","<<"ͬƵ����ƽ���ص�С����"<<","<<"ͬƵ����ƽ���ܸ���ǿ��"<<","<<"��Ƶ����ƽ���ص�С����"<<","<<"��Ƶ����ƽ���ܸ���ǿ��"\
		<<","<<"ͬƵ���ظ���ƽ���ص�С����"<<","<<"ͬƵ���ظ���ƽ���ܸ���ǿ��"<<","<<"��Ƶ���ظ���ƽ���ص�С����"<<","<<"��Ƶ���ظ���ƽ���ܸ���ǿ��"\
		<<","<<"ͬƵ���ص���"<<","<<"��Ƶ���ص���"<<","<<"ͬƵ���ظ��ǵ���"<<","<<"��Ƶ���ظ��ǵ���"\
		<<","<<"ͬƵƽ����������С����"<<","<<"ͬƵƽ����������С��ǿ��"<<","<<"��Ƶƽ����������С����"<<","<<"��Ƶƽ����������С��ǿ��"\
		<<","<<"��վ����ƫ��ϵ��"<<","<<"��վ�߶�ƫ��ϵ��"\
		<<","<<"���ǿ��ӱ���"<<","<<"����һ��NLOS����"<<","<<"���Ƕ��NLOS"\
		<<","<<"���ؿ��ӱ���"<<","<<"����һ��NLOS����"<<","<<"���ض��NLOS"\
		<<","<<"���ظ��ǿ��ӱ���"<<","<<"���ظ���һ��NLOS����"<<","<<"���ظ��Ƕ��NLOS"<<endl;
	fo.close();
	//cout<<"true\n";
	return true;
}
bool Read::WriteFactors_another(std::string resultpathname){
	ofstream fo(resultpathname.c_str(),'w');
	if (fo.is_open()==false){
		return false;
	}
	fo<<"��������"<<","<<"����ƽ����ǿ"<<","<<"�������"<<","<<"��Զ���Ǿ���"<<","<<"ƽ�����Ǿ���"<<","<<"MR��ȨTCH�����"<<","\
		<<"����ƽ����ǿ"<<","<<"���ر߽�ƽ����ǿ"<<","<<"���ص�·ƽ����ǿ"<<","<<"�������"<<","<<"��Զ���ؾ���"<<","<<"ƽ�����ؾ���"<<","\
		<<"����С������������"<<","<<"���ع�������"<<","<<"���ؽ��渲����"<<","<<"�����븲�������"<<","\
		<<"�����븲�Ǿ����"<<","<<"ͬƵ�����޸������"<<","<<"ͬƵ�����и������"<<","\
		<<"��Ƶ�����޸������"<<","<<"��Ƶ�����и������"<<","<<"ͬƵ����C\\I�����"<<","<<"��Ƶ����C\\I�����"<<","\
		<<"ͬƵ���ص�·C\\I�����"<<","<<"��Ƶ���ص�·C\\I�����"<<","<<"ͬƵ����ƽ���ص�С����"<<","<<"ͬƵ����ƽ���ܸ���ǿ��"<<","<<"��Ƶ����ƽ���ص�С����"<<","<<"��Ƶ����ƽ���ܸ���ǿ��"<<","\
		<<"ͬƵ���ص���"<<","<<"��Ƶ���ص���"<<","<<"���ظ���ƽ����ǿ"<<","<<"���ظ��Ǳ߽�ƽ����ǿ"<<","<<"���ظ��ǵ�·ƽ����ǿ"<<","<<"���ظ������"<<","<<"��Զ���ظ��Ǿ���"<<","<<"ƽ�����ظ��Ǿ���"<<","\
		<<"���ظ���С������������"<<","<<"���ظ��ǹ�������"<<","<<"���ظ��ǽ��渲����"<<","<<"���ظ��Ƿ��򸲸���"<<","<<"���ظ����븲�������"<<","<<"���ظ����븲�Ǿ����"<<","\
		<<"ͬƵ���ظ����޸������"<<","<<"ͬƵ���ظ����и������"<<","<<"��Ƶ���ظ����޸������"<<","<<"��Ƶ���ظ����и������"<<","<<"ͬƵ���ظ���C\\I�����"<<","<<"��Ƶ���ظ���C\\I�����"<<","\
		<<"ͬƵ���ظ��ǵ�·C\\I�����"<<","<<"��Ƶ���ظ��ǵ�·C\\I�����"<<","\
		<<"ͬƵ���ظ���ƽ���ص�С����"<<","<<"ͬƵ���ظ���ƽ���ܸ���ǿ��"<<","<<"��Ƶ���ظ���ƽ���ص�С����"<<","<<"��Ƶ���ظ���ƽ���ܸ���ǿ��"<<","\
		<<"ͬƵ���ظ��ǵ���"<<","<<"��Ƶ���ظ��ǵ���"<<","<<"ͬƵ���Ƿ������޸������"<<","<<"ͬƵ���Ƿ������и������"<<","<<"��Ƶ���Ƿ������޸������"<<","<<"��Ƶ���Ƿ������и������"<<","\
		<<"ͬƵƽ����������С����"<<","<<"ͬƵƽ����������С��ǿ��"<<","<<"��Ƶƽ����������С����"<<","<<"��Ƶƽ����������С��ǿ��"<<","\
		<<"��վ����ƫ��ϵ��"<<","<<"��վ�߶�ƫ��ϵ��"\
		<<","<<"���ǿ��ӱ���"<<","<<"����һ��NLOS����"<<","<<"���Ƕ��NLOS"\
		<<","<<"���ؿ��ӱ���"<<","<<"����һ��NLOS����"<<","<<"���ض��NLOS"\
		<<","<<"���ظ��ǿ��ӱ���"<<","<<"���ظ���һ��NLOS����"<<","<<"���ظ��Ƕ��NLOS"<<endl;

	fo.close();
	return true;
}
bool Read::AddResultToFile(std::string resultpathname, Evaluation celleva){//д��ָ��ֵ
	ofstream fo(resultpathname.c_str(),ios::app);
	if (fo.is_open()==false){
		return false;
	}
	fo<<celleva.cellinfo.cell_id<<","<<celleva.cover_powerever<<","<<celleva.main_powerever<<","<<celleva.maincover_powerever<<","<<celleva.main_boundpowerever<<","<<celleva.maincover_boundpowerever<<","\
		<<celleva.main_roadpowerever<<","<<celleva.maincover_roadpowerever<<","<<celleva.cover_erea<<","<<celleva.main_erea<<","<<celleva.maincover_erea<<","<<celleva.cover_maxdistance<<","\
		<<celleva.main_maxdistance<<","<<celleva.maincover_maxdistance<<","<<celleva.cover_avgdistance<<","<<celleva.main_avgdistance<<","<<celleva.maincover_avgdistance<<","\
		<<celleva.main_contiever<<","<<celleva.maincover_contiever<<","<<celleva.main_overcoverever<<","<<celleva.maincover_overcoverever<<","<<celleva.reverseever<<","<<celleva.main_crossever<<","<<celleva.maincover_crossever<<","\
		<<celleva.main_cover_erea<<","<<celleva.maincover_cover_erea<<","<<celleva.main_cover_dist<<","<<celleva.maincover_cover_dist<<","<<celleva.Same_main_nointererea<<","<<celleva.Same_main_intererea<<","\
		<<celleva.Same_maincover_nointererea<<","<<celleva.Same_maincover_intererea<<","<<celleva.Diff_main_nointererea<<","<<celleva.Diff_main_intererea<<","<<celleva.Diff_maincover_nointererea<<","\
		<<celleva.Diff_maincover_intererea<<","<<celleva.Same_covernotmain_nointererea<<","<<celleva.Same_covernotmain_intererea<<","<<celleva.Diff_covernotmain_nointererea<<","<<celleva.Diff_covernotmain_intererea<<","\
		<<celleva.Same_main_CI<<","<<celleva.Diff_main_CI<<","<<celleva.Same_maincover_CI<<","<<celleva.Diff_maincover_CI<<","<<celleva.Same_main_roadCI<<","<<celleva.Diff_main_roadCI<<","<<\
		celleva.Same_maincover_CI<<","<<celleva.Diff_maincover_CI<<","<<celleva.cover_MR_CI<<","<<celleva.Same_main_intercellnum<<","<<celleva.Same_main_interpower<<","<<celleva.Diff_main_intercellnum<<","\
		<<celleva.Diff_main_interpower<<","<<celleva.Same_maincover_intercellnum<<","<<celleva.Same_maincover_interpower<<","<<celleva.Diff_maincover_intercellnum<<","<<celleva.Diff_maincover_interpower<<","\
		<<celleva.Same_main_Noise<<","<<celleva.Diff_main_Noise<<","<<celleva.Same_maincover_Noise<<","<<celleva.Diff_maincover_Noise<<","\
		<<celleva.Same_covernotmain_intercellnum<<","<<celleva.Same_covernotmain_interpower<<","<<celleva.Diff_covernotmain_intercellnum<<","<<celleva.Diff_covernotmain_interpower<<","\
		<<celleva.Bias_Distance<<","<<celleva.Bias_Height<<","\
		<<celleva.cover_LOS<<","<<celleva.cover_NLOS_1<<","<<celleva.cover_NLOS<<","\
		<<celleva.main_LOS<<","<<celleva.main_NLOS_1<<","<<celleva.main_NLOS<<","\
		<<celleva.maincover_LOS<<","<<celleva.maincover_NLOS_1<<","<<celleva.maincover_NLOS<<endl;
	fo.close();
	return true;
}
bool Read::AddResultToFile_another(std::string resultpathname, Evaluation celleva){
	ofstream fo(resultpathname.c_str(),ios::app);
	if (fo.is_open()==false){
		return false;
	}
	fo<<celleva.cellinfo.cell_id<<","<<celleva.cover_powerever<<","<<celleva.cover_erea<<","<<celleva.cover_maxdistance<<","<<celleva.cover_avgdistance<<","<<celleva.cover_MR_CI<<","\
		<<celleva.main_powerever<<","<<celleva.main_boundpowerever<<","<<celleva.main_roadpowerever<<","<<celleva.main_erea<<","<<celleva.main_maxdistance<<","<<celleva.main_avgdistance<<","\
		<<celleva.main_contiever<<","<<celleva.main_overcoverever<<","<<celleva.main_crossever<<","<<celleva.main_cover_erea<<","<<celleva.main_cover_dist<<","\
		<<celleva.Same_main_nointererea<<","<<celleva.Same_main_intererea<<","<<celleva.Diff_main_nointererea<<","<<celleva.Diff_main_intererea<<","<<celleva.Same_main_CI<<","<<celleva.Diff_main_CI<<","\
		<<celleva.Same_main_roadCI<<","<<celleva.Diff_main_roadCI<<","<<celleva.Same_main_intercellnum<<","<<celleva.Same_main_interpower<<","<<celleva.Diff_main_intercellnum<<","<<celleva.Diff_main_interpower<<","\
		<<celleva.Same_main_Noise<<","<<celleva.Diff_main_Noise<<","<<celleva.maincover_powerever<<","<<celleva.maincover_boundpowerever<<","<<celleva.maincover_roadpowerever<<","<<celleva.maincover_erea<<","\
		<<celleva.maincover_maxdistance<<","<<celleva.maincover_avgdistance<<","<<celleva.maincover_contiever<<","<<celleva.maincover_overcoverever<<","<<celleva.maincover_crossever<<","<<celleva.reverseever<<","\
		<<celleva.maincover_cover_erea<<","<<celleva.maincover_cover_dist<<","<<celleva.Same_maincover_nointererea<<","<<celleva.Same_maincover_intererea<<","<<celleva.Diff_maincover_nointererea<<","\
		<<celleva.Diff_maincover_intererea<<","<<celleva.Same_maincover_CI<<","<<celleva.Diff_maincover_CI<<","<<celleva.Same_maincover_roadCI<<","<<celleva.Diff_maincover_roadCI<<","\
		<<celleva.Same_maincover_intercellnum<<","<<celleva.Same_maincover_interpower<<","<<celleva.Diff_maincover_intercellnum<<","<<celleva.Diff_maincover_interpower<<","\
		<<celleva.Same_maincover_Noise<<","<<celleva.Diff_maincover_Noise<<","<<celleva.Same_covernotmain_nointererea<<","<<celleva.Same_covernotmain_intererea<<","<<celleva.Diff_covernotmain_nointererea<<","\
		<<celleva.Diff_covernotmain_intererea<<","<<celleva.Same_covernotmain_intercellnum<<","<<celleva.Same_covernotmain_interpower<<","<<celleva.Diff_covernotmain_intercellnum<<","\
		<<celleva.Diff_covernotmain_interpower<<","\
		<<celleva.Bias_Distance<<","<<celleva.Bias_Height<<","\
		<<celleva.cover_LOS<<","<<celleva.cover_NLOS_1<<","<<celleva.cover_NLOS<<","\
		<<celleva.main_LOS<<","<<celleva.main_NLOS_1<<","<<celleva.main_NLOS<<","\
		<<celleva.maincover_LOS<<","<<celleva.maincover_NLOS_1<<","<<celleva.maincover_NLOS<<endl;
	fo.close();
	return true;
}
