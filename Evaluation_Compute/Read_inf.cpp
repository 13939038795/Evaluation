#include "StdAfx.h"
#include "Read_info.h"
Read_info::Read_info(void)
{
}

Read_info::~Read_info(void)
{
}
bool Read_info::ReadComputeInfo(string compute_path_name){
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
		if(str.find("grids_tablename") != string::npos){
			int j=0;
			while(str[j]!=':'){
				j++;
			}
			j++;
			while(str[j]==' '){
				j++;
			}
			grids_tablename=str.substr(j,str.length()-j);
				cout<<grids_tablename<<endl;
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
		if(str.find("maincell_tablename") != string::npos){
			int j=0;
			while(str[j]!=':'){
				j++;
			}
			j++;
			while(str[j]==' '){
				j++;
			}
			maincells_tablename=str.substr(j,str.length()-j);
			cout<<maincells_tablename<<endl;
			continue;
		}
		if(str.find("SameInterTablename") != string::npos){
			int j=0;
			while(str[j]!=':'){
				j++;
			}
			j++;
			while(str[j]==' '){
				j++;
			}
			SameInterTablename=str.substr(j,str.length()-j);
			cout<<SameInterTablename<<endl;
			continue;
		}
		if(str.find("Diff_Inter_Tablename") != string::npos){
			int j=0;
			while(str[j]!=':'){
				j++;
			}
			j++;
			while(str[j]==' '){
				j++;
			}
			DiffInterTablename=str.substr(j,str.length()-j);
			cout<<DiffInterTablename<<endl;
			continue;
		}
		if(str.find("MR_COUNT_DROP_tablename") != string::npos){
			int j=0;
			while(str[j]!=':'){
				j++;
			}
			j++;
			while(str[j]==' '){
				j++;
			}
			MrCountDroptablename=str.substr(j,str.length()-j);
			cout<<MrCountDroptablename<<endl;
			continue;
		}
		if(str.find("MR_COUNT_tablename") != string::npos){
			int j=0;
			while(str[j]!=':'){
				j++;
			}
			j++;
			while(str[j]==' '){
				j++;
			}
			MrCounttablename=str.substr(j,str.length()-j);
			cout<<MrCounttablename<<endl;
			continue;
		}
		if(str.find("effect_cells_tablename") != string::npos){
			int j=0;
			while(str[j]!=':'){
				j++;
			}
			j++;
			while(str[j]==' '){
				j++;
			}
			effect_cells_tablename=str.substr(j,str.length()-j);
			cout<<effect_cells_tablename<<endl;
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
/*
		if(str.find("result_file_name") != string::npos){
			unsigned j=0;
			while(str[j]!=':'){
				j++;
			}
			j++;
			while(str[j]==' '){
				j++;
			}
			result_file_name=str.substr(j,str.length()-j);
			//		cout<<result_file_name<<endl;
			continue;
		}
*/
		if(str.find("cellcoverinfo_file_name") != string::npos){
			unsigned j=0;
			while(str[j]!=':'){
				j++;
			}
			j++;
			while(str[j]==' '){
				j++;
			}
			cellcoverinfo_file_name=str.substr(j,str.length()-j);
			//		cout<<cellcoverinfo_file_name<<endl;
			continue;
		}
		if(str.find("cell_compute_type") != string::npos){
			unsigned j=0;
			while(str[j]!=':'){
				j++;
			}
			j++;
			while(str[j]==' '){
				j++;
			}
			cell_compute_type=str.substr(j,str.length()-j);
			//			cout<<cell_compute_type<<endl;
			continue;
		}

		if(cell_compute_type=="file"){
			if(str.find("cellid_file_name") != string::npos){
				unsigned j=0;
				while(str[j]!=':'){
					j++;
				}
				j++;
				while(str[j]==' '){
					j++;
				}
				cellid_file_name=str.substr(j,str.length()-j);
				//		cout<<cellid_file_name<<endl;
				continue;
			}
		}

		if(cell_compute_type=="region"){
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
	}
	regionminid=max(regionminid,(UINT64)1);
	//cout<<"regionminid"<<regionminid<<" height"<<height<<" width"<<width<<endl;
	fi.close();
	cout<<"read successful"<<endl;
	return true;
}

bool Read_info::GetCellsInRegion( ){
	PGconn *conn=PQconnectdb(connection.c_str());//�����ݿ�
	if (PQstatus(conn) != CONNECTION_OK){
		fprintf(stderr, "Connection to database failed: %s",PQerrorMessage(conn));
		PQfinish(conn);
		return false;
	}
	Cellinfo_Class ce;
	ce.SetConn(conn);
	cout<<"tablename:"<<cellinfo_tablename<<" field:"<<default_cell_field<<" keyfield:"<<default_cell_keyfield_gridid<<endl;
	ce.setdefault();
	ce.SetTableName(cellsinfo_tablename);
	ce.Set(default_cell_keyfield_gridid);
	vector<Cellinfo> tempcellvector;
	tempcellvector.clear();
	ce.selectonecondition(NULL,tempcellvector);
	cout<<"size:"<<(int)tempcellvector.size()<<endl;
//	system("pause");
	PQfinish(conn);
	//cout<<tempcellvector.size()<<endl;
	CellsInRegion.clear();
	Point regionpoint=IdToCoordinate(regionminid);
	for(int i=0;i<(int)tempcellvector.size();i++){
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

Point Read_info::IdToCoordinate(UINT64 grid_id){
	Point temppoint;
	temppoint.x=((int)grid_id-1)/BIG_WIDTH;
	temppoint.y=((int)grid_id-1)%BIG_WIDTH;
	return temppoint;
}

bool Read_info::WriteFactors(std::string resultpathname){
	ofstream fo(resultpathname.c_str(),'w');
	if (fo.is_open()==false){
		return false;
	}
	fo<<"��������"<<","\
	    <<"����ƽ����ǿ"<<","<<"���ط�Χƽ����ǿ"<<","<<"���ظ���ƽ����ǿ"<<","<<"���ط�Χ�߽�ƽ����ǿ"<<","<<"���ظ��Ǳ߽�ƽ����ǿ"<<","\
		<<"���ط�Χ��·ƽ����ǿ"<<","<<"���ظ��ǵ�·ƽ����ǿ"<<","\
		<<"�������"<<","<<"���ط�Χ���"<<","<<"���ظ������"<<","\
		<<"��Զ���Ǿ���"<<","<<"��Զ���ط�Χ����"<<","<<"��Զ���ظ��Ǿ���"<<","<<"ƽ�����Ǿ���"<<","<<"ƽ�����ط�Χ����"<<","<<"ƽ�����ظ��Ǿ���"<<","\
		<<"ͬƵ���ǻ�վ����ƫ��ϵ��"<<","<<"ͬƵ���ǻ�վ�߶�ƫ��ϵ��"<<","\
		<<"ͬƵ���ظ��ǻ�վ����ƫ��ϵ��"<<","<<"ͬƵ���ظ��ǻ�վ�߶�ƫ��ϵ��"<<","\
		<<"��Ƶ���ǻ�վ����ƫ��ϵ��"<<","<<"��Ƶ���ǻ�վ�߶�ƫ��ϵ��"<<","\
		<<"��Ƶ���ظ��ǻ�վ����ƫ��ϵ��"<<","<<"��Ƶ���ظ��ǻ�վ�߶�ƫ��ϵ��"<<","\
		<<"���ط�ΧС������������"<<","<<"���ظ���С������������"<<","<<"��������"<<","<<"���򸲸���"<<","<<"���ؽ��渲����"<<","<<"���ظ��ǽ��渲����"<<","\
		<<"���ظ��������"<<","<<"���ظ��Ǿ����"<<","\
		<<"ͬƵ���ط�Χ�޸������"<<","<<"��Ƶ���ط�Χ�޸������"<<","<<"ͬƵ���ط�Χ�и������"<<","<<"��Ƶ���ط�Χ�и������"<<","\
		<<"ͬƵ���ظ����޸������"<<","<<"��Ƶ���ظ����޸������"<<","<<"ͬƵ���ظ����и������"<<","<<"��Ƶ���ظ����и������"<<","<<"ͬƵ�����ظ����޸������"<<","<<"��Ƶ�����ظ����޸������"<<","<<"ͬƵ�����ظ����и������"<<","<<"��Ƶ�����ظ����и������"<<","\
		<<"ͬƵ���ظ���ƽ���ص�С����"<<","<<"ͬƵ���ظ���ƽ���ܸ���ǿ��"<<","<<"��Ƶ���ظ���ƽ���ص�С����"<<","<<"��Ƶ���ظ���ƽ���ܸ���ǿ��"<<","<<"ͬƵC\\I�����"<<","<<"��ƵC\\I�����"<<","\
		<<"ͬƵ���ظ��ǵ�·C\\I�����"<<","<<"��Ƶ���ظ��ǵ�·C\\I�����"<<","\
		<<"ͬƵ����MR������ȨC\\I�����"<<","<<"��Ƶ����MR������ȨC\\I�����"<<","<<"ͬƵ���ظ���MR������ȨC\\I�����"<<","<<"��ƵMR������ȨC\\I�����"<<","\
		<<"ͬƵ����MR��ȨC\\I�����"<<","<<"��Ƶ����MR��ȨC\\I�����"<<","<<"ͬƵ���ظ���MR��ȨC\\I�����"<<","<<"��ƵMR��ȨC\\I�����"<<","\
		<<"ͬƵ����ˮƽ����"<<","<<"��Ƶ����ˮƽ����"<<","<<"ͬƵƽ����������С����"<<","<<"ͬƵƽ����������С��ǿ��"<<","<<"��Ƶƽ����������С����"<<","<<"��Ƶƽ����������С��ǿ��"<<endl;
	fo.close();
	return true;
}
/*
bool Read_info::AddResultToFile(std::string resultpathname, Evaluation celleva){
	ofstream fo(resultpathname.c_str(),ios::app);
	if (fo.is_open()==false){
		return false;
	}
	float maincoverever=0;
	if(celleva.covererea>0){
		maincoverever=celleva.maincovererea/(float)celleva.covererea;
	}	
	float maincoverdisever=0;
	if(celleva.CoverDisEver>0)
		maincoverdisever=celleva.MainCoverDisEver/(float)celleva.CoverDisEver;
	float DiffCIcanonever=0;
	float SameCIcanonever=0;
	float DiffRoadCIcanonever=0;
	float SameRoadCIcanonever=0;
	if(celleva.maincovererea>0){
		DiffCIcanonever=celleva.Diffmaincovernointerference/(float)celleva.maincovererea;
		SameCIcanonever=celleva.Samemaincovernointerference/(float)celleva.maincovererea;
	}
	float tempovercoverever=0;
	if(celleva.CoverDisEver>0)
		tempovercoverever=celleva.CoverMaxDis/(float)celleva.CoverDisEver;
	fo<<celleva.cellinfo.cell_id<<","\
	    <<celleva.cellcoverpowerever<<","<<celleva.cellmainpowerever<<","<<celleva.cellmaincoverpowerever<<","<<celleva.MainBoundpowerever<<","<<celleva.MainCoverboundpowerever<<","\
		<<celleva.RoadPowerever_Main<<","<<celleva.RoadPowerever_Maincover<<","\
		<<celleva.covererea<<","<<celleva.mainerea<<","<<celleva.maincovererea<<","\
		<<celleva.CoverMaxDis<<","<<celleva.MainMaxDis<<","<<celleva.MainCoverMaxDis<<","<<celleva.CoverDisEver<<","<<celleva.MainDisEver<<","<<celleva.MainCoverDisEver<<","\
		<<celleva.SameCoverBaseBiasEver<<","<<celleva.SameCoverHeightBiasEver<<","\
		<<celleva.SameMainBaseBiasEver<<","<<celleva.SameMainHeightBiasEver<<","\
		<<celleva.DiffCoverBaseBiasEver<<","<<celleva.DiffCoverHeightBiasEver<<","\
		<<celleva.DiffMainBaseBiasEver<<","<<celleva.DiffMainHeightBiasEver<<","\
		<<celleva.MainContiEver<<","<<celleva.MainCoverContiEver<<","<<tempovercoverever<<","<<celleva.ReverseCoverEver<<","<<celleva.CrossCoverEver_Main<<","<<celleva.CrossCoverEver_MainCover<<","\
		<<maincoverever<<","<<maincoverdisever<<","\
		<<celleva.Samemainnointerference<<","<<celleva.Diffmainnointerference<<","<<celleva.Samemaininterference<<","<<celleva.Diffmaininterference<<","\
		<<celleva.Samemaincovernointerference<<","<<celleva.Diffmaincovernointerference<<","<<celleva.Samemaincoverinterference<<","<<celleva.Diffmaincoverinterference<<","\
		<<celleva.Samecovernotmainnointerference<<","<<celleva.Diffcovernotmainnointerference<<","<<celleva.Samecovernotmaininterference<<","<<celleva.Diffcovernotmaininterference<<","\
		<<celleva.Diffcoverintercellsever<<","<<celleva.Samecoverinterpowerever<<","<<celleva.Samecoverintercellsever<<","<<celleva.Diffcoverinterpowerever<<","<<SameCIcanonever<<","<<DiffCIcanonever<<","\
		<<celleva.SameRoadCIever_MainCover<<","<<celleva.DiffRoadCIever_MainCover<<","\
		<<celleva.SameMainCIMREver<<","<<celleva.DiffMainCIMREver<<","<<celleva.SameMainCoverCIMREver<<","<<celleva.DiffMainCoverCIMREver<<","\
		<<celleva.SameMainCIMRCOUNTEver<<","<<celleva.DiffMainCIMRCOUNTEver<<","<<celleva.SameMainCoverCIMRCOUNTEver<<","<<celleva.DiffMainCoverCIMRCOUNTEver<<","\
		<<celleva.SameNoiseever<<","<<celleva.DiffNoiseever<<","<<celleva.Sameinterothercellsnum<<","<<celleva.Sameinterotherpowerever<<","<<celleva.Diffinterothercellsnum<<","<<celleva.Diffinterotherpowerever<<endl;
return true;

}
*/