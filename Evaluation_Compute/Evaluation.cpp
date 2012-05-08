#include "StdAfx.h"
#include "Evaluation.h"
#include <time.h>

Evaluation::Evaluation(void)
{
	time="";
	height=0;
	width=0;
	frequency=0;
	
	CellsInRegion.clear();
	CellsAll.clear();
	CellsInfo_Region.clear();
	GridsInfo_Region.clear();

}

Evaluation::~Evaluation(void)
{
	CellsInRegion.clear();
	CellsAll.clear();
	CellsInfo_Region.clear();
	GridsInfo_Region.clear();
	//mainpower.clear();
	//gridtype.clear();
	//interpower.clear();
	//gridgeoinfo.clear();
}
//��ȡ�����ļ�
bool Evaluation::Readinfo(string compute_path_name){
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
			continue;
		}
		if(str.find("grid_tablename") != string::npos){
			int j=0;
			while(str[j]!=':'){
				j++;
			}
			j++;
			while(str[j]==' '){
				j++;
			}
			grid_tablename=str.substr(j,str.length()-j);
			cout<<"Grid Table is "<<grid_tablename<<endl;
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
			cout<<"Cell Table is "<<cellinfo_tablename<<endl;
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
			maincell_tablename=str.substr(j,str.length()-j);
		//	cout<<maincell_tablename<<endl;
			continue;
		}
		if(str.find("inter_tablename") != string::npos){
			int j=0;
			while(str[j]!=':'){
				j++;
			}
			j++;
			while(str[j]==' '){
				j++;
			}
			inter_tablename=str.substr(j,str.length()-j);
	//		cout<<inter_tablename<<endl;
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
			cout<<"Cell Type is "<<celltype<<endl;
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
			cout<<"Frequency Band is "<<frequency<<endl;
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
			//cout<<networksort<<endl;
			continue;
		}
		if(str.find("time")!=string::npos){
			int i=0;
			//�ҵ�ð�ź����һ����Ϊ�ո���ַ�λ��
			while(str[i]!=':'){//�ҵ�":"���ڵ�λ��
				i++;
			}
			i++;
			while(str[i]==' '){
				i++;
			}
			time=str.substr(i,str.length()-i);
			continue;
		}
		if(str.find("cellcover_path_name") != string::npos){
			unsigned j=0;
			while(str[j]!=':'){
				j++;
			}
			j++;
			while(str[j]==' '){
				j++;
			}
			cellcover_path_name=str.substr(j,str.length()-j);
			cout<<"Cell Coverage Info Storage Path is "<<cellcover_path_name<<endl;
			continue;
		}
		if(str.find("geo_path_name") != string::npos){
			unsigned j=0;
			while(str[j]!=':'){
				j++;
			}
			j++;
			while(str[j]==' '){
				j++;
			}
			geo_path_name=str.substr(j,str.length()-j);
			cout<<"Geo Info Storage Path is "<<geo_path_name<<endl;
			continue;
		}
		if(str.find("mrinfo_path_name") != string::npos){
			unsigned j=0;
			while(str[j]!=':'){
				j++;
			}
			j++;
			while(str[j]==' '){
				j++;
			}
			mrinfo_path_name=str.substr(j,str.length()-j);
		//	cout<<mrinfo_path_name<<endl;
			continue;
		}
		if(str.find("tchinter_neighbor_path_name") != string::npos){
			unsigned j=0;
			while(str[j]!=':'){
				j++;
			}
			j++;
			while(str[j]==' '){
				j++;
			}
			tchinter_neighbor_path_name=str.substr(j,str.length()-j);
		//	cout<<tchinter_neighbor_path_name<<endl;
			continue;
		}
		if(str.find("tchinter_same_path_name") != string::npos){
			unsigned j=0;
			while(str[j]!=':'){
				j++;
			}
			j++;
			while(str[j]==' '){
				j++;
			}
			tchinter_same_path_name=str.substr(j,str.length()-j);
	//		cout<<tchinter_same_path_name<<endl;
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
			cout<<"Evaluation Result will be stored at "<<result_path_name<<endl;
			continue;
		}
		if(str.find("region_info") != string::npos){
			unsigned i=0;
			while(str[i]!=':'){
				i++;
			}
			i++;
			int j;
			int index=0;//�ڼ������֣�0��minid��1��height��2��width��
			while(i<str.length()){
				while(str[i]==' '){
					i++;
				}
				j=i;//�������һ���ǿո�Ԫ�أ����֣���λ��
				while(str[i]!=' ' &&  i<str.length()){//Ѱ����һ���ո���ߵ����ַ�����β��
					i++;
				}
				switch(index){
					case 0:regionminid=atoi(str.substr(j,i-j).c_str()); break;
					case 1:height=atoi(str.substr(j,i-j).c_str()); break;
					case 2:width=atoi(str.substr(j,i-j).c_str()); break;
				}
				index++;
			}
			continue;
		}
		if(str.find("To_Big_Matrix") != string::npos){
			unsigned i=0;
			while(str[i]!=':'){
				i++;
			}
			i++;
			sToBigMatrix=str.substr(i,str.length()-i);
			continue;
		}
	}
	regionminid=max(regionminid,(UINT64)1);
	regionminpoint=IdToCoordinate(regionminid);
	fi.close();
//	cout<<" read successful\n";
	return true;
}
//������
bool Evaluation::Eval(){
	bool bToBigMatrix=false;
	
	if(sToBigMatrix=="true"){
		bToBigMatrix=true;
	}
		
	//*----��ʼ��GridsInfo_RegionΪheight*width��С----
	time_t start,end;
	GridsInfo_Region.clear();
	GridsInfo_Region.resize(height);
	for(int i=0;i<height;i++){
		GridsInfo_Region[i].gi.clear();;
		GridsInfo_Region[i].gi.resize(width);
	}
	//�����ݿ��е�gridtype�ֶ�д��GridInfo_Region��
	start=clock();
	if(!GetGridsType()){
		cout<<"database reading Grid Type process error\n";
		return false;
	}
	else{
		end=clock();
		cout<<"database reading Grid Type process complete in "<<difftime(end,start)<<" ms\n";
	}
	
	//�����ݿ��е�С����Ϣд��CellsAll��
	start=clock();
	if(!GetCells()){
		if(CellsAll.size()==0){//С���б�Ϊ�յĻ�����
			cout<<"database reading cells process error\n";
			return false;
		}
		else{
			cout<<"Very ODD Thing Happened\n";
			return false;
		}
	}
	else{
		end=clock();
		cout<<"database reading cells process complete in "<<difftime(end,start)<<" ms\n";
	}
	
	
	start=clock();
	//��С�������ļ��ж�ȡ���Ƿ������õĳ�ǿֵ����Ӧ��CellsInfo_RegionС����
	if(bToBigMatrix==false){
		if(!GetCells_CoverInfo()){
			cout<<"file reading Cell Cover process error\n";
			return false;
		}
		else{
			end=clock();
			cout<<"file reading Cell Cover process complete in "<<difftime(end,start)<<" ms\n";
		}

		//��CellsInfo_Region����GridsInfo_Region�е�cells
		if(!GetGridsInfo()){
			cout<<"Coversion From Cell Cover To Grids Info and Compute Grids KPI process error\n";
			return false;
		}
		else{
			cout<<"Coversion From Cell Cover To Grids Info and Compute Grids KPI process complete\n";
		}
	}
	else{
		if(!GetCells_CoverInfo_BigMatrix()){
			cout<<"file reading Cell Cover process error\n";
			return false;
		}
		else{
			end=clock();
			cout<<"file reading Cell Cover process complete in "<<difftime(end,start)<<" ms\n";
		}

		//����grid����KPI
		if(!GetGridsKPI_BigMatrix()){
			cout<<"Compute Grids KPI process error\n";
			return false;
		}
		else{
			cout<<"Compute Grids KPI process complete\n";
		}
	}
	return true;
}

//��ȡ���ݿ�
bool Evaluation::GetCells(){
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
	CellsAll.clear();
	//Point regionpoint=IdToCoordinate(regionminid);
	for(int i=0;i<(int)tempcellvector.size();i++){//��ȫ��������ѡȡ�ڶ�ӦƵ�κ����͵�������Ϣ������CellsAll��
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
			int j=0;
			tempcellinfo.tchno.clear();
			while(j<tempcellvector[i].tch.length()){
				if(tempcellvector[i].tch.substr(j,1)=="'"){
					j++;
				}
				else{
					int k=(int)tempcellvector[i].tch.find_first_of(",",j);
					if(j==k){
						j++;
					}
					else{
						int tchn=atoi(tempcellvector[i].tch.substr(j,k-j).c_str());
						tempcellinfo.tchno.push_back(tchn);
						j=k+1;
					}
				}
			}
			CellsAll.push_back(tempcellinfo);
			Point cellpoint=IdToCoordinate(tempcellvector[i].grid_id);
			if(cellpoint.x<regionminpoint.x||cellpoint.x>regionminpoint.x+height||cellpoint.y<regionminpoint.y||cellpoint.y>regionminpoint.y+width)
				continue;
			else
				CellsInRegion.push_back((int)CellsAll.size()-1);	
		}
	}
	tempcellvector.clear();
	cout<<"���ݿ���Ƶ��Ϊ"<<frequency<<"����Ϊ"<<celltype<<"�����߸���:"<<CellsAll.size()<<endl;
	cout<<"������Ƶ��Ϊ"<<frequency<<"����Ϊ"<<celltype<<"�����߸���:"<<CellsInRegion.size()<<endl;
	return true;
}
bool Evaluation::GetGridsType(){
	//gridtype.clear();
	//gridtype.resize(height);
	//for(int i=0;i<height;i++){
	//	gridtype[i].clear();
	//	gridtype[i].resize(width,-1);
	//}
	int pronum=omp_get_num_procs();//���Դ�����������Ĭ�ϲ����߳���
	vector<PGconn*> conn_array;//���ݿⲢ�е�����
	conn_array.clear();
	conn_array.resize(pronum);
	int conn_i=0;
	bool connect_flag=true;
#pragma omp parallel for
	for(conn_i=0;conn_i<pronum;conn_i++){
		PGconn*tempconn=PQconnectdb(connection.c_str());
		if (PQstatus(tempconn) != CONNECTION_OK){
			PQfinish(tempconn);
			connect_flag=false;
			continue;
		}
		conn_array[conn_i]=tempconn;
	}
	if(connect_flag==false){//��һ�����ݿ�û�д�
		cout<<"second:connection to the database failed"<<endl;
		return false;
	}
	//***********************************************************
	Point minpoint=IdToCoordinate(regionminid);
#pragma omp parallel for
	for(int i=0;i<height;i++){		
		char mingridchar[64];
		char maxgridchar[64];
		UINT64 mingridid=regionminid+i*BIG_WIDTH;
		UINT64 maxgridid=mingridid+width-1;
		sprintf_s(mingridchar,"%d",mingridid);
		sprintf_s(maxgridchar,"%d",maxgridid);
		int threadi=omp_get_thread_num();
		vector<Grid_Type> tempgrid_type;
		tempgrid_type.clear();		
		GridType tempgridtype;
		tempgridtype.SetConn(conn_array[threadi]);
		tempgridtype.SetDefault();
		tempgridtype.SetTableName(grid_tablename.c_str());
		tempgridtype.SelectTwoCondition(mingridchar,maxgridchar,tempgrid_type);
		for(int j=0;j<tempgrid_type.size();j++){
			Point temppoint=IdToCoordinate(tempgrid_type[j].grid_id);
			if(temppoint.x>=minpoint.x&&temppoint.x<minpoint.x+width&&temppoint.y>=minpoint.y&&temppoint.y<minpoint.y+height){
				//gridtype[temppoint.y-minpoint.y][temppoint.x-minpoint.x]=tempgrid_type[j].grid_type;
				GridsInfo_Region[temppoint.y-minpoint.y].gi[temppoint.x-minpoint.x].grid_id=tempgrid_type[j].grid_id;
				GridsInfo_Region[temppoint.y-minpoint.y].gi[temppoint.x-minpoint.x].grid_type=tempgrid_type[j].grid_type;
			}
		}
		tempgrid_type.clear();
	}
#pragma omp parallel for
	for(conn_i=0;conn_i<pronum;conn_i++){
		PQfinish(conn_array[conn_i]);
	}
	conn_array.clear();
	return true;
}
//��bin�ļ��ж�ȡ
bool Evaluation::GetCells_CoverInfo(){
//��ʼ��CellsInfo_Region��СΪС����
	CellsInfo_Region.clear();
	CellsInfo_Region.resize(CellsAll.size());	
#pragma omp parallel for
	for(int i=0;i<CellsAll.size();i++){
		CellsInfo_Region[i].grids.clear();
		GetCell_CoverInfo(CellsAll[i],i);
		GetGeoInformation(CellsAll[i],i);
//		cout<<"complete loaded cover info of cell "<<i+1<<"/"<<CellsAll.size()<<endl;
	}

return true;
}
bool Evaluation::GetCell_CoverInfo(Cellinfo cell,int index){
	CellsInfo_Region[index].minpoint.x=BIG_HEIGHT-1;
	CellsInfo_Region[index].minpoint.y=BIG_WIDTH-1;
	CellsInfo_Region[index].maxpoint.x=0;
	CellsInfo_Region[index].maxpoint.y=0;
	CellsInfo_Region[index].mingrid_id=0;
	CellsInfo_Region[index].cellid=cell.cell_id;
	
	string cellcover_name=cellcover_path_name+(string)"\\field_"+networksort+(string)"\\"+cell.cell_id+(string)".dat";
	ifstream fi;
	fi.open(cellcover_name.c_str(),ios_base::in|ios_base::binary);
	if(fi.is_open()==false){
		CellsInfo_Region[index].minpoint.x=0;
		CellsInfo_Region[index].minpoint.y=0;
		CellsInfo_Region[index].maxpoint.x=0;
		CellsInfo_Region[index].maxpoint.y=0;
		CellsInfo_Region[index].mingrid_id=0;
		return false;
	}
	//��field�ļ��Ķ�ȡ��ʽ
	/*Cover_GridPower tempgridpower;
	while (!fi.eof()){
		fi.read((char*)&tempgridpower,sizeof(tempgridpower));
		if(tempgridpower.grid_id==0)
			break;
		Point temppoint=IdToCoordinate(tempgridpower.grid_id);
		
		if(tempgridpower.receivepower>=RXLEVEL_MIN&&temppoint.x>=regionminpoint.x&&temppoint.x<=(regionminpoint.x+height)&&temppoint.y>=regionminpoint.y&&temppoint.y<=(regionminpoint.y+width)){
			CellsInfo_Region[index].minpoint.x=min(CellsInfo_Region[index].minpoint.x,temppoint.x);
			CellsInfo_Region[index].minpoint.y=min(CellsInfo_Region[index].minpoint.y,temppoint.y);
			CellsInfo_Region[index].maxpoint.x=max(CellsInfo_Region[index].maxpoint.x,temppoint.x);
			CellsInfo_Region[index].maxpoint.y=max(CellsInfo_Region[index].maxpoint.y,temppoint.y);
			Grid_Power_GeographyInfo tempgrid;
			tempgrid.grid_id=tempgridpower.grid_id;
			tempgrid.receivepower=tempgridpower.receivepower;
			CellsInfo_Region[index].grids.push_back(tempgrid);
		}
	}*/

	//��field�ļ��Ķ�ȡ����
	int gridnum=0;
	fi.read((char*)&gridnum,sizeof(gridnum));
	vector<Cover_GridPower> gridpower_vec;
	gridpower_vec.clear();
	gridpower_vec.resize(gridnum);
	fi.read((char*)&gridpower_vec[0],sizeof(gridpower_vec[0])*gridnum);
//#pragma  omp parallel for
	for (int i=0;i<gridnum;i++)
	{
		Point temppoint=IdToCoordinate(gridpower_vec[i].grid_id);
		if(gridpower_vec[i].receivepower>=RXLEVEL_MIN&&temppoint.x>=regionminpoint.x&&temppoint.x<=(regionminpoint.x+height)&&temppoint.y>=regionminpoint.y&&temppoint.y<=(regionminpoint.y+width)){
			CellsInfo_Region[index].minpoint.x=min(CellsInfo_Region[index].minpoint.x,temppoint.x);
			CellsInfo_Region[index].minpoint.y=min(CellsInfo_Region[index].minpoint.y,temppoint.y);
			CellsInfo_Region[index].maxpoint.x=max(CellsInfo_Region[index].maxpoint.x,temppoint.x);
			CellsInfo_Region[index].maxpoint.y=max(CellsInfo_Region[index].maxpoint.y,temppoint.y);
			Grid_Power_GeographyInfo tempgrid;
			tempgrid.grid_id=gridpower_vec[i].grid_id;
			tempgrid.receivepower=gridpower_vec[i].receivepower;
			CellsInfo_Region[index].grids.push_back(tempgrid);
		}
	}


	fi.close();
	CellsInfo_Region[index].minpoint.x=max(0,CellsInfo_Region[index].minpoint.x);
	CellsInfo_Region[index].minpoint.y=max(0,CellsInfo_Region[index].minpoint.y);
	CellsInfo_Region[index].maxpoint.x=min(BIG_HEIGHT-1,CellsInfo_Region[index].maxpoint.x);
	CellsInfo_Region[index].maxpoint.y=min(BIG_WIDTH-1,CellsInfo_Region[index].maxpoint.y);
	CellsInfo_Region[index].mingrid_id=CellsInfo_Region[index].minpoint.x*BIG_WIDTH+CellsInfo_Region[index].minpoint.y+1;
	
	return true;
}

bool Evaluation::GetGeoInformation(Cellinfo cell,int index){
	//gridgeoinfo.clear();
	//gridgeoinfo.resize(height);
	GeographyInfo tempgeo;
	tempgeo.grid_id=0;
	tempgeo.is_diffraction=-1;
	tempgeo.is_reflection=-1;
	tempgeo.is_visual=-1;
	//for(int i=0;i<height;i++){
	//	gridgeoinfo[i].clear();
	//	gridgeoinfo[i].resize(width,tempgeo);
	//}
	//typedef map<string,VEC>::iterator MCI;
	//for(MCI p=tempmap.begin();p!=tempmap.end();p++){
	//	string cellid=p->first;
	vector<GeographyInfo>tempgridgeo_vec;
	tempgridgeo_vec.clear();
		
	string geoinfo_name=geo_path_name+(string)"\\"+cell.cell_id+(string)".dat";
	ifstream fi(geoinfo_name.c_str(),ios::in|ios::binary);
	
	/*----------------------------------
	�˴�����Ч�ʽ�������ѡ���ļ�����vector tempgridgeo�У�Ȼ���ٰ�ÿһ��vector�е���Ŀ������е�cellinfo_region��ÿ��gridid�Ƿ���ͬ����д��
	��Ҫ���Ч�ʣ��裿����
	----------------------*/
	if(fi.is_open()==true){
		/*while(!fi.eof()){
			GeographyInfo tempgridgeo;
			fi.read((char*)&tempgridgeo,sizeof(tempgridgeo));
			tempgridgeo_vec.push_back(tempgridgeo);
		}*/
		//�µ�geo�ļ���ʽ�Ķ�ȡ����
		int gridgeonum=0;
		fi.read((char*)&gridgeonum,sizeof(gridgeonum));
		tempgridgeo_vec.resize(gridgeonum);
		fi.read((char*)&tempgridgeo_vec[0],sizeof(tempgridgeo_vec[0])*gridgeonum);
	}
	else{
		return false;
	}
	fi.close();
//#pragma  omp parallel for
	for(int i=0;i<(int)tempgridgeo_vec.size();i++){
		if(CellsInfo_Region[index].mingrid_id==0){
			//return false;
		}
		else{
			for(int j=0;j<(int)CellsInfo_Region[index].grids.size();j++){
				if(tempgridgeo_vec[i].grid_id==CellsInfo_Region[index].grids[j].grid_id){
					CellsInfo_Region[index].grids[j].is_diffraction=tempgridgeo_vec[i].is_diffraction;
					CellsInfo_Region[index].grids[j].is_reflection=tempgridgeo_vec[i].is_reflection;
					CellsInfo_Region[index].grids[j].is_visual=tempgridgeo_vec[i].is_visual;
				}
			}
		}
	}

		/****************************************************************
		Point minpoint=IdToCoordinate(regionminid);
		for(int i=0;i<(int)p->second.gridid_vec.size();i++){
			Point temppoint=IdToCoordinate(p->second.gridid_vec[i]);
			if(temppoint.x>=minpoint.x&&temppoint.x<minpoint.x+width&&temppoint.y>=minpoint.y&&temppoint.y<minpoint.y+height){
				gridgeoinfo[temppoint.y-minpoint.y][temppoint.x-minpoint.x].gridid=p->second.gridid_vec[i];
			}
		}
		//********************************************************************
		for(int i=0;i<(int)tempgridgeo_vec.size();i++){
			Point temppoint=IdToCoordinate(tempgridgeo_vec[i].gridid);
			if(temppoint.x>=minpoint.x&&temppoint.x<minpoint.x+width&&temppoint.y>=minpoint.y&&temppoint.y<minpoint.y+height\
				&&gridgeoinfo[temppoint.y-minpoint.y][temppoint.x-minpoint.x].gridid==tempgridgeo_vec[i].gridid){
				gridgeoinfo[temppoint.y-minpoint.y][temppoint.x-minpoint.x].is_visual=tempgridgeo_vec[i].is_visual;
				gridgeoinfo[temppoint.y-minpoint.y][temppoint.x-minpoint.x].is_reflection=tempgridgeo_vec[i].is_reflection;
				gridgeoinfo[temppoint.y-minpoint.y][temppoint.x-minpoint.x].is_diffraction=tempgridgeo_vec[i].is_diffraction;
			}
		}*/
	

	tempgridgeo_vec.clear();
		//p->second.gridid_vec.clear();
	//}
	return true;
}


bool Evaluation::GetCells_CoverInfo_BigMatrix(){
	time_t start,end;
	//��ʼ��CellsInfo_Region��СΪС����
	CellsInfo_Region.clear();
	CellsInfo_Region.resize(CellsAll.size());
#pragma omp parallel for
	for(int i=0;i<CellsAll.size();i++){
		start=clock();
		CellsInfo_Region[i].grids.clear();
		GetCell_CoverInfo_BigMatrix(CellsAll[i],i);
		GetGeoInformation_BigMatrix(CellsAll[i],i);
		end=clock();
		cout<<"complete loaded cover info of cell "<<i+1<<"/"<<CellsAll.size()<<" in "<<difftime(end,start)<<" ms"<<endl;
	}
	return true;
}
bool Evaluation::GetCell_CoverInfo_BigMatrix(Cellinfo cell,int index){
	CellsInfo_Region[index].minpoint.x=BIG_HEIGHT-1;
	CellsInfo_Region[index].minpoint.y=BIG_WIDTH-1;
	CellsInfo_Region[index].maxpoint.x=0;
	CellsInfo_Region[index].maxpoint.y=0;
	CellsInfo_Region[index].mingrid_id=0;
	CellsInfo_Region[index].cellid=cell.cell_id;
	
	string cellcover_name=cellcover_path_name+(string)"\\field_"+networksort+(string)"\\"+cell.cell_id+(string)".dat";
	ifstream fi;
	fi.open(cellcover_name.c_str(),ios_base::in|ios_base::binary);
	if(fi.is_open()==false){
		CellsInfo_Region[index].minpoint.x=0;
		CellsInfo_Region[index].minpoint.y=0;
		CellsInfo_Region[index].maxpoint.x=0;
		CellsInfo_Region[index].maxpoint.y=0;
		CellsInfo_Region[index].mingrid_id=0;
		return false;
	}
//��field�ļ��Ķ�ȡ����
//	Cover_GridPower tempgridpower;
//	while (!fi.eof()){
//		fi.read((char*)&tempgridpower,sizeof(tempgridpower));
//		if(tempgridpower.grid_id==0)
//			break;
//		Point temppoint=IdToCoordinate_Region(tempgridpower.grid_id,regionminid);
//		
//		if(tempgridpower.receivepower>=RXLEVEL_MIN&&temppoint.x>=0&&temppoint.x<width&&temppoint.y>=0&&temppoint.y<height){
//			CellsInfo_Region[index].minpoint.x=min(CellsInfo_Region[index].minpoint.x,temppoint.x);
//			CellsInfo_Region[index].minpoint.y=min(CellsInfo_Region[index].minpoint.y,temppoint.y);
//			CellsInfo_Region[index].maxpoint.x=max(CellsInfo_Region[index].maxpoint.x,temppoint.x);
//			CellsInfo_Region[index].maxpoint.y=max(CellsInfo_Region[index].maxpoint.y,temppoint.y);
//			
//			Grid_Power_GeographyInfo tempgrid;
//			tempgrid.grid_id=tempgridpower.grid_id;
//			tempgrid.receivepower=tempgridpower.receivepower;
//			CellsInfo_Region[index].grids.push_back(tempgrid);
//
//			Cells_Power_GeographyInfo tempcellinfo;
//			tempcellinfo.cellid=cell.cell_id;
//			tempcellinfo.receivepower=tempgridpower.receivepower;
//			tempcellinfo.isValid=true;
//#pragma omp critical
//			GridsInfo_Region[(int)temppoint.y].gi[(int)temppoint.x].cells.push_back(tempcellinfo);
//			
//			
//			//if(GridsInfo_Region[(int)temppoint.x].gi[(int)temppoint.y].cells.size()!=CELLID_MAX){
//			//	GridsInfo_Region[(int)temppoint.x].gi[(int)temppoint.y].cells.clear();
//			//	GridsInfo_Region[(int)temppoint.x].gi[(int)temppoint.y].cells.resize(CELLID_MAX);
//
//			//	for(int i=0;i<CELLID_MAX;i++){//isValid��ʼ��ΪʧЧ
//			//		GridsInfo_Region[temppoint.x].gi[temppoint.y].cells[i].isValid=false;
//			//	}
//			//}
//			//else{
//			//	int ncellid=atoi(cell.cell_id.c_str());
//			//	if(ncellid<=CELLID_MAX){
//			//		GridsInfo_Region[temppoint.x].gi[temppoint.y].cells[ncellid].receivepower=tempgridpower.receivepower;
//			//		GridsInfo_Region[temppoint.x].gi[temppoint.y].cells[ncellid].cellid=cell.cell_id;
//			//		GridsInfo_Region[temppoint.x].gi[temppoint.y].cells[ncellid].isValid=true;
//			//	}
//			//}
//		}
//	}

	//��field�ļ���ȡ����
	int gridnum=0;
	fi.read((char*)&gridnum,sizeof(gridnum));
	vector<Cover_GridPower> gridpower_vec;
	gridpower_vec.clear();
	gridpower_vec.resize(gridnum);
	fi.read((char*)&gridpower_vec[0],sizeof(gridpower_vec[0])*gridnum);
//#pragma  omp parallel for
	for (int i=0;i<gridnum;i++)
	{
		if(gridpower_vec[i].grid_id==0)
			continue;
		Point temppoint=IdToCoordinate(gridpower_vec[i].grid_id);
		
		if(gridpower_vec[i].receivepower>=RXLEVEL_MIN&&temppoint.x>=regionminpoint.x&&temppoint.x<=(regionminpoint.x+height)&&temppoint.y>=regionminpoint.y&&temppoint.y<=(regionminpoint.y+width)){
			CellsInfo_Region[index].minpoint.x=min(CellsInfo_Region[index].minpoint.x,temppoint.x);
			CellsInfo_Region[index].minpoint.y=min(CellsInfo_Region[index].minpoint.y,temppoint.y);
			CellsInfo_Region[index].maxpoint.x=max(CellsInfo_Region[index].maxpoint.x,temppoint.x);
			CellsInfo_Region[index].maxpoint.y=max(CellsInfo_Region[index].maxpoint.y,temppoint.y);
			
			Grid_Power_GeographyInfo tempgrid;
			tempgrid.grid_id=gridpower_vec[i].grid_id;
			tempgrid.receivepower=gridpower_vec[i].receivepower;
			CellsInfo_Region[index].grids.push_back(tempgrid);

			Cells_Power_GeographyInfo tempcellinfo;
			tempcellinfo.cellid=cell.cell_id;
			tempcellinfo.receivepower=gridpower_vec[i].receivepower;
			tempcellinfo.isValid=true;
#pragma omp critical
			GridsInfo_Region[(int)temppoint.y].gi[(int)temppoint.x].cells.push_back(tempcellinfo);
		}
	}

	fi.close();
	CellsInfo_Region[index].minpoint.x=max(0,CellsInfo_Region[index].minpoint.x);
	CellsInfo_Region[index].minpoint.y=max(0,CellsInfo_Region[index].minpoint.y);
	CellsInfo_Region[index].maxpoint.x=min(height,CellsInfo_Region[index].maxpoint.x);
	CellsInfo_Region[index].maxpoint.y=min(width,CellsInfo_Region[index].maxpoint.y);
	CellsInfo_Region[index].mingrid_id=regionminid+CellsInfo_Region[index].minpoint.x*BIG_WIDTH+CellsInfo_Region[index].minpoint.y+1;
	
	return true;
}

bool Evaluation::GetGeoInformation_BigMatrix(Cellinfo cell,int index){
		
	string geoinfo_name=geo_path_name+(string)"\\"+cell.cell_id+(string)".dat";
	ifstream fi(geoinfo_name.c_str(),ios::in|ios::binary);
	
	if(fi.is_open()==true){
		//while(!fi.eof()){
		//	GeographyInfo tempgridgeo;
		//	fi.read((char*)&tempgridgeo,sizeof(tempgridgeo));
		//	//tempgridgeo_vec.push_back(tempgridgeo);
		//	
		//	Point temppoint=IdToCoordinate_Region(tempgridgeo.grid_id,regionminid);
		//	if(temppoint.x>=CellsInfo_Region[index].minpoint.x&&temppoint.x<=CellsInfo_Region[index].maxpoint.x&&temppoint.y>=CellsInfo_Region[index].minpoint.y&&temppoint.y<=CellsInfo_Region[index].maxpoint.y){
		//		
		//		//����ѭ������cell��GridsInfo_Region[temppoint.x].gi[temppoint.y].cells�е�λ��
		//		//�����ֱ��Ѱַ�����ٶȿ��Դ��ӿ�
		//		for(int i=0;i<GridsInfo_Region[temppoint.y].gi[temppoint.x].cells.size();i++){
		//			if(GridsInfo_Region[temppoint.y].gi[temppoint.x].cells[i].cellid==cell.cell_id){
		//				GridsInfo_Region[temppoint.y].gi[temppoint.x].cells[i].is_visual=tempgridgeo.is_visual;
		//				GridsInfo_Region[temppoint.y].gi[temppoint.x].cells[i].is_reflection=tempgridgeo.is_reflection;
		//				GridsInfo_Region[temppoint.y].gi[temppoint.x].cells[i].is_diffraction=tempgridgeo.is_diffraction;
		//				break;
		//			}
		//		}
		//		
		//		/*int ncellid=atoi(cell.cell_id.c_str());
		//		if(ncellid<=CELLID_MAX){
		//			GridsInfo_Region[temppoint.x].gi[temppoint.y].cells[ncellid].is_visual=tempgridgeo.is_visual;
		//			GridsInfo_Region[temppoint.x].gi[temppoint.y].cells[ncellid].is_reflection=tempgridgeo.is_reflection;
		//			GridsInfo_Region[temppoint.x].gi[temppoint.y].cells[ncellid].is_diffraction=tempgridgeo.is_diffraction;
		//		}*/
		//	}
		//}

		int gridgeonum=0;
		fi.read((char*)&gridgeonum,sizeof(gridgeonum));
		vector<GeographyInfo> tempgridgeo_vec;
		tempgridgeo_vec.clear();
		tempgridgeo_vec.resize(gridgeonum);
		fi.read((char*)&tempgridgeo_vec[0],sizeof(tempgridgeo_vec[0])*gridgeonum);
		for(int i=0;i<(int)tempgridgeo_vec.size();i++){
			if(CellsInfo_Region[index].mingrid_id==0){
				//return false;
			}
			else{
				Point temppoint=IdToCoordinate_Region(tempgridgeo_vec[i].grid_id,regionminid);
				if(temppoint.x>=CellsInfo_Region[index].minpoint.x&&temppoint.x<=CellsInfo_Region[index].maxpoint.x&&temppoint.y>=CellsInfo_Region[index].minpoint.y&&temppoint.y<=CellsInfo_Region[index].maxpoint.y){
					
					//����ѭ������cell��GridsInfo_Region[temppoint.x].gi[temppoint.y].cells�е�λ��
					//�����ֱ��Ѱַ�����ٶȿ��Դ��ӿ�
					for(int i=0;i<GridsInfo_Region[temppoint.y].gi[temppoint.x].cells.size();i++){
						if(GridsInfo_Region[temppoint.y].gi[temppoint.x].cells[i].cellid==cell.cell_id){
							GridsInfo_Region[temppoint.y].gi[temppoint.x].cells[i].is_visual=tempgridgeo_vec[i].is_visual;
							GridsInfo_Region[temppoint.y].gi[temppoint.x].cells[i].is_reflection=tempgridgeo_vec[i].is_reflection;
							GridsInfo_Region[temppoint.y].gi[temppoint.x].cells[i].is_diffraction=tempgridgeo_vec[i].is_diffraction;
							break;
						}
					}
				}
			}
		}
	}
	else{
		return false;
	}
	fi.close();

	return true;
}
//��ʼ�������
bool Evaluation::GetGridsInfo(){
//�������ӵ���Ϣ����
#pragma omp parallel for
	for(int i=0;i<height;i++){
		time_t start,end;
		start=clock();
		for(int j=0;j<width;j++){
			GridsInfo_Region[i].gi[j].cells.clear();
			for(int k=0;k<CellsAll.size();k++){
				Point temppoint=IdToCoordinate(GridsInfo_Region[i].gi[j].grid_id);
				if(CellsInfo_Region[k].mingrid_id!=0&&temppoint.x>=CellsInfo_Region[k].minpoint.x&&temppoint.y>=CellsInfo_Region[k].minpoint.y&&temppoint.x<=CellsInfo_Region[k].maxpoint.x&&temppoint.y<=CellsInfo_Region[k].maxpoint.y){
					for(int l=0;l<CellsInfo_Region[k].grids.size();l++){
						if(CellsInfo_Region[k].grids[l].grid_id==GridsInfo_Region[i].gi[j].grid_id){
							Cells_Power_GeographyInfo tempcellinfo;
							tempcellinfo.cellid=CellsInfo_Region[k].cellid;
							tempcellinfo.receivepower=CellsInfo_Region[k].grids[l].receivepower;
							tempcellinfo.is_diffraction=CellsInfo_Region[k].grids[l].is_diffraction;
							tempcellinfo.is_reflection=CellsInfo_Region[k].grids[l].is_reflection;
							tempcellinfo.is_visual=CellsInfo_Region[k].grids[l].is_visual;
							
							GridsInfo_Region[i].gi[j].cells.push_back(tempcellinfo);
							break;
						}
					}
				}
			}

			GetGridsKPI(i,j);
		}
		end=clock();

		cout<<"gridInfo column "<<i+1<<" of "<<height<<" KPI is computed in "<<difftime(end,start)<<" ms"<<endl;
	}

	return true;
}
bool Evaluation::GetGridsKPI(int i,int j){

	GridsInfo_Region[i].gi[j].maincellpower=DEFAULT_POWER;
	GridsInfo_Region[i].gi[j].top1ncellpower=DEFAULT_POWER;
	GridsInfo_Region[i].gi[j].top2ncellpower=DEFAULT_POWER;
	GridsInfo_Region[i].gi[j].top3ncellpower=DEFAULT_POWER;
	GridsInfo_Region[i].gi[j].top4ncellpower=DEFAULT_POWER;
	GridsInfo_Region[i].gi[j].top5ncellpower=DEFAULT_POWER;
	GridsInfo_Region[i].gi[j].top6ncellpower=DEFAULT_POWER;
	for(int k=0;k<GridsInfo_Region[i].gi[j].cells.size();k++){
		if(GridsInfo_Region[i].gi[j].cells[k].receivepower>GridsInfo_Region[i].gi[j].maincellpower){
			GridsInfo_Region[i].gi[j].maincellpower=GridsInfo_Region[i].gi[j].cells[k].receivepower;
			GridsInfo_Region[i].gi[j].maincellid=GridsInfo_Region[i].gi[j].cells[k].cellid;
			GridsInfo_Region[i].gi[j].is_diffraction=GridsInfo_Region[i].gi[j].cells[k].is_diffraction;
			GridsInfo_Region[i].gi[j].is_reflection=GridsInfo_Region[i].gi[j].cells[k].is_reflection;
			GridsInfo_Region[i].gi[j].is_visual=GridsInfo_Region[i].gi[j].cells[k].is_visual;
		}
		else if(GridsInfo_Region[i].gi[j].cells[k].receivepower>GridsInfo_Region[i].gi[j].top1ncellpower){
			GridsInfo_Region[i].gi[j].top1ncellpower=GridsInfo_Region[i].gi[j].cells[k].receivepower;
			GridsInfo_Region[i].gi[j].top1ncellid=GridsInfo_Region[i].gi[j].cells[k].cellid;
		}
		else if(GridsInfo_Region[i].gi[j].cells[k].receivepower>GridsInfo_Region[i].gi[j].top2ncellpower){
			GridsInfo_Region[i].gi[j].top2ncellpower=GridsInfo_Region[i].gi[j].cells[k].receivepower;
			GridsInfo_Region[i].gi[j].top2ncellid=GridsInfo_Region[i].gi[j].cells[k].cellid;
		}
		else if(GridsInfo_Region[i].gi[j].cells[k].receivepower>GridsInfo_Region[i].gi[j].top3ncellpower){
			GridsInfo_Region[i].gi[j].top3ncellpower=GridsInfo_Region[i].gi[j].cells[k].receivepower;
			GridsInfo_Region[i].gi[j].top3ncellid=GridsInfo_Region[i].gi[j].cells[k].cellid;
		}
		else if(GridsInfo_Region[i].gi[j].cells[k].receivepower>GridsInfo_Region[i].gi[j].top4ncellpower){
			GridsInfo_Region[i].gi[j].top4ncellpower=GridsInfo_Region[i].gi[j].cells[k].receivepower;
			GridsInfo_Region[i].gi[j].top4ncellid=GridsInfo_Region[i].gi[j].cells[k].cellid;
		}
		else if(GridsInfo_Region[i].gi[j].cells[k].receivepower>GridsInfo_Region[i].gi[j].top5ncellpower){
			GridsInfo_Region[i].gi[j].top5ncellpower=GridsInfo_Region[i].gi[j].cells[k].receivepower;
			GridsInfo_Region[i].gi[j].top5ncellid=GridsInfo_Region[i].gi[j].cells[k].cellid;
		}
		else if(GridsInfo_Region[i].gi[j].cells[k].receivepower>GridsInfo_Region[i].gi[j].top5ncellpower){
			GridsInfo_Region[i].gi[j].top5ncellpower=GridsInfo_Region[i].gi[j].cells[k].receivepower;
			GridsInfo_Region[i].gi[j].top5ncellid=GridsInfo_Region[i].gi[j].cells[k].cellid;
		}
		else{
			//continue;
		}
	}
			
	GridsInfo_Region[i].gi[j].isBorder=false;
	GridsInfo_Region[i].gi[j].isIndoorOutdoorMargin=false;

	if(i==0||j==0||i==(height-1)||j==(width-1)){
		GridsInfo_Region[i].gi[j].isBorder=true;
		GridsInfo_Region[i].gi[j].isIndoorOutdoorMargin=false;
	}
	else{
		if(GridsInfo_Region[i].gi[j].maincellid!=GridsInfo_Region[i+1].gi[j].maincellid||GridsInfo_Region[i].gi[j].maincellid!=GridsInfo_Region[i].gi[j+1].maincellid||GridsInfo_Region[i].gi[j].maincellid!=GridsInfo_Region[i-1].gi[j].maincellid||GridsInfo_Region[i].gi[j].maincellid!=GridsInfo_Region[i].gi[j-1].maincellid||GridsInfo_Region[i].gi[j].maincellid!=GridsInfo_Region[i+1].gi[j+1].maincellid||GridsInfo_Region[i].gi[j].maincellid!=GridsInfo_Region[i-1].gi[j-1].maincellid||GridsInfo_Region[i].gi[j].maincellid!=GridsInfo_Region[i-1].gi[j+1].maincellid||GridsInfo_Region[i].gi[j].maincellid!=GridsInfo_Region[i+1].gi[j-1].maincellid){
			GridsInfo_Region[i].gi[j].isBorder=true;
		}
		if(GridsInfo_Region[i].gi[j].grid_type==GRID_TYPE_INDOOR&&(GridsInfo_Region[i+1].gi[j].grid_type!=GRID_TYPE_INDOOR||GridsInfo_Region[i].gi[j+1].grid_type!=GRID_TYPE_INDOOR||GridsInfo_Region[i-1].gi[j].grid_type!=GRID_TYPE_INDOOR||GridsInfo_Region[i].gi[j-1].grid_type!=GRID_TYPE_INDOOR||GridsInfo_Region[i+1].gi[j+1].grid_type!=GRID_TYPE_INDOOR||GridsInfo_Region[i-1].gi[j-1].grid_type!=GRID_TYPE_INDOOR||GridsInfo_Region[i+1].gi[j-1].grid_type!=GRID_TYPE_INDOOR||GridsInfo_Region[i-1].gi[j+1].grid_type!=GRID_TYPE_INDOOR)){
			GridsInfo_Region[i].gi[j].isIndoorOutdoorMargin=true;
		}			
	}

	GridsInfo_Region[i].gi[j].Same_inter_power=DEFAULT_POWER;
	GridsInfo_Region[i].gi[j].Same_inter_num=0;
	GridsInfo_Region[i].gi[j].Diff_inter_power=DEFAULT_POWER;
	GridsInfo_Region[i].gi[j].Diff_inter_num=0;
	GridsInfo_Region[i].gi[j].TCH_Same_Num=0;
	GridsInfo_Region[i].gi[j].TCH_Neighbor_Num=0;
		
	for(int k=0;k<GridsInfo_Region[i].gi[j].cells.size();k++){
		
		if(GridsInfo_Region[i].gi[j].cells[k].cellid!=GridsInfo_Region[i].gi[j].maincellid){

			GridsInfo_Region[i].gi[j].Same_inter_power=PowerAdd(GridsInfo_Region[i].gi[j].Same_inter_power,GridsInfo_Region[i].gi[j].cells[k].receivepower);
			
			if(GridsInfo_Region[i].gi[j].maincellpower<(GridsInfo_Region[i].gi[j].cells[k].receivepower+12)){
				GridsInfo_Region[i].gi[j].Same_inter_num++;
			}	
			
			if(IsFreqSameBCCH(GridsInfo_Region[i].gi[j].cells[k].cellid,GridsInfo_Region[i].gi[j].maincellid)){
				GridsInfo_Region[i].gi[j].Diff_inter_power=PowerAdd(GridsInfo_Region[i].gi[j].Diff_inter_power,GridsInfo_Region[i].gi[j].cells[k].receivepower);
				
				if(GridsInfo_Region[i].gi[j].maincellpower<(GridsInfo_Region[i].gi[j].cells[k].receivepower+INTERF_MARGIN)){
					GridsInfo_Region[i].gi[j].Diff_inter_num++;
				}
			}
			vector<int> tchlist;
			vector<int> tchSameList;
			vector<int> tchNeighbourList;
			tchlist.clear();
			tchSameList.clear();
			tchNeighbourList.clear();
			
			for(int l=0;l<CellsAll.size();l++){
				if(GridsInfo_Region[i].gi[j].maincellid==CellsAll[l].cell_id){
					tchlist.resize(CellsAll[l].tchno.size());
					for(int m=0;m<CellsAll[l].tchno.size();m++){
						tchlist.push_back(CellsAll[l].tchno[m]);
					}
				}
			}

			GridsInfo_Region[i].gi[j].TCH_Same_Vec.resize(tchlist.size());
			for(int l=0;l<GridsInfo_Region[i].gi[j].TCH_Same_Vec.size();l++){
				GridsInfo_Region[i].gi[j].TCH_Same_Vec[l]=DEFAULT_POWER;
			}
			GridsInfo_Region[i].gi[j].TCH_Neighbor_Vec.resize(tchlist.size());
			for(int l=0;l<GridsInfo_Region[i].gi[j].TCH_Neighbor_Vec.size();l++){
				GridsInfo_Region[i].gi[j].TCH_Neighbor_Vec[l]=DEFAULT_POWER;
			}

			if(ListOfFreqsSameTCH(GridsInfo_Region[i].gi[j].maincellid,GridsInfo_Region[i].gi[j].cells[k].cellid,tchSameList)){
				for(int n=0;n<tchSameList.size();n++){
					for(int m=0;m<tchlist.size();m++){
						if(tchSameList[n]==tchlist[m]){
							GridsInfo_Region[i].gi[j].TCH_Same_Vec[m]=PowerAdd(GridsInfo_Region[i].gi[j].TCH_Same_Vec[m],GridsInfo_Region[i].gi[j].cells[k].receivepower);
						}
					}
				}
				if(GridsInfo_Region[i].gi[j].maincellpower<(GridsInfo_Region[i].gi[j].cells[k].receivepower+INTERF_MARGIN)){
					GridsInfo_Region[i].gi[j].TCH_Same_Num++;
				}
			}

			if(ListOfFreqsNeighbourTCH(GridsInfo_Region[i].gi[j].maincellid,GridsInfo_Region[i].gi[j].cells[k].cellid,tchNeighbourList)){
				for(int n=0;n<tchNeighbourList.size();n++){
					for(int m=0;m<tchlist.size();m++){
						if(tchNeighbourList[n]==tchlist[m]){
							GridsInfo_Region[i].gi[j].TCH_Neighbor_Vec[m]=PowerAdd(GridsInfo_Region[i].gi[j].TCH_Neighbor_Vec[m],GridsInfo_Region[i].gi[j].cells[k].receivepower);
						}
					}
				}
				if(GridsInfo_Region[i].gi[j].maincellpower<(GridsInfo_Region[i].gi[j].cells[k].receivepower+INTERF_MARGIN_NEIGHBOR)){
					GridsInfo_Region[i].gi[j].TCH_Neighbor_Num++;
				}
			}
		}		
	}

	return true;
}




bool Evaluation::GetCellsKPI(){
	//�������ӵ���Ϣ����
#pragma omp parallel for
	for(int i=0;i<CellsAll.size();i++){

	}
}
bool Evaluation::GetRegionKPI(){
	
	result_region.cover_ratio=0;
	result_region.cover_indoor_outdoor_ratio=0;
	result_region.cover_road_ratio=0;
	result_region.Diff_cover_rate=0;
	result_region.Diff_cover_rate_road=0;
	result_region.main_LOS=0;
	result_region.main_NLOS=0;
	result_region.main_NLOS_1=0;
	result_region.Same_cover_rate=0;
	result_region.Same_cover_rate_road=0;
	result_region.Same_C2I_rate=0;
	result_region.Diff_C2I_rate=0;
	result_region.TCH_C2I_rate=0;
	result_region.Same_C2I_rate_road=0;
	result_region.Diff_C2I_rate_road=0;
	result_region.TCH_C2I_rate_road=0;
	
	int road_area=0;
	int indoor_outdoor_margin_area=0;

	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){

			result_region.Diff_cover_rate+=GridsInfo_Region[i].gi[j].Diff_inter_num;
			result_region.Same_cover_rate+=GridsInfo_Region[i].gi[j].Same_inter_num;

			if(GridsInfo_Region[i].gi[j].maincellpower>=POOR_COVER_MARGIN){
				//������
				result_region.cover_ratio++;
			}

			if(GridsInfo_Region[i].gi[j].is_visual==1){
				result_region.main_LOS++;
			}
			else if(GridsInfo_Region[i].gi[j].is_reflection==1||GridsInfo_Region[i].gi[j].is_diffraction==1){
				result_region.main_NLOS_1++;
			}
			else{
				result_region.main_NLOS++;
			}
			
			if(GridsInfo_Region[i].gi[j].maincellpower>=(GridsInfo_Region[i].gi[j].Same_inter_power+INTERF_MARGIN)){
				//ͬƵ C/I���
				result_region.Same_C2I_rate++;
				if(GridsInfo_Region[i].gi[j].grid_type==GRID_TYPE_ROAD){
					result_region.Same_C2I_rate_road++;
				}
			}

			if(GridsInfo_Region[i].gi[j].maincellpower>=(GridsInfo_Region[i].gi[j].Diff_inter_power+INTERF_MARGIN)){
				//BCCH C/I���
				result_region.Diff_C2I_rate++;
				if(GridsInfo_Region[i].gi[j].grid_type==GRID_TYPE_ROAD){
					result_region.Diff_C2I_rate_road++;
				}
			}
			
			//for(int m=0;m<GridsInfo_Region[i][j].TCH_Same_Vec.size();m++){
				//TCH C/I���
				//if(GridsInfo_Region[i][j].maincellpower>=
			//}

			if(GridsInfo_Region[i].gi[j].grid_type==GRID_TYPE_ROAD){
				road_area++;
				if(GridsInfo_Region[i].gi[j].maincellpower>=POOR_COVER_MARGIN){
					//��·������
					result_region.cover_road_ratio++;
				}
				result_region.Same_cover_rate_road+=GridsInfo_Region[i].gi[j].Same_inter_num;
				result_region.Diff_cover_rate_road+=GridsInfo_Region[i].gi[j].Diff_inter_num;
			}

			
			if(GridsInfo_Region[i].gi[j].isIndoorOutdoorMargin=true){
				indoor_outdoor_margin_area++;
				if(GridsInfo_Region[i].gi[j].maincellpower>=POOR_COVER_MARGIN){
					//�����⽻�Ӵ�������
					result_region.cover_indoor_outdoor_ratio++;
				}
				
			}
		}
	}
	int area=height*width;
	
	if(area>0){
		result_region.cover_ratio=result_region.cover_ratio/(float)area;
		result_region.Diff_cover_rate=result_region.Diff_cover_rate/(float)area;
		result_region.Same_cover_rate=result_region.Same_cover_rate/(float)area;
		result_region.Same_C2I_rate=result_region.Same_C2I_rate/(float)area;
		result_region.Diff_C2I_rate=result_region.Diff_C2I_rate/(float)area;
		result_region.TCH_C2I_rate=result_region.TCH_C2I_rate/(float)area;
		result_region.main_LOS=result_region.main_LOS/(float)area;
		result_region.main_NLOS=result_region.main_NLOS/(float)area;
		result_region.main_NLOS_1=result_region.main_NLOS_1/(float)area;
	}
	if(road_area>0){
		result_region.cover_road_ratio=result_region.cover_road_ratio/(float)road_area;
		result_region.Same_cover_rate_road=result_region.Same_cover_rate_road/(float)road_area;
		result_region.Diff_cover_rate_road=result_region.Diff_cover_rate_road/(float)road_area;
		result_region.Same_C2I_rate_road=result_region.Same_C2I_rate_road/(float)road_area;
		result_region.Diff_C2I_rate_road=result_region.Diff_C2I_rate_road/(float)road_area;
		result_region.TCH_C2I_rate_road=result_region.TCH_C2I_rate_road/(float)road_area;
	}
	if(indoor_outdoor_margin_area>0){
		result_region.cover_indoor_outdoor_ratio=result_region.cover_indoor_outdoor_ratio/(float)indoor_outdoor_margin_area;
	}
	return true;

}

bool Evaluation::GetGridsKPI_BigMatrix(){
#pragma omp parallel for
	for(int i=0;i<height;i++){
		time_t start,end;
		start=clock();
		for(int j=0;j<width;j++){
			GridsInfo_Region[i].gi[j].maincellpower=DEFAULT_POWER;
			GridsInfo_Region[i].gi[j].top1ncellpower=DEFAULT_POWER;
			GridsInfo_Region[i].gi[j].top2ncellpower=DEFAULT_POWER;
			GridsInfo_Region[i].gi[j].top3ncellpower=DEFAULT_POWER;
			GridsInfo_Region[i].gi[j].top4ncellpower=DEFAULT_POWER;
			GridsInfo_Region[i].gi[j].top5ncellpower=DEFAULT_POWER;
			GridsInfo_Region[i].gi[j].top6ncellpower=DEFAULT_POWER;
			for(int k=0;k<GridsInfo_Region[i].gi[j].cells.size();k++){
				if(GridsInfo_Region[i].gi[j].cells[k].isValid==true){
					if(GridsInfo_Region[i].gi[j].cells[k].receivepower>GridsInfo_Region[i].gi[j].maincellpower){
						GridsInfo_Region[i].gi[j].maincellpower=GridsInfo_Region[i].gi[j].cells[k].receivepower;
						GridsInfo_Region[i].gi[j].maincellid=GridsInfo_Region[i].gi[j].cells[k].cellid;
						GridsInfo_Region[i].gi[j].is_diffraction=GridsInfo_Region[i].gi[j].cells[k].is_diffraction;
						GridsInfo_Region[i].gi[j].is_reflection=GridsInfo_Region[i].gi[j].cells[k].is_reflection;
						GridsInfo_Region[i].gi[j].is_visual=GridsInfo_Region[i].gi[j].cells[k].is_visual;
					}
					else if(GridsInfo_Region[i].gi[j].cells[k].receivepower>GridsInfo_Region[i].gi[j].top1ncellpower){
						GridsInfo_Region[i].gi[j].top1ncellpower=GridsInfo_Region[i].gi[j].cells[k].receivepower;
						GridsInfo_Region[i].gi[j].top1ncellid=GridsInfo_Region[i].gi[j].cells[k].cellid;
					}
					else if(GridsInfo_Region[i].gi[j].cells[k].receivepower>GridsInfo_Region[i].gi[j].top2ncellpower){
						GridsInfo_Region[i].gi[j].top2ncellpower=GridsInfo_Region[i].gi[j].cells[k].receivepower;
						GridsInfo_Region[i].gi[j].top2ncellid=GridsInfo_Region[i].gi[j].cells[k].cellid;
					}
					else if(GridsInfo_Region[i].gi[j].cells[k].receivepower>GridsInfo_Region[i].gi[j].top3ncellpower){
						GridsInfo_Region[i].gi[j].top3ncellpower=GridsInfo_Region[i].gi[j].cells[k].receivepower;
						GridsInfo_Region[i].gi[j].top3ncellid=GridsInfo_Region[i].gi[j].cells[k].cellid;
					}
					else if(GridsInfo_Region[i].gi[j].cells[k].receivepower>GridsInfo_Region[i].gi[j].top4ncellpower){
						GridsInfo_Region[i].gi[j].top4ncellpower=GridsInfo_Region[i].gi[j].cells[k].receivepower;
						GridsInfo_Region[i].gi[j].top4ncellid=GridsInfo_Region[i].gi[j].cells[k].cellid;
					}
					else if(GridsInfo_Region[i].gi[j].cells[k].receivepower>GridsInfo_Region[i].gi[j].top5ncellpower){
						GridsInfo_Region[i].gi[j].top5ncellpower=GridsInfo_Region[i].gi[j].cells[k].receivepower;
						GridsInfo_Region[i].gi[j].top5ncellid=GridsInfo_Region[i].gi[j].cells[k].cellid;
					}
					else if(GridsInfo_Region[i].gi[j].cells[k].receivepower>GridsInfo_Region[i].gi[j].top5ncellpower){
						GridsInfo_Region[i].gi[j].top5ncellpower=GridsInfo_Region[i].gi[j].cells[k].receivepower;
						GridsInfo_Region[i].gi[j].top5ncellid=GridsInfo_Region[i].gi[j].cells[k].cellid;
					}
					else{
						//continue;
					}
				}
			}
					
			GridsInfo_Region[i].gi[j].isBorder=false;
			GridsInfo_Region[i].gi[j].isIndoorOutdoorMargin=false;

			if(i==0||j==0||i==(height-1)||j==(width-1)){
				GridsInfo_Region[i].gi[j].isBorder=true;
				GridsInfo_Region[i].gi[j].isIndoorOutdoorMargin=false;
			}
			else{
				if(GridsInfo_Region[i].gi[j].maincellid!=GridsInfo_Region[i+1].gi[j].maincellid||GridsInfo_Region[i].gi[j].maincellid!=GridsInfo_Region[i].gi[j+1].maincellid||GridsInfo_Region[i].gi[j].maincellid!=GridsInfo_Region[i-1].gi[j].maincellid||GridsInfo_Region[i].gi[j].maincellid!=GridsInfo_Region[i].gi[j-1].maincellid||GridsInfo_Region[i].gi[j].maincellid!=GridsInfo_Region[i+1].gi[j+1].maincellid||GridsInfo_Region[i].gi[j].maincellid!=GridsInfo_Region[i-1].gi[j-1].maincellid||GridsInfo_Region[i].gi[j].maincellid!=GridsInfo_Region[i-1].gi[j+1].maincellid||GridsInfo_Region[i].gi[j].maincellid!=GridsInfo_Region[i+1].gi[j-1].maincellid){
					GridsInfo_Region[i].gi[j].isBorder=true;
				}
				if(GridsInfo_Region[i].gi[j].grid_type==GRID_TYPE_INDOOR&&(GridsInfo_Region[i+1].gi[j].grid_type!=GRID_TYPE_INDOOR||GridsInfo_Region[i].gi[j+1].grid_type!=GRID_TYPE_INDOOR||GridsInfo_Region[i-1].gi[j].grid_type!=GRID_TYPE_INDOOR||GridsInfo_Region[i].gi[j-1].grid_type!=GRID_TYPE_INDOOR||GridsInfo_Region[i+1].gi[j+1].grid_type!=GRID_TYPE_INDOOR||GridsInfo_Region[i-1].gi[j-1].grid_type!=GRID_TYPE_INDOOR||GridsInfo_Region[i+1].gi[j-1].grid_type!=GRID_TYPE_INDOOR||GridsInfo_Region[i-1].gi[j+1].grid_type!=GRID_TYPE_INDOOR)){
					GridsInfo_Region[i].gi[j].isIndoorOutdoorMargin=true;
				}			
			}

			GridsInfo_Region[i].gi[j].Same_inter_power=DEFAULT_POWER;
			GridsInfo_Region[i].gi[j].Same_inter_num=0;
			GridsInfo_Region[i].gi[j].Diff_inter_power=DEFAULT_POWER;
			GridsInfo_Region[i].gi[j].Diff_inter_num=0;
			GridsInfo_Region[i].gi[j].TCH_Same_Num=0;
			GridsInfo_Region[i].gi[j].TCH_Neighbor_Num=0;
				
			for(int k=0;k<GridsInfo_Region[i].gi[j].cells.size();k++){
				if(GridsInfo_Region[i].gi[j].cells[k].isValid==true){
					if(GridsInfo_Region[i].gi[j].cells[k].cellid!=GridsInfo_Region[i].gi[j].maincellid){

						GridsInfo_Region[i].gi[j].Same_inter_power=PowerAdd(GridsInfo_Region[i].gi[j].Same_inter_power,GridsInfo_Region[i].gi[j].cells[k].receivepower);
						
						if(GridsInfo_Region[i].gi[j].maincellpower<(GridsInfo_Region[i].gi[j].cells[k].receivepower+12)){
							GridsInfo_Region[i].gi[j].Same_inter_num++;
						}	
						
						if(IsFreqSameBCCH(GridsInfo_Region[i].gi[j].cells[k].cellid,GridsInfo_Region[i].gi[j].maincellid)){
							GridsInfo_Region[i].gi[j].Diff_inter_power=PowerAdd(GridsInfo_Region[i].gi[j].Diff_inter_power,GridsInfo_Region[i].gi[j].cells[k].receivepower);
							
							if(GridsInfo_Region[i].gi[j].maincellpower<(GridsInfo_Region[i].gi[j].cells[k].receivepower+INTERF_MARGIN)){
								GridsInfo_Region[i].gi[j].Diff_inter_num++;
							}
						}
						vector<int> tchlist;
						vector<int> tchSameList;
						vector<int> tchNeighbourList;
						tchlist.clear();
						tchSameList.clear();
						tchNeighbourList.clear();
						
						for(int l=0;l<CellsAll.size();l++){
							if(GridsInfo_Region[i].gi[j].maincellid==CellsAll[l].cell_id){
								tchlist.resize(CellsAll[l].tchno.size());
								for(int m=0;m<CellsAll[l].tchno.size();m++){
									tchlist.push_back(CellsAll[l].tchno[m]);
								}
							}
						}

						GridsInfo_Region[i].gi[j].TCH_Same_Vec.resize(tchlist.size());
						for(int l=0;l<GridsInfo_Region[i].gi[j].TCH_Same_Vec.size();l++){
							GridsInfo_Region[i].gi[j].TCH_Same_Vec[l]=DEFAULT_POWER;
						}
						GridsInfo_Region[i].gi[j].TCH_Neighbor_Vec.resize(tchlist.size());
						for(int l=0;l<GridsInfo_Region[i].gi[j].TCH_Neighbor_Vec.size();l++){
							GridsInfo_Region[i].gi[j].TCH_Neighbor_Vec[l]=DEFAULT_POWER;
						}

						if(ListOfFreqsSameTCH(GridsInfo_Region[i].gi[j].maincellid,GridsInfo_Region[i].gi[j].cells[k].cellid,tchSameList)){
							for(int n=0;n<tchSameList.size();n++){
								for(int m=0;m<tchlist.size();m++){
									if(tchSameList[n]==tchlist[m]){
										GridsInfo_Region[i].gi[j].TCH_Same_Vec[m]=PowerAdd(GridsInfo_Region[i].gi[j].TCH_Same_Vec[m],GridsInfo_Region[i].gi[j].cells[k].receivepower);
									}
								}
							}
							if(GridsInfo_Region[i].gi[j].maincellpower<(GridsInfo_Region[i].gi[j].cells[k].receivepower+INTERF_MARGIN)){
								GridsInfo_Region[i].gi[j].TCH_Same_Num++;
							}
						}

						if(ListOfFreqsNeighbourTCH(GridsInfo_Region[i].gi[j].maincellid,GridsInfo_Region[i].gi[j].cells[k].cellid,tchNeighbourList)){
							for(int n=0;n<tchNeighbourList.size();n++){
								for(int m=0;m<tchlist.size();m++){
									if(tchNeighbourList[n]==tchlist[m]){
										GridsInfo_Region[i].gi[j].TCH_Neighbor_Vec[m]=PowerAdd(GridsInfo_Region[i].gi[j].TCH_Neighbor_Vec[m],GridsInfo_Region[i].gi[j].cells[k].receivepower);
									}
								}
							}
							if(GridsInfo_Region[i].gi[j].maincellpower<(GridsInfo_Region[i].gi[j].cells[k].receivepower+INTERF_MARGIN_NEIGHBOR)){
								GridsInfo_Region[i].gi[j].TCH_Neighbor_Num++;
							}
						}
					}
				}		
			}
		}
		end=clock();

		cout<<"gridInfo column "<<i+1<<" of "<<height<<" KPI is computed in "<<difftime(end,start)<<" ms"<<endl;
	}

	return true;
}
//������
bool Evaluation::WriteRegionResult(string temp_result_path_name){
	ofstream fo(temp_result_path_name.c_str(),'w');
	if(fo.is_open()==true){
		fo<<"������"<<","<<"��·������"<<","<<"�����⽻�紦������"<<","\
			<<"LOS����"<<","<<"NLOS_1����"<<","<<"NLOS����"<<","\
			<<"ƽ��ͬƵ�ص����Ƕ�"<<","<<"��·ƽ��ͬƵ�ص�������"<<","<<"ƽ��BCCHͬƵ�ص����Ƕ�"<<","<<"��·ƽ��BCCHͬƵ�ص����Ƕ�"<<","\
			<<"������ͬƵC/I�����"<<","<<"������BCCH C/I�����"<<","<<"������TCH C/I�����"<<","\
			<<"�����·ͬƵC/I�����"<<","<<"�����·BCCH C/I�����"<<","<<"�����·TCH C/I�����"<<","<<endl;
		fo<<result_region.cover_ratio<<","<<result_region.cover_road_ratio<<","<<result_region.cover_indoor_outdoor_ratio<<","\
			<<result_region.main_LOS<<","<<result_region.main_NLOS_1<<","<<result_region.main_NLOS<<","\
			<<result_region.Same_cover_rate<<","<<result_region.Same_cover_rate_road<<","<<result_region.Diff_cover_rate<<","<<result_region.Diff_cover_rate_road<<","\
			<<result_region.Same_C2I_rate<<","<<result_region.Diff_C2I_rate<<","<<result_region.TCH_C2I_rate<<","\
			<<result_region.Same_C2I_rate_road<<","<<result_region.Diff_C2I_rate_road<<","<<result_region.TCH_C2I_rate_road<<endl;
	}
	fo.close();
	return true;
}


bool Evaluation::WriteCellsResult(std::string resultpathname){//д��ָ������
	ofstream fo(resultpathname.c_str(),'w');
	if (fo.is_open()==false){
		return false;
	}
	/*
	fo<<"��������"<<","\
		
		<<"����ƽ����ǿ"<<","<<"����ƽ����ǿ"<<","<<"���ر߽�ƽ����ǿ"<<"���ص�·ƽ����ǿ"<<","<<"���ظ��ǵ�·ƽ����ǿ"\
		<<","<<"�������"<<","<<"�������"<<","<<"���ظ������"<<","<<"��Զ���Ǿ���"<<","<<"��Զ���ؾ���"<<","<<"��Զ���ظ��Ǿ���"<<","<<"ƽ�����Ǿ���"<<","<<"ƽ�����ؾ���"<<","<<"ƽ�����ظ��Ǿ���"\
		<<","<<"��������������"<<","<<"���ع�������"<<","<<"���ط��򸲸���"<<","<<"���ؽ��渲����"\
		<<","<<"�����븲�������"<<","<<"���ظ����븲�������"<<","<<"�����븲�Ǿ����"<<","<<"���ظ����븲�Ǿ����"\
		<<","<<"ͬƵ�����޸������"<<","<<"ͬƵ�����и������"<<","<<"ͬƵ���ظ����޸������"<<","<<"ͬƵ���ظ����и������"\
		<<","<<"��Ƶ�����޸������"<<","<<"��Ƶ�����и������"<<","<<"��Ƶ���ظ����޸������"<<","<<"��Ƶ���ظ����и������"\
		<<","<<"ͬƵ���Ƿ������޸������"<<","<<"ͬƵ���Ƿ������и������"<<","<<"��Ƶ���Ƿ������޸������"<<","<<"��Ƶ���Ƿ������и������"\
		<<","<<"ͬƵ����C\\I�����"<<","<<"��Ƶ����C\\I�����"<<","<<"ͬƵ���ظ���C\\I�����"<<","<<"��Ƶ���ظ���C\\I�����"\
		<<","<<"ͬƵ���ص�·C\\I�����"<<","<<"��Ƶ���ص�·C\\I�����"<<","<<"ͬƵ���ظ��ǵ�·C\\I�����"<<","<<"��Ƶ���ظ��ǵ�·C\\I�����"<<","<<"MR��ȨTCH�����"\
		<<","<<"ͬƵ����ƽ���ص�С����"<<","<<"ͬƵ����ƽ���ܸ���ǿ��"<<","<<"��Ƶ����ƽ���ص�С����"<<","<<"��Ƶ����ƽ���ܸ���ǿ��"\
		<<","<<"ͬƵ���ظ���ƽ���ص�С����"<<","<<"ͬƵ���ظ���ƽ���ܸ���ǿ��"<<","<<"��Ƶ���ظ���ƽ���ص�С����"<<","<<"��Ƶ���ظ���ƽ���ܸ���ǿ��"\
		<<","<<"ͬƵ���ص���"<<","<<"��Ƶ���ص���"<<","<<"ͬƵ���ظ��ǵ���"<<","<<"��Ƶ���ظ��ǵ���"\
		<<","<<"ͬƵƽ����������С����"<<","<<"ͬƵƽ����������С��ǿ��"<<","<<"��Ƶƽ����������С����"<<","<<"��Ƶƽ����������С��ǿ��"\
		<<","<<"��վ����ƫ��ϵ��"<<","<<"��վ�߶�ƫ��ϵ��"\
		<<","<<"���ǿ��ӱ���"<<","<<"����һ��NLOS����"<<","<<"���Ƕ��NLOS"\
		<<","<<"���ؿ��ӱ���"<<","<<"����һ��NLOS����"<<","<<"���ض��NLOS"\
		<<","<<"���ظ��ǿ��ӱ���"<<","<<"���ظ���һ��NLOS����"<<","<<"���ظ��Ƕ��NLOS"<<endl;
	for(int i=0;i<CellsAll.size();i++){
		Cell_CoverInfo celleva=CellsInfo_Region[i];

		fo<<celleva.cellid<<","\

		<<celleva.cover_power_avg<<","<<celleva.main_power_avg<<","<<celleva.main_boundpower_avg<<","<<celleva.main_roadpower_avg<<","\
		<<celleva.cover_area<<","<<celleva.main_area<<","<<celleva.maincover_area<<","<<celleva.cover_distance_max<<","<<celleva.main_distance_max<<","<<celleva.maincover_distance_max<<","<<celleva.cover_distance_avg<<","<<celleva.main_distance_avg<<","<<celleva.maincover_distance_avg<<","\
		<<celleva.main_conti<<","<<","<<celleva.main_overcover<<","<<celleva.reverse<<","<<celleva.main_cross<<","\
		<<celleva.main_cover_erea<<","<<celleva.maincover_cover_erea<<","<<celleva.main_cover_dist<<","<<celleva.maincover_cover_dist<<","
		<<celleva.Same_main_nointer_area<<","<<celleva.Same_main_inter_area<<","<<celleva.Same_maincover_nointer_area<<","<<celleva.Same_maincover_inter_area<<","\
		<<celleva.Diff_main_nointer_area<<","<<celleva.Diff_main_inter_area<<","<<celleva.Diff_maincover_nointer_area<<","<<celleva.Diff_maincover_inter_area<<","\
		<<celleva.Same_covernotmain_nointer_area<<","<<celleva.Same_covernotmain_inter_area<<","<<celleva.Diff_covernotmain_nointer_area<<","<<celleva.Diff_covernotmain_inter_area<<","\
		<<celleva.Same_main_CI<<","<<celleva.Diff_main_CI<<","<<celleva.Same_maincover_CI<<","<<celleva.Diff_maincover_CI<<","\
		<<celleva.Same_main_roadCI<<","<<celleva.Diff_main_roadCI<<","<<celleva.Same_maincover_CI<<","<<celleva.Diff_maincover_CI<<","<<celleva.cover_MR_CI<<","\
		<<celleva.Same_main_intercellnum<<","<<celleva.Same_main_interpower<<","<<celleva.Diff_main_intercellnum<<","\
		<<celleva.Diff_main_interpower<<","<<celleva.Same_maincover_intercellnum<<","<<celleva.Same_maincover_interpower<<","<<celleva.Diff_maincover_intercellnum<<","<<celleva.Diff_maincover_interpower<<","\
		<<celleva.Same_main_Noise<<","<<celleva.Diff_main_Noise<<","<<celleva.Same_maincover_Noise<<","<<celleva.Diff_maincover_Noise<<","\
		<<celleva.Same_covernotmain_intercellnum<<","<<celleva.Same_covernotmain_interpower<<","<<celleva.Diff_covernotmain_intercellnum<<","<<celleva.Diff_covernotmain_interpower<<","\
		<<celleva.Bias_Distance<<","<<celleva.Bias_Height<<","\
		<<celleva.cover_LOS<<","<<celleva.cover_NLOS_1<<","<<celleva.cover_NLOS<<","\
		<<celleva.main_LOS<<","<<celleva.main_NLOS_1<<","<<celleva.main_NLOS<<","\
		<<celleva.maincover_LOS<<","<<celleva.maincover_NLOS_1<<","<<celleva.maincover_NLOS<<endl;
	}

	*/
	fo.close();
	//cout<<"true\n";
	return true;
}

bool Evaluation::WriteGridsInfo(std::string temp_result_path_name){
	ofstream fo(temp_result_path_name.c_str(),'w');
	if(fo.is_open()==true){
		fo<<"GridID"<<","<<"��ò����"<<","\
			<<"�Ƿ�LOS"<<","<<"�Ƿ���"<<","<<"�Ƿ�����"<<","\
			<<"����С��"<<","<<"����С����ǿ"<<","\
			<<"BCCH����С����"<<","<<"BCCH C/I"<<","\
			<<"ͬƵ����С����"<<","<<"ͬƵC/I"<<","\
			<<"TCHͬƵ����С����"<<","<<"TCH��Ƶ����С����"<<","<<endl;
		for(int i=0;i<height;i++){
			for(int j=0;j<width;j++){
				fo<<GridsInfo_Region[i].gi[j].grid_id<<","<<GridsInfo_Region[i].gi[j].grid_type<<","\
					<<(GridsInfo_Region[i].gi[j].is_visual==1? "true": "false")<<","\
					<<(GridsInfo_Region[i].gi[j].is_reflection==1? "true": "false")<<","\
					<<(GridsInfo_Region[i].gi[j].is_diffraction==1? "true": "false")<<","\
					<<GridsInfo_Region[i].gi[j].maincellid<<","<<GridsInfo_Region[i].gi[j].maincellpower<<","\
					<<GridsInfo_Region[i].gi[j].Diff_inter_num<<","<<GridsInfo_Region[i].gi[j].Diff_inter_power<<","\
					<<GridsInfo_Region[i].gi[j].Same_inter_num<<","<<GridsInfo_Region[i].gi[j].Same_inter_power<<","\
					<<GridsInfo_Region[i].gi[j].TCH_Same_Num<<","<<GridsInfo_Region[i].gi[j].TCH_Neighbor_Num<<endl;
			}
		}
	}
	fo.close();
	return true;
}
//���ߺ���
Point Evaluation::IdToCoordinate(UINT64 grid_id){
	Point point;
	point.x=((int)grid_id-1)/BIG_WIDTH;
	point.y=((int)grid_id-1)%BIG_WIDTH;
	return point;
	//cout<<point.x<<" "<<point.y<<endl;
}
Point Evaluation::IdToCoordinate_Region(UINT64 grid_id,UINT64 minregionid){
	Point point;
	point.x=((int)grid_id-1)/BIG_WIDTH-((int)minregionid-1)/BIG_WIDTH;
	point.y=((int)grid_id-1)%BIG_WIDTH-((int)minregionid-1)%BIG_WIDTH;
	return point;
	//cout<<point.x<<" "<<point.y<<endl;
}
float Evaluation::Distance(UINT64 gridid_one,UINT64 gridid_two){
	Point point_one=IdToCoordinate(gridid_one);
	Point point_two=IdToCoordinate(gridid_two);
	return GRID_SIZE*sqrt((float)(point_one.x-point_two.x)*(point_one.x-point_two.x)+(point_one.y-point_two.y)*(point_one.y-point_two.y));
}


bool Evaluation::ListOfFreqsSameTCH(string cellid1,string cellid2,vector<int> & tchlist){
	tchlist.clear();
	if(CellsAll.size()==0){
		return false;
	}
	for(int i=0;i<CellsAll.size();i++){
		if(CellsAll[i].cell_id==cellid1){
			for(int j=0;j<CellsAll.size();j++){
				if(CellsAll[j].cell_id==cellid2){
					for(int k=0;k<CellsAll[i].tchno.size();k++){
						for (int l=0;l<CellsAll[j].tchno.size();l++){
							if(CellsAll[i].tchno[k]==CellsAll[j].tchno[l]){
								tchlist.push_back(CellsAll[i].tchno[k]);
							}
						}
					}
					return true;
				}
			}
		}
	}
	return false;
}
bool Evaluation::ListOfFreqsNeighbourTCH(string cellid1,string cellid2,vector<int> & tchlist){
	tchlist.clear();
	if(CellsAll.size()==0){
		return false;
	}
	for(int i=0;i<CellsAll.size();i++){
		if(CellsAll[i].cell_id==cellid1){
			for(int j=0;j<CellsAll.size();j++){
				if(CellsAll[j].cell_id==cellid2){
					for(int k=0;k<CellsAll[i].tchno.size();k++){
						for (int l=0;l<CellsAll[j].tchno.size();l++){
							if(CellsAll[j].tchno[l]==(CellsAll[i].tchno[k]+1)||CellsAll[j].tchno[l]==(CellsAll[i].tchno[k]-1)){
								tchlist.push_back(CellsAll[i].tchno[k]);
							}
						}
					}
					return true;
				}
			}
		}
	}
	return false;
}
bool Evaluation::IsFreqSameBCCH(string cellid1,string cellid2){
	if(CellsAll.size()==0){
		return false;
	}
	for(int i=0;i<CellsAll.size();i++){
		if(CellsAll[i].cell_id==cellid1){
			for(int j=0;j<CellsAll.size();j++){
				if(CellsAll[j].cell_id==cellid2){
					if(CellsAll[i].bcchno==CellsAll[j].bcchno){
						return true;
					}
					else{
						return false;
					}
				}
			}			
		}
	}

	return false;
}
float Evaluation::PowerAdd(float p1,float p2){
	return 10*log10(pow(10,p1/10)+pow(10,p2/10));
}
//��֤����
bool Evaluation::Verify_MainPower(){
	//��֤����С���ĳ�ǿ
}
bool Evaluation::Verify_Inter(){
	//��֤ͬƵ��BCCH����
}
bool Evaluation::Verify_TCHInter(){
	;//��֤TCH����
}
