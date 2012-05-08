//�����ݿ���ȡ����֤
bool Evaluation::GetMainPower_FromDB(){
//*******************************************************************************
//		���ж�ȡpostgres���ݿ��е�maincell_tablename���е�����С���źͶ�Ӧ�ĳ�ǿ
//		��ǿ�浽��Ĺ�������vector<vector<float> > mainpower���ǰ���height��width)�ֲ�
//		����С���浽��ʱ����tempmap�У��ṹΪ<cell_id,VEC>��map
//*******************************************************************************
	mainpower.clear();
	mainpower.resize(height);
	for(int i=0;i<height;i++){
		mainpower[i].clear();
		mainpower[i].resize(width,POWER);//POWERΪ����-94
	}
	typedef map<string,VEC>::iterator MI;
	for(MI p=tempmap.begin();p!=tempmap.end();p++){
		p->second.gridid_vec.clear();
	}

//********************************************************************************
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

//********************************************************************************
	vector<vector<Main_GridPower> > tempmain_gridpower;
	tempmain_gridpower.clear();
	tempmain_gridpower.resize(height);

#pragma omp parallel for
	for(int i=0;i<height;i++){		
		char mingridchar[64];
		char maxgridchar[64];
		UINT64 mingridid=regionminid+i*BIG_WIDTH;
		UINT64 maxgridid=mingridid+width-1;
		sprintf_s(mingridchar,"%d",mingridid);
		sprintf_s(maxgridchar,"%d",maxgridid);
		int threadi=omp_get_thread_num();
		//***********************************************
		tempmain_gridpower[i].clear();
		MainCell_Grid_Power tempmaingridpower;
		tempmaingridpower.SetConn(conn_array[threadi]);
		tempmaingridpower.SetDefault();
		tempmaingridpower.Setkeyfield_gridid();
		tempmaingridpower.SetTableName(maincell_tablename.c_str());
		tempmaingridpower.SelectTwoCondition(mingridchar,maxgridchar,tempmain_gridpower[i]);

	}

#pragma omp parallel for
	for(conn_i=0;conn_i<pronum;conn_i++){
		PQfinish(conn_array[conn_i]);
	}
	conn_array.clear();

	//***************************************************************************************[[[[

	Point minpoint=IdToCoordinate(regionminid);

	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			Point temppoint=IdToCoordinate(tempmain_gridpower[i][j].grid_id);
			if(temppoint.x-minpoint.x>=0&&temppoint.x-minpoint.x<width&&temppoint.y-minpoint.y>=0&&temppoint.y-minpoint.y<height){
				mainpower[temppoint.y-minpoint.y][temppoint.x-minpoint.x]=tempmain_gridpower[i][j].receivepower;
				tempmap[tempmain_gridpower[i][j].cellid].gridid_vec.push_back(tempmain_gridpower[i][j].grid_id);
				
			}
		}		
	}

	tempmain_gridpower.clear();
	return true;
}
bool Evaluation::GetInter_FromDB(){
	Grid_Inter tempinter;
	tempinter.Diff_internum=0;
	tempinter.Diff_interpower=POWER;
	tempinter.grid_id=0;
	tempinter.Same_internum=0;
	tempinter.Same_interpower=POWER;
	interpower.clear();
	interpower.resize(height);
	for(int i=0;i<height;i++){
		interpower[i].clear();
		interpower[i].resize(width,tempinter);
	}
	//****************************************************
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
		Inter_Info_Class tempinterclass;
		vector<Grid_Inter> tempgridinter;
		tempgridinter.clear();
		tempinterclass.SetConn(conn_array[threadi]);
		tempinterclass.SetDefault();
		tempinterclass.SetTableName(inter_tablename.c_str());
		tempinterclass.SelectTwoCondition(mingridchar,maxgridchar,tempgridinter);
		for(int j=0;j<tempgridinter.size();j++){
			Point temppoint=IdToCoordinate(tempgridinter[j].grid_id);
			if(temppoint.x>=minpoint.x&&temppoint.x<minpoint.x+width&&temppoint.y>=minpoint.y&&temppoint.y<minpoint.y+height){
				interpower[temppoint.y-minpoint.y][temppoint.x-minpoint.x].Same_internum=tempgridinter[j].Same_internum;
				interpower[temppoint.y-minpoint.y][temppoint.x-minpoint.x].Same_interpower=tempgridinter[j].Same_interpower;
				interpower[temppoint.y-minpoint.y][temppoint.x-minpoint.x].Diff_internum=tempgridinter[j].Diff_internum;
				interpower[temppoint.y-minpoint.y][temppoint.x-minpoint.x].Diff_interpower=tempgridinter[j].Diff_interpower;
			}
		}
		tempgridinter.clear();
	}
#pragma omp parallel for
	for(conn_i=0;conn_i<pronum;conn_i++){
		PQfinish(conn_array[conn_i]);
	}
	conn_array.clear();
	return true;

}



bool Evaluation::GetTCHInter_FromFile(){
	//******************************************************************
	//������Ϣ��ʼ��
	int height=maxpoint.x-minpoint.x+1;
	int width=maxpoint.y-minpoint.y+1;
	if(height<2&&width<2)
		return false;
	Grid_Info tempgrid;
	tempgrid.cover_power=-300;
	tempgrid.grid_id=0;
	tempgrid.main_power=-300;
	tempgrid.grid_type=0;
	tempgrid.Same_internum=0;
	tempgrid.Same_interpower=0;
	tempgrid.Diff_internum=0;
	tempgrid.Diff_interpower=0;
	//tempgrid.mrcount=0;
	//tempgrid.mrcount_drop=0;
	tempgrid.mrtraffic=0;
	tempgrid.TCH_Neighbor_Num=0;
	tempgrid.TCH_Neighbor_Vec.clear();
	tempgrid.TCH_Neighbor_Vec.resize(tempgrid.TCH_Neighbor_Num,-300);
	tempgrid.maincellid="";
	tempgrid.TCH_Same_Num=0;
	tempgrid.TCH_Same_Vec.clear();
	tempgrid.TCH_Same_Vec.resize(tempgrid.TCH_Same_Num,-300);
	tempgrid.geoinfo.is_diffraction=0;
	tempgrid.geoinfo.grid_id=0;
	tempgrid.geoinfo.is_reflection=0;
	tempgrid.geoinfo.is_visual=0;
//************************************************************************************************
	gridinfo.clear();
	gridinfo.resize(height);
	for(int i=0;i<height;i++){
		gridinfo[i].clear();
		gridinfo[i].resize(width,tempgrid);
	}
//********************************************************************
	//��ȡ���񸲸���Ϣ������-94������
	string cellcover_name=cellcover_path_name+(string)"\\field_"+networksort+(string)"\\"+cellinfo.cell_id+(string)".dat";
	ifstream fi;
	fi.open(cellcover_name.c_str(),ios_base::in|ios_base::binary);
	if(fi.is_open()==true){
		Cover_GridPower tempgridpower;
		while (!fi.eof()){
			fi.read((char*)&tempgridpower,sizeof(tempgridpower));
			if(tempgridpower.grid_id==0)
				break;
			Point temppoint=IdToCoordinate(tempgridpower.grid_id);
			if(temppoint.x>=minpoint.x&&temppoint.y>=minpoint.y&&temppoint.x<=maxpoint.x&&temppoint.y<=maxpoint.y){
				gridinfo[temppoint.x-minpoint.x][temppoint.y-minpoint.y].grid_id=tempgridpower.grid_id;
				gridinfo[temppoint.x-minpoint.x][temppoint.y-minpoint.y].cover_power=tempgridpower.receivepower;
			}
		}
	}
	fi.close();
	//*****************************************************************************************************************************************************
	//��ȡ������Ϣ
	string geo_name=geo_path_name+(string)"\\field_"+networksort+(string)"\\"+cellinfo.cell_id+(string)".dat";
	ifstream fi_geo;
	fi_geo.open(geo_name.c_str(),ios_base::in|ios_base::binary);
	if(fi_geo.is_open()==true){
		GeographyInfo tempgridgeo;
		while (!fi.eof()){
			fi_geo.read((char*)&tempgridgeo,sizeof(tempgridgeo));
			if(tempgridgeo.grid_id==0)
				break;
			Point temppoint=IdToCoordinate(tempgridgeo.grid_id);
			if(temppoint.x>=minpoint.x&&temppoint.y>=minpoint.y&&temppoint.x<=maxpoint.x&&temppoint.y<=maxpoint.y){
				gridinfo[temppoint.x-minpoint.x][temppoint.y-minpoint.y].geoinfo.grid_id=tempgridgeo.grid_id;
				gridinfo[temppoint.x-minpoint.x][temppoint.y-minpoint.y].geoinfo.is_diffraction=tempgridgeo.is_diffraction;
				gridinfo[temppoint.x-minpoint.x][temppoint.y-minpoint.y].geoinfo.is_reflection=tempgridgeo.is_reflection;
				gridinfo[temppoint.x-minpoint.x][temppoint.y-minpoint.y].geoinfo.is_visual=tempgridgeo.is_visual;
			}
		}
	}
	fi_geo.close();

	//*****************************************************************************************************************************************************
	//��ȡͬƵTCH������Ϣ
	string tchinter_same_name=tchinter_same_path_name+(string)"\\"+cellinfo.cell_id+(string)".dat";
	ifstream fi_tchsame;
	fi_tchsame.open(tchinter_same_name.c_str(),ios_base::in|ios_base::binary);
	TCH_GridVec temptchvec;
	UINT64 tempgridid;
		int tchnum=0;
	if(fi_tchsame.is_open()==true){		
		while(!fi_tchsame.eof()){
			fi_tchsame.read((char*)&tempgridid,sizeof(tempgridid));
			fi_tchsame.read((char*)&tchnum,sizeof(tchnum));
			if(tempgridid==0)
				break;
			else{
				temptchvec.grid_id=tempgridid;
				temptchvec.TCHnum=tchnum;
				temptchvec.TCHvec.clear();
				for(int i=0;i<tchnum;i++){
					float vecnum=-300;
					fi_tchsame.read((char*)&vecnum,sizeof(vecnum));
					temptchvec.TCHvec.push_back(vecnum);
				}
				Point temppoint=IdToCoordinate(temptchvec.grid_id);
				if(temppoint.x>=minpoint.x&&temppoint.y>=minpoint.y&&temppoint.x<=maxpoint.x&&temppoint.y<=maxpoint.y){
					gridinfo[temppoint.x-minpoint.x][temppoint.y-minpoint.y].TCH_Same_Num=temptchvec.TCHnum;
					gridinfo[temppoint.x-minpoint.x][temppoint.y-minpoint.y].TCH_Same_Vec=temptchvec.TCHvec;
				}
			}
		}
	}
	temptchvec.TCHvec.clear();
	fi_tchsame.close();
	//***************************************************************************
	//��ȡ��ƵTCH������Ϣ
	string tchinter_neighbor_name=tchinter_neighbor_path_name+(string)"\\"+cellinfo.cell_id+(string)".dat";
	ifstream fi_tchneighbor;
	fi_tchneighbor.open(tchinter_neighbor_name.c_str(),ios_base::in|ios_base::binary);
	if(fi_tchneighbor.is_open()==true){
		while(!fi_tchneighbor.eof()){
			fi_tchneighbor.read((char*)&tempgridid,sizeof(tempgridid));
			fi_tchneighbor.read((char*)&tchnum,sizeof(tchnum));
			if(tempgridid==0)
				break;
			else{
				temptchvec.grid_id=tempgridid;
				temptchvec.TCHnum=tchnum;
				temptchvec.TCHvec.clear();
				for(int i=0;i<tchnum;i++){
					float vecnum=-300;
					fi_tchneighbor.read((char*)&vecnum,sizeof(vecnum));
					temptchvec.TCHvec.push_back(vecnum);
				}
				Point temppoint=IdToCoordinate(temptchvec.grid_id);
				if(temppoint.x>=minpoint.x&&temppoint.y>=minpoint.y&&temppoint.x<=maxpoint.x&&temppoint.y<=maxpoint.y){
					gridinfo[temppoint.x-minpoint.x][temppoint.y-minpoint.y].TCH_Neighbor_Num=temptchvec.TCHnum;
					gridinfo[temppoint.x-minpoint.x][temppoint.y-minpoint.y].TCH_Neighbor_Vec=temptchvec.TCHvec;
				}
			}
		}
	}
	fi_tchneighbor.close();
	temptchvec.TCHvec.clear();
//*******************************************************************************
	//MR����
	string mr_name=mrinfo_path_name+(string)"\\"+cellinfo.cell_id+(string)".dat";
	ifstream fi_mr;
	fi_mr.open(mr_name.c_str(),ios_base::in|ios_base::binary);
	if(fi_mr.is_open()==true){
		while(!fi_mr.eof()){
			UINT64 mrgrid=0;
			double mrtraffic=0;
			fi_mr.read((char*)&mrgrid,sizeof(mrgrid));
			fi_mr.read((char*)&mrtraffic,sizeof(mrtraffic));
			if(mrgrid==0)
				break;
			else{
				Point temppoint=IdToCoordinate(mrgrid);
				if(temppoint.x>=minpoint.x&&temppoint.y>=minpoint.y&&temppoint.x<=maxpoint.x&&temppoint.y<=maxpoint.y){
					gridinfo[temppoint.x-minpoint.x][temppoint.y-minpoint.y].mrtraffic=mrtraffic;
				}
			}
		}
	}
	fi_mr.close();
//***************************************************************************************************************************************************************
	//��ȡ��������س�ǿֵ��������Ϣ�͸�����Ϣ MR��Ϣ
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
#pragma omp parallel for
	for(int i=0;i<height;i++){		
		char mingridchar[64];
		char maxgridchar[64];
		UINT64 mingridid=mingrid_id+i*BIG_WIDTH;
		UINT64 maxgridid=mingridid+width-1;
		sprintf_s(mingridchar,"%d",mingridid);
		sprintf_s(maxgridchar,"%d",maxgridid);
		int threadi=omp_get_thread_num();
		//***********************************************
		vector<Main_GridPower> tempmain_gridpower;
		tempmain_gridpower.clear();
		MainCell_Grid_Power tempmaingridpower;
		tempmaingridpower.SetConn(conn_array[threadi]);
		tempmaingridpower.SetDefault();
		tempmaingridpower.Setkeyfield_gridid();
		tempmaingridpower.SetTableName(maincell_tablename.c_str());
		tempmaingridpower.SelectTwoCondition(mingridchar,maxgridchar,tempmain_gridpower);

		//************************************************
		vector<Grid_Type> tempgrid_type;
		tempgrid_type.clear();		
		GridType tempgridtype;
		tempgridtype.SetConn(conn_array[threadi]);
		tempgridtype.SetDefault();
		tempgridtype.SetTableName(grid_tablename.c_str());
		tempgridtype.SelectTwoCondition(mingridchar,maxgridchar,tempgrid_type);

		//*****************************************************
		Inter_Info_Class tempinterclass;
		vector<Grid_Inter> tempgridinter;
		tempgridinter.clear();
		tempinterclass.SetConn(conn_array[threadi]);
		tempinterclass.SetDefault();
		tempinterclass.SetTableName(inter_tablename.c_str());
		tempinterclass.SelectTwoCondition(mingridchar,maxgridchar,tempgridinter);

		/*
		//******************************************************
		MR_Class tempmrclass;
		vector<Grid_MR>tempgridmr;
		tempgridmr.clear();
		tempmrclass.SetConn(conn_array[threadi]);
		tempmrclass.SetDefault_MR();
		tempmrclass.SetTableName(MR_tablename);
		tempmrclass.SelectTwoCondition(mingridchar,maxgridchar,tempgridmr);
		//*******************************************************
		vector<Grid_MR>tempgridmr_drop;
		tempgridmr_drop.clear();
		tempmrclass.SetDefault_MR_Drop();
		tempmrclass.SetTableName(MR_DROP_tablename);
		tempmrclass.SelectTwoCondition(mingridchar,maxgridchar,tempgridmr_drop);
		*/
		//********************************************************************
		Point temppoint;
		for(int j=0;j<tempmain_gridpower.size();j++){
			temppoint=IdToCoordinate(tempmain_gridpower[j].grid_id);
			if(temppoint.x>=minpoint.x&&temppoint.y>=minpoint.y&&temppoint.x<=maxpoint.x&&temppoint.y<=maxpoint.y){
				gridinfo[temppoint.x-minpoint.x][temppoint.y-minpoint.y].main_power=tempmain_gridpower[j].receivepower;
				gridinfo[temppoint.x-minpoint.x][temppoint.y-minpoint.y].maincellid=tempmain_gridpower[j].cellid;
			}
		}
		//********************************************************************
		for(int j=0;j<tempgrid_type.size();j++){
			temppoint=IdToCoordinate(tempgrid_type[j].grid_id);
			if(temppoint.x>=minpoint.x&&temppoint.y>=minpoint.y&&temppoint.x<=maxpoint.x&&temppoint.y<=maxpoint.y){
				gridinfo[temppoint.x-minpoint.x][temppoint.y-minpoint.y].grid_type=tempgrid_type[j].grid_type;
			}
		}
		//********************************************************************
		for(int j=0;j<tempgridinter.size();j++){
			temppoint=IdToCoordinate(tempgridinter[j].grid_id);
			if(temppoint.x>=minpoint.x&&temppoint.y>=minpoint.y&&temppoint.x<=maxpoint.x&&temppoint.y<=maxpoint.y){
				gridinfo[temppoint.x-minpoint.x][temppoint.y-minpoint.y].Same_internum=tempgridinter[j].Same_internum;
				gridinfo[temppoint.x-minpoint.x][temppoint.y-minpoint.y].Same_interpower=tempgridinter[j].Same_interpower;
				gridinfo[temppoint.x-minpoint.x][temppoint.y-minpoint.y].Diff_internum=tempgridinter[j].Diff_internum;
				gridinfo[temppoint.x-minpoint.x][temppoint.y-minpoint.y].Diff_interpower=tempgridinter[j].Diff_interpower;
			}
		}
			/*
			temppoint=IdToCoordinate(tempgridmr[i].grid_id);
			if(temppoint.x>=minpoint.x&&temppoint.y>=minpoint.y&&temppoint.x<=maxpoint.x&&temppoint.y<=maxpoint.y){
				gridinfo[temppoint.x-minpoint.x][temppoint.y-minpoint.y].mrcount=tempgridmr[j].mrcount;
			}
			temppoint=IdToCoordinate(tempgridmr_drop[i].grid_id);
			if(temppoint.x>=minpoint.x&&temppoint.y>=minpoint.y&&temppoint.x<=maxpoint.x&&temppoint.y<=maxpoint.y){
				gridinfo[temppoint.x-minpoint.x][temppoint.y-minpoint.y].mrcount_drop=tempgridmr_drop[j].mrcount;
			}
			*/
		tempmain_gridpower.clear();
		tempgrid_type.clear();
		tempgridinter.clear();
	}
	#pragma omp parallel for
	for(conn_i=0;conn_i<pronum;conn_i++){
		PQfinish(conn_array[conn_i]);
	}
	conn_array.clear();
	//*************************************************************************
	return true;
	}


bool Evaluation::Grid_Compute(){
	int height=maxpoint.x-minpoint.x+1;
	int width=maxpoint.y-minpoint.y+1;
	if(height<2&&width<2)
		return false;
	int cover_bounderea=0;
	//float main_MR_drop_sum=0;
//	float main_MR_sum=0;
	Point cellpoint=IdToCoordinate(cellinfo.grid_id);
//	int reversesum=0;
	double cover_mr_sum=0;

//	float maincover_MR_drop_sum=0;
//	float maincover_MR_sum=0;
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			//********************************************************************
			//��������� ƽ����ǿ�����
			if(gridinfo[i][j].cover_power>=POWER_CANON){
				cover_powerever=(cover_powerever*cover_erea+gridinfo[i][j].cover_power)/(float)(cover_erea+1);
				cover_erea++;
				if(gridinfo[i][j].geoinfo.is_visual==1){
					cover_LOS++;
				}
				else if(gridinfo[i][j].geoinfo.is_diffraction==1||gridinfo[i][j].geoinfo.is_reflection==1){
					cover_NLOS_1++;
				}
				else{
					cover_NLOS++;
				}
			}
			//*****************************************************************
			//���ر߽��������������ĵ�һ�л������һ�л��ߵ�һ�л������һ�У������������������������������һ�������س�ǿֵΪ-300
			if(gridinfo[i][j].maincellid==cellinfo.cell_id&&(i==0||i==height-1||j==0||j==width-1)){//���ر߽�
				main_boundpowerever=(main_boundpowerever*main_bounderea+gridinfo[i][j].main_power)/(float)(main_bounderea+1);//���� �߽�ƽ����ǿֵ
				Point point_one=IdToCoordinate(cellinfo.grid_id);
				Point point_two=IdToCoordinate(gridinfo[i][j].grid_id);
				float tempdistance=GRID_SIZE*sqrt((float)(point_one.x-point_two.x)*(point_one.x-point_two.x)+(point_one.y-point_two.y)*(point_one.y-point_two.y));
				main_avgdistance=(main_avgdistance*main_bounderea+tempdistance)/(float)(main_bounderea+1);//ƽ�����ر߽����
				main_maxdistance=max(tempdistance,main_maxdistance);//��Զ���ؾ���
				main_bounderea++;//���ر߽����
				if(gridinfo[i][j].main_power>=-94){
					//*************************************************************
					//���ظ��ǵı߽�
					maincover_boundpowerever=(maincover_boundpowerever*maincover_bounderea+gridinfo[i][j].main_power)/(float)(maincover_bounderea+1);//���ظ���ƽ����ǿֵ
					maincover_avgdistance=(maincover_avgdistance*maincover_bounderea+tempdistance)/(float)(maincover_bounderea+1);//ƽ�����ظ��Ǳ߽����
					maincover_maxdistance=max(tempdistance,maincover_maxdistance);//��Զ���ظ��Ǿ���
					maincover_bounderea++;//���ظ��Ǳ߽����
				}
			}
			else if(gridinfo[i][j].maincellid==cellinfo.cell_id&&(gridinfo[i][j-1].maincellid!=cellinfo.cell_id||gridinfo[i][j+1].maincellid!=cellinfo.cell_id||gridinfo[i-1][j].maincellid!=cellinfo.cell_id||gridinfo[i+1][j].maincellid!=cellinfo.cell_id)){//���ر߽�
				main_boundpowerever=(main_boundpowerever*main_bounderea+gridinfo[i][j].main_power)/(float)(main_bounderea+1);//����ƽ����ǿֵ
				Point point_one=IdToCoordinate(cellinfo.grid_id);
				Point point_two=IdToCoordinate(gridinfo[i][j].grid_id);
				float tempdistance=GRID_SIZE*sqrt((float)(point_one.x-point_two.x)*(point_one.x-point_two.x)+(point_one.y-point_two.y)*(point_one.y-point_two.y));
				main_avgdistance=(main_avgdistance*main_bounderea+tempdistance)/(float)(main_bounderea+1);//ƽ�����ر߽����
				main_maxdistance=max(tempdistance,main_maxdistance);//��Զ���ؾ���
				main_bounderea++;//���ر߽����
				if(gridinfo[i][j].main_power>=-94){//���ظ��ǵı߽�
					maincover_boundpowerever=(maincover_boundpowerever*maincover_bounderea+gridinfo[i][j].main_power)/(float)(maincover_bounderea+1);//���ظ���ƽ����ǿֵ
					maincover_avgdistance=(maincover_avgdistance*maincover_bounderea+tempdistance)/(float)(maincover_bounderea+1);//ƽ�����ظ��Ǳ߽����
					maincover_maxdistance=max(tempdistance,maincover_maxdistance);//��Զ���ظ��Ǿ���
					maincover_bounderea++;//���ظ��Ǳ߽����
				}
			}
			//***********************************************************************************
			//���Ǳ߽�
			if(gridinfo[i][j].cover_power>-94&&(i==0||i==height-1||j==0||j==width-1)){//���Ǳ߽�
				Point point_one=IdToCoordinate(cellinfo.grid_id);
				Point point_two=IdToCoordinate(gridinfo[i][j].grid_id);
				float tempdistance=GRID_SIZE*sqrt((float)(point_one.x-point_two.x)*(point_one.x-point_two.x)+(point_one.y-point_two.y)*(point_one.y-point_two.y));
				cover_avgdistance=(cover_avgdistance*cover_bounderea+tempdistance)/(float)(cover_bounderea+1);//ƽ�����Ǳ߽����
				cover_maxdistance=max(tempdistance,cover_maxdistance);//��Զ���Ǿ���
				cover_bounderea++;//���Ǳ߽����
			}
			else if(gridinfo[i][j].cover_power>=-94&&(gridinfo[i][j-1].cover_power<-94||gridinfo[i][j+1].cover_power<-94||gridinfo[i-1][j].cover_power<-94||gridinfo[i+1][j].cover_power<-94)){//���Ǳ߽�
			Point point_one=IdToCoordinate(cellinfo.grid_id);
				Point point_two=IdToCoordinate(gridinfo[i][j].grid_id);
				float tempdistance=GRID_SIZE*sqrt((float)(point_one.x-point_two.x)*(point_one.x-point_two.x)+(point_one.y-point_two.y)*(point_one.y-point_two.y));
				cover_avgdistance=(cover_avgdistance*cover_bounderea+tempdistance)/(float)(cover_bounderea+1);//ƽ�����Ǳ߽����
				cover_maxdistance=max(tempdistance,cover_maxdistance);//��Զ���Ǿ���
				cover_bounderea++;//���Ǳ߽����
			}
			//************************************************************************************
			//��·��Ϣ
			if(gridinfo[i][j].maincellid==cellinfo.cell_id&&gridinfo[i][j].grid_type==1){//����
				main_roadpowerever=(main_roadpowerever*main_roaderea+gridinfo[i][j].main_power)/(float)(main_roaderea+1);				
				if(gridinfo[i][j].Same_interpower>=CANONNUM){
					Same_main_roadCI=(Same_main_roadCI*main_roaderea+1)/(float)(main_roaderea+1);
				}
				if(gridinfo[i][j].Diff_interpower>=CANONNUM){
					Diff_main_roadCI=(Diff_main_roadCI*main_roaderea+1)/(float)(main_roaderea+1);
				}
				main_roaderea++;
				if(gridinfo[i][j].main_power>=-94){//���ظ���
					maincover_roadpowerever=(maincover_roadpowerever*maincover_roaderea+gridinfo[i][j].main_power)/(float)(maincover_roaderea+1);
					
					if(gridinfo[i][j].Same_interpower>=CANONNUM){
						Same_maincover_roadCI=(Same_maincover_roadCI*maincover_roaderea+1)/(float)(maincover_roaderea+1);
					}
					if(gridinfo[i][j].Diff_interpower>=CANONNUM){
						Diff_maincover_roadCI=(Diff_maincover_roadCI*maincover_roaderea+1)/(float)(maincover_roaderea+1);}
					maincover_roaderea++;
				}
			}
			//************************************************************************
			//���������ƽ����ǿֵ ͬƵ/��Ƶ���޸������   MR MR���� C\I
			if(gridinfo[i][j].maincellid==cellinfo.cell_id){				
				main_powerever=(main_powerever*main_erea+gridinfo[i][j].main_power)/(float)(main_erea+1);
				if(gridinfo[i][j].geoinfo.is_visual==1){
					main_LOS++;
				}
				else if(gridinfo[i][j].geoinfo.is_diffraction==1||gridinfo[i][j].geoinfo.is_reflection==1){
					main_NLOS_1++;
				}
				else{
					main_NLOS++;
				}
			//	main_MR_sum+=gridinfo[i][j].mrcount;
			//	main_MR_drop_sum+=gridinfo[i][j].mrcount_drop;
				Same_main_Noise=(Same_main_Noise*main_erea+gridinfo[i][j].main_power-gridinfo[i][j].Same_interpower)/(float)(main_erea+1);
				Diff_main_Noise=(Diff_main_Noise*main_erea+gridinfo[i][j].main_power-gridinfo[i][j].Diff_interpower)/(float)(main_erea+1);
				Same_main_intercellnum=(Same_main_intercellnum*main_erea+gridinfo[i][j].Same_internum)/(float)(main_erea+1);
				Diff_main_intercellnum=(Diff_main_intercellnum*main_erea+gridinfo[i][j].Diff_internum)/(float)(main_erea+1);
				main_erea++;
				if(gridinfo[i][j].Same_interpower>=CANONNUM){
					Same_main_nointererea++;
			//		Same_main_MRCI+=gridinfo[i][j].mrcount;//����MRC\I ͬƵ
			//		Same_main_MRdropCI+=gridinfo[i][j].mrcount_drop;//����MR����C\I ͬƵ
				}
				else{
					
					Same_main_interpower=(Same_main_interpower*Same_main_intererea+gridinfo[i][j].Same_interpower)/(float)(Same_main_intererea+1);
					Same_main_intererea++;
				}
				if(gridinfo[i][j].Diff_interpower>=CANONNUM){
					Diff_main_nointererea++;
			//		Diff_main_MRCI+=gridinfo[i][j].mrcount;//����MRC\I ͬƵ
			//		Diff_main_MRdropCI+=gridinfo[i][j].mrcount_drop;//����MR����C\I ͬƵ
				}
				else{
					Diff_main_interpower=(Diff_main_interpower*Diff_main_intererea+gridinfo[i][j].Diff_interpower)/(float)(Diff_main_intererea+1);
					Diff_main_intererea++;
				}
				//***********************************************
				//���ظ���  ͬƵ/��Ƶ���޸������   MR MR���� C\I
				if(gridinfo[i][j].main_power>-94){
					if(gridinfo[i][j].geoinfo.is_visual==1){
						maincover_LOS++;
					}
					else if(gridinfo[i][j].geoinfo.is_diffraction==1||gridinfo[i][j].geoinfo.is_reflection==1){
						maincover_NLOS_1++;
					}
					else{
						maincover_NLOS++;
					}
					double pointx=cos(cellinfo.direction*PI/180);
					double pointy=sin(cellinfo.direction*PI/180);
					Point tempgridpoint=IdToCoordinate(gridinfo[i][j].grid_id);
					Point pointfirst;
					pointfirst.x=tempgridpoint.x-cellpoint.x;
					pointfirst.y=tempgridpoint.y-cellpoint.y;
					double yuxian=(pointfirst.x*pointx+pointfirst.y*pointy)/(sqrt((float)pointfirst.x*pointfirst.x+pointfirst.y*pointfirst.y)*sqrt((float)pointx*pointx+pointy*pointy));
					double jiajiao=acos(yuxian)*180/PI;//��Ϊ���ĳ�ʼֵΪ���� ���뽫���Ȼ�Ϊ����
					if(jiajiao>(DIRECTION/(float)2)){
						reverseever=(reverseever*maincover_erea+1)/(float)(maincover_erea+1);
					}
					maincover_powerever=(maincover_powerever*maincover_erea+gridinfo[i][j].main_power)/(float)(maincover_erea+1);
					maincover_erea++;
				//	maincover_MR_sum+=gridinfo[i][j].mrcount;
				//	maincover_MR_drop_sum+=gridinfo[i][j].mrcount_drop;
					Same_maincover_Noise=(Same_maincover_Noise*maincover_erea+gridinfo[i][j].main_power-gridinfo[i][j].Same_interpower)/(float)(maincover_erea+1);
					Diff_maincover_Noise=(Diff_maincover_Noise*maincover_erea+gridinfo[i][j].main_power-gridinfo[i][j].Diff_interpower)/(float)(maincover_erea+1);
					Same_maincover_intercellnum=(Same_maincover_intercellnum*maincover_erea+gridinfo[i][j].Same_internum)/(float)(maincover_erea+1);
					Diff_maincover_intercellnum=(Diff_maincover_intercellnum*maincover_erea+gridinfo[i][j].Diff_internum)/(float)(maincover_erea+1);
					if(gridinfo[i][j].Same_interpower>=CANONNUM){
						Same_maincover_nointererea++;
				//		Same_maincover_MRCI+=gridinfo[i][j].mrcount;//����MRC\I ͬƵ
				//		Same_maincover_MRdropCI+=gridinfo[i][j].mrcount_drop;//����MR����C\I ͬƵ
					}
					else{
						Same_maincover_interpower=(Same_maincover_interpower*Same_maincover_intererea+gridinfo[i][j].Same_interpower)/(float)(Same_maincover_intererea+1);
						Same_maincover_intererea++;
					}
					if(gridinfo[i][j].Diff_interpower>=CANONNUM){
						Diff_maincover_nointererea++;
				//		Diff_maincover_MRCI+=gridinfo[i][j].mrcount;//����MRC\I ͬƵ
				//		Diff_maincover_MRdropCI+=gridinfo[i][j].mrcount_drop;//����MR����C\I ͬƵ
					}
					else{
						Diff_maincover_interpower=(Diff_maincover_interpower*Diff_maincover_intererea+gridinfo[i][j].Diff_interpower)/(float)(Diff_maincover_intererea+1);
						Diff_maincover_intererea++;
					}
				}

			}
			//****************************************************************
			//���Ƿ�������Ϣ
			if(gridinfo[i][j].maincellid!=cellinfo.cell_id&&gridinfo[i][j].cover_power>-94){//���Ƿ�����
				if(gridinfo[i][j].Same_interpower>=CANONNUM){
					Same_covernotmain_nointererea++;
				}
				else{
					Same_covernotmain_interpower=(Same_covernotmain_interpower*Same_covernotmain_intererea+gridinfo[i][j].cover_power-gridinfo[i][j].main_power)/(float)(Same_covernotmain_intererea+1);
					Same_covernotmain_intercellnum=(Same_covernotmain_intercellnum*Same_covernotmain_intererea+gridinfo[i][j].Same_internum)/(float)(Same_covernotmain_intererea+1);
					Same_covernotmain_intererea++;
				}
				if(gridinfo[i][j].Diff_interpower>=CANONNUM){
					Diff_covernotmain_nointererea++;
				}
				else{
					Diff_covernotmain_interpower=(Diff_covernotmain_interpower*Diff_covernotmain_intererea+gridinfo[i][j].cover_power-gridinfo[i][j].main_power)/(float)(Diff_covernotmain_intererea+1);
					Diff_covernotmain_intercellnum=(Diff_covernotmain_intercellnum*Diff_covernotmain_intererea+gridinfo[i][j].Same_internum)/(float)(Diff_covernotmain_intererea+1);
					Diff_covernotmain_intererea++;
				}

			}
			//************************************************************************
			//����MR ��ȨTCH�����
			
			int tempmin=min(gridinfo[i][j].TCH_Neighbor_Num,gridinfo[i][j].TCH_Same_Num);
			int goodnum=0;
			if(tempmin>0){
				for(int k=0;k<tempmin;k++){
					if(gridinfo[i][j].TCH_Neighbor_Vec[k]>-12 && gridinfo[i][j].TCH_Same_Vec[k]>9)
						goodnum++;
				}
				cover_MR_CI+=gridinfo[i][j].mrtraffic*goodnum/(float)tempmin;	
			}
			cover_mr_sum+=gridinfo[i][j].mrtraffic;
		}
	}
	/*
	

	Same_main_MRCI=Same_main_MRCI/main_MR_sum;
	Same_main_MRdropCI=Same_main_MRdropCI/main_MR_drop_sum;
	Diff_main_MRCI=Diff_main_MRCI/main_MR_sum;
	Diff_main_MRdropCI=Diff_main_MRdropCI/main_MR_drop_sum;

	Same_maincover_MRCI=Same_maincover_MRCI/maincover_MR_sum;
	Same_maincover_MRdropCI=Same_maincover_MRdropCI/maincover_MR_drop_sum;
	Diff_maincover_MRCI=Diff_maincover_MRCI/maincover_MR_sum;
	Diff_maincover_MRdropCI=Diff_maincover_MRdropCI/maincover_MR_drop_sum;
	
	reverseever=reversesum/(float)maincover_erea;
	
*/
	if(cover_mr_sum>0){
		cover_MR_CI=cover_MR_CI/(float)cover_mr_sum;
	}
	if(main_roaderea>0){
		Same_main_roadCI=Same_main_roadCI/(float)main_roaderea;
		Diff_main_roadCI=Diff_main_roadCI/(float)main_roaderea;
	}
	

	if(maincover_roaderea>0){
		Same_maincover_roadCI=Same_maincover_roadCI/(float)maincover_roaderea;
		Diff_maincover_roadCI=Diff_maincover_roadCI/(float)maincover_roaderea;
	}
	
	if(main_erea>0){
		main_contiever=main_bounderea/(float)main_erea;
		Same_main_CI=Same_main_nointererea/(float)main_erea;
		Diff_main_CI=Diff_main_nointererea/(float)main_erea;
		main_LOS=main_LOS/(float)main_erea;
		main_NLOS=main_NLOS/(float)main_erea;
		main_NLOS_1=main_NLOS_1/(float)main_erea;
	}
	if(maincover_erea>0){
		maincover_contiever=maincover_bounderea/(float)maincover_erea;
		Same_maincover_CI=Same_maincover_nointererea/(float)maincover_erea;
		Diff_maincover_CI=Diff_maincover_nointererea/(float)maincover_erea;
		maincover_LOS=maincover_LOS/(float)maincover_erea;
		maincover_NLOS=maincover_NLOS/(float)maincover_erea;
		maincover_NLOS_1=maincover_NLOS_1/(float)maincover_erea;
	}
	if(main_avgdistance>0){
		main_overcoverever=main_maxdistance/main_avgdistance;
	}
	if(maincover_avgdistance>0){
		maincover_overcoverever=maincover_maxdistance/maincover_avgdistance;
	}
	if(cover_erea>0){
		main_cover_erea=main_erea/(float)cover_erea;
		maincover_cover_erea=maincover_erea/(float)cover_erea;
		cover_LOS=cover_LOS/(float)cover_erea;
		cover_NLOS=cover_NLOS/(float)cover_erea;
		cover_NLOS_1=cover_NLOS_1/(float)cover_erea;
	}
	if(cover_avgdistance>0){
		main_cover_dist=main_avgdistance/(float)cover_avgdistance;
		maincover_cover_dist=maincover_avgdistance/(float)cover_avgdistance;
	}


	return true;

}

bool Evaluation::Get_Crossever(){
	int height=maxpoint.x-minpoint.x+1;
	int width=maxpoint.y-minpoint.y+1;
	if(height<2&&width<2)
		return false;
	int main_gridsum=0;
	float tempmain_crossever=0;
	int maincover_gridsum=0;
	float tempmaincover_crossever=0;
#pragma omp parallel for reduction(+:main_gridsum,tempmain_crossever,maincover_gridsum,tempmaincover_crossever)
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			float tempdis=Distance(cellinfo.grid_id,gridinfo[i][j].grid_id);
			if(tempdis<=main_avgdistance){
				main_gridsum++;
				if(gridinfo[i][j].maincellid!=cellinfo.cell_id)
					tempmain_crossever++;
			}
			if(tempdis<=maincover_avgdistance){
				maincover_gridsum++;
				if(gridinfo[i][j].maincellid!=cellinfo.cell_id)
					tempmaincover_crossever++;
			}
		}
	}
	if(main_gridsum>0)
		main_crossever=tempmain_crossever/(float)main_gridsum;
	if(main_gridsum)
		maincover_crossever=tempmaincover_crossever/(float)maincover_gridsum;
	return true;
	
}
bool Evaluation::Get_Bias(){//����ֻ���������ͣ����ڻ������⣬û�п���ͬƵ������Ƶ��
	Bias_Height=0;
	Bias_Distance=0;
//��ȡ�������ߵ�cell_id
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
		//system("pause");
		vector<Cellinfo> tempcellvector;
		tempcellvector.clear();
		ce.selectall(tempcellvector);
		PQfinish(conn);
//�Ӷ������ļ���Ѱ�����ߵ��ھ�����
		int cellsize=(int)tempcellvector.size();
		vector<int>covernum;//��ͬ���ǵ���������һ�������ֻҪ��һ�����ǾͿ��ԣ�����Ҳ�п��ܻ��Ǹ����������ﵽһ����Ŀ�ſ��Գ�Ϊ�ھ����ߣ������ڴ˶���˱���
		covernum.clear();
		covernum.resize(cellsize,0);
//#pragma omp parallel for
		for(int i=0;i<cellsize;i++){
			if(cellinfo.celltype==tempcellvector[i].celltype){//�������ͱ���һ�£�����վ��������վһ�����
				string cellcover_name=cellcover_path_name+(string)"\\field_"+networksort+(string)"\\"+tempcellvector[i].cell_id+(string)".dat";
				ifstream fi;
				fi.open(cellcover_name.c_str(),ios_base::in|ios_base::binary);
				//	system("pause");
				if(fi.is_open()==true){
				//	system("pause");
					Cover_GridPower tempgridpower;
					while (!fi.eof()){
						fi.read((char*)&tempgridpower,sizeof(tempgridpower));
						if(tempgridpower.grid_id==0)
							break;
						if(tempgridpower.receivepower>-94){//���㸲�������ż���
							Point temppoint=IdToCoordinate(tempgridpower.grid_id);
							if(temppoint.x>=minpoint.x&&temppoint.y>=minpoint.y&&temppoint.x<=maxpoint.x&&temppoint.y<=maxpoint.y\
								&&gridinfo[temppoint.x-minpoint.x][temppoint.y-minpoint.y].grid_id==tempgridpower.grid_id\
								&&gridinfo[temppoint.x-minpoint.x][temppoint.y-minpoint.y].cover_power>-94){//���������ߵĸ������򣬲���������ͬ����Ϊ��ȡ����󸲸����򣬼�ʹ��Ҳ��һ����ʵ���ǣ�������֤����id��
									covernum[i]++;
							}
						}
					}
				}
				fi.close();
			}
		}
		float average_height=0;
		float average_distance=0;
		int tempnum=0;
		for(int i=0;i<cellsize;i++){//ƽ�����롢ƽ���߶�
			if(covernum[i]>COVER_CANON){//�ص�������ֻҪ���������Ϊ���ھ�����
				float tempdistance=Distance(cellinfo.grid_id,tempcellvector[i].grid_id);
				average_distance=(tempdistance+average_distance*tempnum)/(float)(tempnum+1);//ƽ������֮��ľ���
				average_height=(tempcellvector[i].height+average_height*tempnum)/(float)(tempnum+1);
				tempnum++;
			}
		}
		float bias_distance=0;
		float bias_height=0;
		for(int i=0;i<cellsize;i++){
			if(covernum[i]>COVER_CANON){
				float tempdistance=Distance(cellinfo.grid_id,tempcellvector[i].grid_id);
				bias_distance+=(tempdistance-average_distance)*(tempdistance-average_distance);
				bias_height+=(tempcellvector[i].height-average_height)*(tempcellvector[i].height-average_height);
			}
		}
		if(tempnum>0&&average_height>0)
			Bias_Height=sqrt(bias_height/tempnum)/average_height;
		if(tempnum>0&&average_distance>0)
			Bias_Distance=sqrt(bias_distance/tempnum)/average_distance;
		return true;
}



