// test1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include "../gdal/gdal_priv.h"
#include "../gdal/gdal_alg.h"
#include "../gdal/gdal_priv.h"   
#include "../gdal/ogrsf_frmts.h"
using namespace std;

int main()
{	
	//注册GDAL
	GDALAllRegister();
	//注册OGR
	OGRRegisterAll();

	cout << "" << endl;
	return 0;
}

