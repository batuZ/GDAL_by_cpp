// test1.cpp : �������̨Ӧ�ó������ڵ㡣
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

