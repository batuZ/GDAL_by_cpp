// hello_GDAL.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include "../gdal/gdal_priv.h"
#include "../gdal/gdal_alg.h"
#include "../gdal/gdal_priv.h"   
#include "../gdal/ogrsf_frmts.h"
using namespace std;

void getAue(double* arr, double* res)
{
	double f = 0;
	for (size_t i = 0; i < 3; i++)
	{
		f += arr[i];
	}
	*res = f / 3;
}

#pragma region MyRegion



void callGDAL()
{
	//注册GDAL
	GDALAllRegister();

	//栅格数据集
	GDALDataset *poDataset;
	char* filePath = "C:\\code\\For_Gdal\\mosaic.tif";

	//打开一个栅格
	poDataset = (GDALDataset *)GDALOpen(filePath, GA_ReadOnly);

	//调GDALDataset的方法。。。。
	char const *dd = poDataset->GetDriver()->GetDescription();
}

void callOGR()
{
	//注册OGR
	OGRRegisterAll();

	//Driver
	const char *pszDriverName = "ESRI Shapefile";
	OGRSFDriver *poDriver = (OGRSFDriver *)OGRGetDriverByName(pszDriverName);

	//矢量数据源
	OGRDataSource *poDatasource = poDriver->CreateDataSource("path", NULL);//创建数据源

	OGRLayer *lyr = poDatasource->GetLayer(0);
}


//创建坡度图 http://blog.csdn.net/liminlu0314/article/details/8498985


// 创建等值线方法	GDALContourGenerate(。。。); http://blog.csdn.net/hong__fang/article/details/51605030


bool CalContourByGdal(string imgPath, string shpPath, double dfContourInterval)
{
	OGRRegisterAll();
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");

	GDALDataset * pInDataset = (GDALDataset *)GDALOpen(imgPath.c_str(), GA_ReadOnly);
	if (pInDataset == NULL)
	{
		printf("读取图像失败！");
		return FALSE;
	}

	int nDemWidth = pInDataset->GetRasterXSize(); //获取图像宽  
	int nDemHeight = pInDataset->GetRasterYSize(); //获取图像高  
	int Count = pInDataset->GetRasterCount(); //波段数  


											  //读取图像数据波段
	GDALRasterBand *pInRasterBand = pInDataset->GetRasterBand(1);
	float *pData = new float[nDemWidth*nDemHeight]();

	CPLErr err = pInRasterBand->RasterIO(GF_Read, 0, 0, nDemWidth, nDemHeight, pData, nDemWidth, nDemHeight, GDT_Float32, 0, 0);
	if (err == CE_Failure)
	{
		cout << "读取DEM图像数据时出错！" << endl;
		GDALDestroyDriverManager();
		return 0;
	}

	//判断图像中是否有异常值，并获取异常值实际值
	float fNoData = 0;
	int nIdx;
	for (int i = 0; i < nDemHeight; i++)
	{
		for (int j = 0; j < nDemWidth; j++)
		{
			nIdx = i*nDemWidth + j;
			if (pData[nIdx] <= -9999)
			{
				fNoData = pData[nIdx];
			}
		}
	}


	//创建矢量图
	const char *pszDriverName = "ESRI Shapefile";
	OGRSFDriver *poDriver;
	GDALAllRegister();
	poDriver = (OGRSFDriver *)OGRGetDriverByName(pszDriverName);
	if (poDriver == NULL)
	{
		printf("%s driver not available.\n", pszDriverName);
		exit(1);
	}
	OGRDataSource *poDS;
	poDS = poDriver->CreateDataSource(shpPath.c_str(), NULL);//创建数据源
	if (poDS == NULL)
	{
		printf("Creation of output file failed.\n");
		exit(1);
	}

	OGRLayer *poLayer;
	OGRSpatialReference *poSpatialRef = new OGRSpatialReference(pInDataset->GetProjectionRef());
	poLayer = poDS->CreateLayer("Elevation", poSpatialRef, wkbLineString, NULL); //创建图层
	if (poLayer == NULL)
	{
		printf("Layer creation failed.\n");
		exit(1);
	}


	OGRFieldDefn ofieldDef1("Elevation", OFTInteger);    //在矢量图中创建高程值字段
	if (poLayer->CreateField(&ofieldDef1) != OGRERR_NONE)
	{
		cout << "创建矢量图层属性表失败！" << endl;
		GDALClose((GDALDatasetH)poDS);
		GDALClose(pInDataset);
		return 0;
	}

	//根据图像波段生成矢量图等高线
	if (fNoData == 0)
		GDALContourGenerate(pInRasterBand, dfContourInterval, 0, 0, NULL, false, 0, (OGRLayerH)poLayer, -1, 0, NULL, NULL);
	else //有异常值时，不对异常值进行处理
		GDALContourGenerate(pInRasterBand, dfContourInterval, 0, 0, NULL, true, fNoData, (OGRLayerH)poLayer, -1, 0, NULL, NULL);

	GDALClose(poDS);
	GDALClose(pInDataset);


	if (pData != NULL)
	{
		delete[] pData;
		pData = NULL;
	}

}

int _tmain(int argc, _TCHAR* argv[])
{
	string imgPath = "F:\\2.tif";
	string shpPath = "C:\\contour.shp";


	CalContourByGdal(imgPath, shpPath, 500);

	cout << "处理完成" << endl;
	getchar();

	return 0;
}
#pragma endregion
int main()
{
	double ss[] = { 44.4,55.5,66.6 ,77.7};
	double* rrr = new double();
	getAue(ss, rrr);
	return 0;
}