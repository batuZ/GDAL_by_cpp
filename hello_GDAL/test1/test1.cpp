// test1.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "dzx.h"

int main()
{	



	GDALAllRegister();
	OGRRegisterAll();
	//֧������·��
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	CPLSetConfigOption("SHAPE_ENCODING", "");

	OGRLayer* res = dzxRes();


	GDALDestroyDriverManager();
	return 0;
}

