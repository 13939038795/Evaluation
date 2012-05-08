// Evaluation_Compute.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"

#include <direct.h>
#include "Evaluation.h"
#include <omp.h>
#include <stdio.h>
#include <io.h>
#include <time.h>

int _tmain(int argc, _TCHAR* argv[])
{
	time_t start,end;
	start=clock();
//************************************************************
	string readpathname;
	if(argc==1||argc==0){
		string filepath="F:\\xudayong\\Evaluation\\ApplicationPrograms\\";
		readpathname=filepath+EVALUATION_COMPUTE_INFO_FILE_NAME;;
	}
	else if(argc==2){
		char tempstring[1000];
		int i=0;
		while(argv[1][i]!='\0'){
			tempstring[i]=(char)argv[1][i];
			i++;
		}
		tempstring[i]='\0';
		readpathname=(std::string)tempstring;
	}
//************************************************************	
	Evaluation tempeva;
	if(!tempeva.Readinfo(readpathname)){
		//读取配置信息
		return 0;
	}
//	tempeva.GetCellsInRegion();//获得所需计算天线信息
	//int cellsize=(int)tempeva.CellsInRegion.size();//天线个数
	string resultpath1=tempeva.result_path_name+(string)"\\EVA_RESULT";//结果文件夹
	if(_access(resultpath1.c_str(),0)==0)//为真表示文件夹存在,则不做任何处理，如果不存在则新建一个文件夹
	{
		//cout<<"directory exist\n";
	}
	else{
		string delresultpath=(string)"rmdir "+resultpath1+(string)" /q/s";
		_mkdir(resultpath1.c_str());
		if(_mkdir(resultpath1.c_str())==-1){
			system(delresultpath.c_str());
			_mkdir(resultpath1.c_str());
		}
	}
	string result_file_name=resultpath1+(string)"\\cell_eva_compute_"+tempeva.networksort+(string)"_"+tempeva.time;
	string result_file_name1=result_file_name+(string)".csv";
	string result_gridfile_name=resultpath1+(string)"\\grid_eva_compute_"+tempeva.networksort+(string)"_"+tempeva.time;
	string result_gridfile_name1=result_file_name+(string)".csv";
	int i=0;
	char num[64];
	while(_access(result_file_name1.c_str(),0)==0){//如果结果文档存在，则最后的标号加1，这是为了保存以前的计算结果
		sprintf_s(num,"%d",i);
		string result_file_name_new=result_file_name+(string)"_"+(string)num+".csv";
		string result_gridfile_name_new=result_gridfile_name+(string)"_"+(string)num+".csv";
		if(_access(result_file_name_new.c_str(),0)!=0||_access(result_gridfile_name_new.c_str(),0)!=0){
			result_file_name1=result_file_name_new;
			result_gridfile_name1=result_gridfile_name_new;
			break;
		}
		i++;
	}
/************************************************************
	vector<Evaluation> tempcellcompute;
	tempcellcompute.clear();
	tempcellcompute.resize(cellsize);
#pragma  omp parallel for
	for(int i=0;i<cellsize;i++){
	cout<<"************************************************************** i:"<<i<<endl;
		tempcellcompute[i].Readinfo(tempreadinfo);
		tempcellcompute[i].Getcellinfo(tempreadinfo.CellsInRegion[i]);
		tempcellcompute[i].Getcell_coverinfo();
		tempcellcompute[i].Getgrid_info();
		tempcellcompute[i].Grid_Compute();
		tempcellcompute[i].Get_Crossever();
		tempcellcompute[i].Get_Bias();
	}
*/
	if(tempeva.Eval()){
		
		bool r=tempeva.GetRegionKPI();
		if(r==true){
			cout<<"Writing Region Evluation Result File "<<result_file_name1<<endl;
			tempeva.WriteRegionResult(result_file_name1);
			//tempeva.WriteCellsResult(result_file_name1);
			tempeva.WriteGridsInfo(result_gridfile_name1);
			end=clock();
			cout<<"evaluation complete in total "<<difftime(end,start)/1000<<"s.\n";
			return 0;
		}
		else{
			cout<<"Evalution of Region is not complete due to errors"<<endl;
		}
		
	}
	else{
		cout<<"evaluation process error\n";
		return 0;
	}
}