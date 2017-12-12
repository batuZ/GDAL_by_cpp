// test1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "dzx.h"

int main()
{	



	GDALAllRegister();
	OGRRegisterAll();
	//支持中文路径
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	CPLSetConfigOption("SHAPE_ENCODING", "");

	OGRLayer* res = dzxRes();


	GDALDestroyDriverManager();
	return 0;
}

