//从数据库中取数验证
bool Evaluation::GetMainPower_FromDB(){
//*******************************************************************************
//		并行读取postgres数据库中的maincell_tablename表中的主控小区号和对应的场强
//		场强存到类的公共变量vector<vector<float> > mainpower，是按（height，width)分布
//		主控小区存到临时变量tempmap中，结构为<cell_id,VEC>的map
//*******************************************************************************
	mainpower.clear();
	mainpower.resize(height);
	for(int i=0;i<height;i++){
		mainpower[i].clear();
		mainpower[i].resize(width,POWER);//POWER为常量-94
	}
	typedef map<string,VEC>::iterator MI;
	for(MI p=tempmap.begin();p!=tempmap.end();p++){
		p->second.gridid_vec.clear();
	}

//********************************************************************************
	int pronum=omp_get_num_procs();//电脑处理器个数，默认并行线程数
	vector<PGconn*> conn_array;//数据库并行的向量
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
	if(connect_flag==false){//有一个数据库没有打开
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
	int pronum=omp_get_num_procs();//电脑处理器个数，默认并行线程数
	vector<PGconn*> conn_array;//数据库并行的向量
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
	if(connect_flag==false){//有一个数据库没有打开
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
	//网格信息初始化
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
	//获取网格覆盖信息，不受-94的限制
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
	//获取地理信息
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
	//获取同频TCH干扰信息
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
	//获取邻频TCH干扰信息
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
	//MR数据
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
	//获取网格的主控场强值、地理信息和干扰信息 MR信息
	int pronum=omp_get_num_procs();//电脑处理器个数，默认并行线程数
	vector<PGconn*> conn_array;//数据库并行的向量
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
	if(connect_flag==false){//有一个数据库没有打开
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
			//覆盖区域的 平均场强和面积
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
			//主控边界的条件：在区域的第一行或者最后一行或者第一列或者最后一列，否则网格的上下左右邻网格至少有一个的主控场强值为-300
			if(gridinfo[i][j].maincellid==cellinfo.cell_id&&(i==0||i==height-1||j==0||j==width-1)){//主控边界
				main_boundpowerever=(main_boundpowerever*main_bounderea+gridinfo[i][j].main_power)/(float)(main_bounderea+1);//主控 边界平均场强值
				Point point_one=IdToCoordinate(cellinfo.grid_id);
				Point point_two=IdToCoordinate(gridinfo[i][j].grid_id);
				float tempdistance=GRID_SIZE*sqrt((float)(point_one.x-point_two.x)*(point_one.x-point_two.x)+(point_one.y-point_two.y)*(point_one.y-point_two.y));
				main_avgdistance=(main_avgdistance*main_bounderea+tempdistance)/(float)(main_bounderea+1);//平均主控边界距离
				main_maxdistance=max(tempdistance,main_maxdistance);//最远主控距离
				main_bounderea++;//主控边界面积
				if(gridinfo[i][j].main_power>=-94){
					//*************************************************************
					//主控覆盖的边界
					maincover_boundpowerever=(maincover_boundpowerever*maincover_bounderea+gridinfo[i][j].main_power)/(float)(maincover_bounderea+1);//主控覆盖平均场强值
					maincover_avgdistance=(maincover_avgdistance*maincover_bounderea+tempdistance)/(float)(maincover_bounderea+1);//平均主控覆盖边界距离
					maincover_maxdistance=max(tempdistance,maincover_maxdistance);//最远主控覆盖距离
					maincover_bounderea++;//主控覆盖边界面积
				}
			}
			else if(gridinfo[i][j].maincellid==cellinfo.cell_id&&(gridinfo[i][j-1].maincellid!=cellinfo.cell_id||gridinfo[i][j+1].maincellid!=cellinfo.cell_id||gridinfo[i-1][j].maincellid!=cellinfo.cell_id||gridinfo[i+1][j].maincellid!=cellinfo.cell_id)){//主控边界
				main_boundpowerever=(main_boundpowerever*main_bounderea+gridinfo[i][j].main_power)/(float)(main_bounderea+1);//主控平均场强值
				Point point_one=IdToCoordinate(cellinfo.grid_id);
				Point point_two=IdToCoordinate(gridinfo[i][j].grid_id);
				float tempdistance=GRID_SIZE*sqrt((float)(point_one.x-point_two.x)*(point_one.x-point_two.x)+(point_one.y-point_two.y)*(point_one.y-point_two.y));
				main_avgdistance=(main_avgdistance*main_bounderea+tempdistance)/(float)(main_bounderea+1);//平均主控边界距离
				main_maxdistance=max(tempdistance,main_maxdistance);//最远主控距离
				main_bounderea++;//主控边界面积
				if(gridinfo[i][j].main_power>=-94){//主控覆盖的边界
					maincover_boundpowerever=(maincover_boundpowerever*maincover_bounderea+gridinfo[i][j].main_power)/(float)(maincover_bounderea+1);//主控覆盖平均场强值
					maincover_avgdistance=(maincover_avgdistance*maincover_bounderea+tempdistance)/(float)(maincover_bounderea+1);//平均主控覆盖边界距离
					maincover_maxdistance=max(tempdistance,maincover_maxdistance);//最远主控覆盖距离
					maincover_bounderea++;//主控覆盖边界面积
				}
			}
			//***********************************************************************************
			//覆盖边界
			if(gridinfo[i][j].cover_power>-94&&(i==0||i==height-1||j==0||j==width-1)){//覆盖边界
				Point point_one=IdToCoordinate(cellinfo.grid_id);
				Point point_two=IdToCoordinate(gridinfo[i][j].grid_id);
				float tempdistance=GRID_SIZE*sqrt((float)(point_one.x-point_two.x)*(point_one.x-point_two.x)+(point_one.y-point_two.y)*(point_one.y-point_two.y));
				cover_avgdistance=(cover_avgdistance*cover_bounderea+tempdistance)/(float)(cover_bounderea+1);//平均覆盖边界距离
				cover_maxdistance=max(tempdistance,cover_maxdistance);//最远覆盖距离
				cover_bounderea++;//覆盖边界面积
			}
			else if(gridinfo[i][j].cover_power>=-94&&(gridinfo[i][j-1].cover_power<-94||gridinfo[i][j+1].cover_power<-94||gridinfo[i-1][j].cover_power<-94||gridinfo[i+1][j].cover_power<-94)){//覆盖边界
			Point point_one=IdToCoordinate(cellinfo.grid_id);
				Point point_two=IdToCoordinate(gridinfo[i][j].grid_id);
				float tempdistance=GRID_SIZE*sqrt((float)(point_one.x-point_two.x)*(point_one.x-point_two.x)+(point_one.y-point_two.y)*(point_one.y-point_two.y));
				cover_avgdistance=(cover_avgdistance*cover_bounderea+tempdistance)/(float)(cover_bounderea+1);//平均覆盖边界距离
				cover_maxdistance=max(tempdistance,cover_maxdistance);//最远覆盖距离
				cover_bounderea++;//覆盖边界面积
			}
			//************************************************************************************
			//道路信息
			if(gridinfo[i][j].maincellid==cellinfo.cell_id&&gridinfo[i][j].grid_type==1){//主控
				main_roadpowerever=(main_roadpowerever*main_roaderea+gridinfo[i][j].main_power)/(float)(main_roaderea+1);				
				if(gridinfo[i][j].Same_interpower>=CANONNUM){
					Same_main_roadCI=(Same_main_roadCI*main_roaderea+1)/(float)(main_roaderea+1);
				}
				if(gridinfo[i][j].Diff_interpower>=CANONNUM){
					Diff_main_roadCI=(Diff_main_roadCI*main_roaderea+1)/(float)(main_roaderea+1);
				}
				main_roaderea++;
				if(gridinfo[i][j].main_power>=-94){//主控覆盖
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
			//主控面积、平均场强值 同频/异频有无干扰面积   MR MR掉话 C\I
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
			//		Same_main_MRCI+=gridinfo[i][j].mrcount;//主控MRC\I 同频
			//		Same_main_MRdropCI+=gridinfo[i][j].mrcount_drop;//主控MR掉话C\I 同频
				}
				else{
					
					Same_main_interpower=(Same_main_interpower*Same_main_intererea+gridinfo[i][j].Same_interpower)/(float)(Same_main_intererea+1);
					Same_main_intererea++;
				}
				if(gridinfo[i][j].Diff_interpower>=CANONNUM){
					Diff_main_nointererea++;
			//		Diff_main_MRCI+=gridinfo[i][j].mrcount;//主控MRC\I 同频
			//		Diff_main_MRdropCI+=gridinfo[i][j].mrcount_drop;//主控MR掉话C\I 同频
				}
				else{
					Diff_main_interpower=(Diff_main_interpower*Diff_main_intererea+gridinfo[i][j].Diff_interpower)/(float)(Diff_main_intererea+1);
					Diff_main_intererea++;
				}
				//***********************************************
				//主控覆盖  同频/异频有无干扰面积   MR MR掉话 C\I
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
					double jiajiao=acos(yuxian)*180/PI;//因为给的初始值为度数 必须将弧度化为度数
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
				//		Same_maincover_MRCI+=gridinfo[i][j].mrcount;//主控MRC\I 同频
				//		Same_maincover_MRdropCI+=gridinfo[i][j].mrcount_drop;//主控MR掉话C\I 同频
					}
					else{
						Same_maincover_interpower=(Same_maincover_interpower*Same_maincover_intererea+gridinfo[i][j].Same_interpower)/(float)(Same_maincover_intererea+1);
						Same_maincover_intererea++;
					}
					if(gridinfo[i][j].Diff_interpower>=CANONNUM){
						Diff_maincover_nointererea++;
				//		Diff_maincover_MRCI+=gridinfo[i][j].mrcount;//主控MRC\I 同频
				//		Diff_maincover_MRdropCI+=gridinfo[i][j].mrcount_drop;//主控MR掉话C\I 同频
					}
					else{
						Diff_maincover_interpower=(Diff_maincover_interpower*Diff_maincover_intererea+gridinfo[i][j].Diff_interpower)/(float)(Diff_maincover_intererea+1);
						Diff_maincover_intererea++;
					}
				}

			}
			//****************************************************************
			//覆盖非主控信息
			if(gridinfo[i][j].maincellid!=cellinfo.cell_id&&gridinfo[i][j].cover_power>-94){//覆盖非主控
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
			//覆盖MR 加权TCH达标率
			
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
bool Evaluation::Get_Bias(){//天线只区分了类型（室内还是室外，没有考虑同频还是邻频）
	Bias_Height=0;
	Bias_Distance=0;
//获取所有天线的cell_id
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
		//system("pause");
		vector<Cellinfo> tempcellvector;
		tempcellvector.clear();
		ce.selectall(tempcellvector);
		PQfinish(conn);
//从二进制文件中寻找天线的邻居天线
		int cellsize=(int)tempcellvector.size();
		vector<int>covernum;//共同覆盖的网格数，一般情况是只要有一个覆盖就可以，但是也有可能会是覆盖网格数达到一定数目才可以成为邻居天线，所以在此定义此变量
		covernum.clear();
		covernum.resize(cellsize,0);
//#pragma omp parallel for
		for(int i=0;i<cellsize;i++){
			if(cellinfo.celltype==tempcellvector[i].celltype){//天线类型必须一致，室外站不和室内站一起计算
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
						if(tempgridpower.receivepower>-94){//满足覆盖条件才计算
							Point temppoint=IdToCoordinate(tempgridpower.grid_id);
							if(temppoint.x>=minpoint.x&&temppoint.y>=minpoint.y&&temppoint.x<=maxpoint.x&&temppoint.y<=maxpoint.y\
								&&gridinfo[temppoint.x-minpoint.x][temppoint.y-minpoint.y].grid_id==tempgridpower.grid_id\
								&&gridinfo[temppoint.x-minpoint.x][temppoint.y-minpoint.y].cover_power>-94){//网格在天线的覆盖区域，并且网格相同（因为是取的最大覆盖区域，即使在也不一定真实覆盖，必须验证网格id）
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
		for(int i=0;i<cellsize;i++){//平均距离、平均高度
			if(covernum[i]>COVER_CANON){//重叠网格数只要大于零就认为是邻居天线
				float tempdistance=Distance(cellinfo.grid_id,tempcellvector[i].grid_id);
				average_distance=(tempdistance+average_distance*tempnum)/(float)(tempnum+1);//平均天线之间的距离
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



