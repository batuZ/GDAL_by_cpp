// test1.cpp : �������̨Ӧ�ó������ڵ㡣
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
	//ע��GDAL
	GDALAllRegister();
	//ע��OGR
	OGRRegisterAll();

	cout << "" << endl;
	return 0;
}

