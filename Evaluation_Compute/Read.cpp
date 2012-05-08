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
		if(str.find("connection") != string::npos){//在找到CONNECTION（数据库信息），然后读取数据库信息
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
		if(str.find("time") != string::npos){//读取"TIME"
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
			int index=0;//第几个数字（0：minid；1：height；2：width）
			while(j<str.length()){
				while(str[j]==' '){
					j++;
				}
				k=j;//“：”后面第一个非空格元素（数字）的位置
				while(str[j]!=' ' &&  j<str.length()){//寻找下一个空格或者到达字符串的尾端
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
	PGconn *conn=PQconnectdb(connection.c_str());//打开数据库
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
	ce.selectall(tempcellvector);// 获得数据表中所有的天线信息
	PQfinish(conn);
	CellsInRegion.clear();
	Point regionpoint=IdToCoordinate(regionminid);
	for(int i=0;i<(int)tempcellvector.size();i++){//从全部天线中选取在区域内的天线信息，存入CellsInRegion中
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
	cout<<"计算天线个数:"<<CellsInRegion.size()<<endl;
	return true;
}
Point Read::IdToCoordinate(UINT64 grid_id){//grid_id转为全局坐标
	Point temppoint;
	temppoint.x=((int)grid_id-1)/BIG_WIDTH;
	temppoint.y=((int)grid_id-1)%BIG_WIDTH;
	return temppoint;
}
bool Read::WriteFactors(std::string resultpathname){//写入指标名称
	ofstream fo(resultpathname.c_str(),'w');
	if (fo.is_open()==false){
		return false;
	}
	fo<<"天线名称"<<","<<"覆盖平均场强"<<","<<"主控平均场强"<<","<<"主控覆盖平均场强"<<","<<"主控边界平均场强"<<","<<"主控覆盖边界平均场强"<<","<<"主控道路平均场强"<<","<<"主控覆盖道路平均场强"\
		<<","<<"覆盖面积"<<","<<"主控面积"<<","<<"主控覆盖面积"<<","<<"最远覆盖距离"<<","<<"最远主控距离"<<","<<"最远主控覆盖距离"<<","<<"平均覆盖距离"<<","<<"平均主控距离"<<","<<"平均主控覆盖距离"\
		<<","<<"主控小区连续覆盖率"<<","<<"主控覆盖小区连续覆盖率"<<","<<"主控过覆盖率"<<","<<"主控覆盖过覆盖率"<<","<<"主控覆盖反向覆盖率"<<","<<"主控交叉覆盖率"<<","<<"主控覆盖交叉覆盖率"\
		<<","<<"主控与覆盖面积比"\
		<<","<<"主控覆盖与覆盖面积比"<<","<<"主控与覆盖距离比"<<","<<"主控覆盖与覆盖距离比"<<","<<"同频主控无干扰面积"<<","<<"同频主控有干扰面积"<<","<<"同频主控覆盖无干扰面积"<<","<<"同频主控覆盖有干扰面积"\
		<<","<<"异频主控无干扰面积"<<","<<"异频主控有干扰面积"<<","<<"异频主控覆盖无干扰面积"<<","<<"异频主控覆盖有干扰面积"<<","<<"同频覆盖非主控无干扰面积"<<","<<"同频覆盖非主控有干扰面积"\
		<<","<<"异频覆盖非主控无干扰面积"<<","<<"异频覆盖非主控有干扰面积"<<","<<"同频主控C\\I达标率"<<","<<"异频主控C\\I达标率"<<","<<"同频主控覆盖C\\I达标率"<<","<<"异频主控覆盖C\\I达标率"\
		<<","<<"同频主控道路C\\I达标率"<<","<<"异频主控道路C\\I达标率"<<","<<"同频主控覆盖道路C\\I达标率"<<","<<"异频主控覆盖道路C\\I达标率"<<","<<"MR加权TCH达标率"\
		<<","<<"同频主控平均重叠小区数"<<","<<"同频主控平均受干扰强度"<<","<<"异频主控平均重叠小区数"<<","<<"异频主控平均受干扰强度"\
		<<","<<"同频主控覆盖平均重叠小区数"<<","<<"同频主控覆盖平均受干扰强度"<<","<<"异频主控覆盖平均重叠小区数"<<","<<"异频主控覆盖平均受干扰强度"\
		<<","<<"同频主控底噪"<<","<<"异频主控底噪"<<","<<"同频主控覆盖底噪"<<","<<"异频主控覆盖底噪"\
		<<","<<"同频平均干扰其他小区数"<<","<<"同频平均干扰其他小区强度"<<","<<"异频平均干扰其他小区数"<<","<<"异频平均干扰其他小区强度"\
		<<","<<"基站布局偏差系数"<<","<<"基站高度偏差系数"\
		<<","<<"覆盖可视比例"<<","<<"覆盖一次NLOS比例"<<","<<"覆盖多次NLOS"\
		<<","<<"主控可视比例"<<","<<"主控一次NLOS比例"<<","<<"主控多次NLOS"\
		<<","<<"主控覆盖可视比例"<<","<<"主控覆盖一次NLOS比例"<<","<<"主控覆盖多次NLOS"<<endl;
	fo.close();
	//cout<<"true\n";
	return true;
}
bool Read::WriteFactors_another(std::string resultpathname){
	ofstream fo(resultpathname.c_str(),'w');
	if (fo.is_open()==false){
		return false;
	}
	fo<<"天线名称"<<","<<"覆盖平均场强"<<","<<"覆盖面积"<<","<<"最远覆盖距离"<<","<<"平均覆盖距离"<<","<<"MR加权TCH达标率"<<","\
		<<"主控平均场强"<<","<<"主控边界平均场强"<<","<<"主控道路平均场强"<<","<<"主控面积"<<","<<"最远主控距离"<<","<<"平均主控距离"<<","\
		<<"主控小区连续覆盖率"<<","<<"主控过覆盖率"<<","<<"主控交叉覆盖率"<<","<<"主控与覆盖面积比"<<","\
		<<"主控与覆盖距离比"<<","<<"同频主控无干扰面积"<<","<<"同频主控有干扰面积"<<","\
		<<"异频主控无干扰面积"<<","<<"异频主控有干扰面积"<<","<<"同频主控C\\I达标率"<<","<<"异频主控C\\I达标率"<<","\
		<<"同频主控道路C\\I达标率"<<","<<"异频主控道路C\\I达标率"<<","<<"同频主控平均重叠小区数"<<","<<"同频主控平均受干扰强度"<<","<<"异频主控平均重叠小区数"<<","<<"异频主控平均受干扰强度"<<","\
		<<"同频主控底噪"<<","<<"异频主控底噪"<<","<<"主控覆盖平均场强"<<","<<"主控覆盖边界平均场强"<<","<<"主控覆盖道路平均场强"<<","<<"主控覆盖面积"<<","<<"最远主控覆盖距离"<<","<<"平均主控覆盖距离"<<","\
		<<"主控覆盖小区连续覆盖率"<<","<<"主控覆盖过覆盖率"<<","<<"主控覆盖交叉覆盖率"<<","<<"主控覆盖反向覆盖率"<<","<<"主控覆盖与覆盖面积比"<<","<<"主控覆盖与覆盖距离比"<<","\
		<<"同频主控覆盖无干扰面积"<<","<<"同频主控覆盖有干扰面积"<<","<<"异频主控覆盖无干扰面积"<<","<<"异频主控覆盖有干扰面积"<<","<<"同频主控覆盖C\\I达标率"<<","<<"异频主控覆盖C\\I达标率"<<","\
		<<"同频主控覆盖道路C\\I达标率"<<","<<"异频主控覆盖道路C\\I达标率"<<","\
		<<"同频主控覆盖平均重叠小区数"<<","<<"同频主控覆盖平均受干扰强度"<<","<<"异频主控覆盖平均重叠小区数"<<","<<"异频主控覆盖平均受干扰强度"<<","\
		<<"同频主控覆盖底噪"<<","<<"异频主控覆盖底噪"<<","<<"同频覆盖非主控无干扰面积"<<","<<"同频覆盖非主控有干扰面积"<<","<<"异频覆盖非主控无干扰面积"<<","<<"异频覆盖非主控有干扰面积"<<","\
		<<"同频平均干扰其他小区数"<<","<<"同频平均干扰其他小区强度"<<","<<"异频平均干扰其他小区数"<<","<<"异频平均干扰其他小区强度"<<","\
		<<"基站布局偏差系数"<<","<<"基站高度偏差系数"\
		<<","<<"覆盖可视比例"<<","<<"覆盖一次NLOS比例"<<","<<"覆盖多次NLOS"\
		<<","<<"主控可视比例"<<","<<"主控一次NLOS比例"<<","<<"主控多次NLOS"\
		<<","<<"主控覆盖可视比例"<<","<<"主控覆盖一次NLOS比例"<<","<<"主控覆盖多次NLOS"<<endl;

	fo.close();
	return true;
}
bool Read::AddResultToFile(std::string resultpathname, Evaluation celleva){//写入指标值
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
