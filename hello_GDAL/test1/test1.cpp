// test1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "dzx.h"

int main()
{	
	GDALAllRegister();
	OGRRegisterAll();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");

	OGRLayer* res = dzxRes();

	GDALDestroyDriverManager();
	return 0;
}

