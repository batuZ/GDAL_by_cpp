// hello_GDAL.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
//#include "../gdal/gdal_priv.h"
//#include "../gdal/gdal_alg.h"
//#include "../gdal/ogrsf_frmts.h"
#include"../gdal221/include/gdal_alg.h"
#include "../gdal221/include/gdal_priv.h"
#include "../gdal221/include/ogrsf_frmts.h"
using namespace std;


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
	char* inFile = "E:\\work\\A-全国资源\\DOM\\Lev13_China\\A03-78,45,73,40.img";
	char* outFile = "C:\\temp\\ff.jpg";
	GDALAllRegister();
	GDALDataset *poSrcDS = (GDALDataset*)GDALOpen(inFile, GA_ReadOnly);
	if (poSrcDS == NULL)
	{
		printf("打开图像失败！");
		return 0;
	}
	//获取图像的基本信息,并把数据读入缓冲区间  
	int m_Width = poSrcDS->GetRasterXSize();
	int m_Height = poSrcDS->GetRasterYSize();
	double* transform = new double[6];
	poSrcDS->GetGeoTransform(transform);

	double s1 = transform[1];	//1
	double s2 = transform[2];	//2
	double s3 = transform[4];	//3
	double s4 = transform[5];	//4
	double s5 = transform[0];	//5
	double s6 = transform[3];	//6
	const char* pro = poSrcDS->GetProjectionRef();

	//把数据保存到临时文件MEM  
	GDALDriver *pDriverMEM = GetGDALDriverManager()->GetDriverByName("MEM");
	GDALDataset *pOutMEMDataset = pDriverMEM->Create("", m_Width, m_Height, 1, GDT_Byte, NULL);
	unsigned char *buffer = new unsigned char[m_Width*m_Height]; //存储数据的缓冲区  
	GDALRasterBand *poSrcDSRasterBand;
	int i = 1;

	while ((poSrcDSRasterBand = poSrcDS->GetRasterBand(i)) != NULL)
	{
		CPLErr err = poSrcDSRasterBand->RasterIO(GF_Read, 0, 0, m_Width, m_Height, buffer, m_Width, m_Height, GDT_Byte, 0, 0);
		if (err != CE_None)
		{
			printf("读取图像数据失败！");
			return 0;
		}
		/* 以下程序是生成JPG的过程，上面的程序只为获得创建过程中的一些参数，如：图像的高、宽及图像数据*/

		GDALRasterBand *pOutMEMRasterBand = pOutMEMDataset->GetRasterBand(i);
		if (pOutMEMRasterBand == NULL)
		{
			pOutMEMDataset->AddBand(GDT_Byte);
			pOutMEMRasterBand = pOutMEMDataset->GetRasterBand(i);
		}
		err = pOutMEMRasterBand->RasterIO(GF_Write, 0, 0, m_Width, m_Height, buffer, m_Width, m_Height, GDT_Byte, 0, 0);
		if (err != CE_None)
		{
			printf("写图像数据失败！");
			return 0;
		}
		i++;
	}
	//以创建复制的方式，生成jpg文件  
	GDALDriver *pDriverJPG = GetGDALDriverManager()->GetDriverByName("JPEG");
	pDriverJPG->CreateCopy(outFile, pOutMEMDataset, TRUE, 0, 0, 0); //创建jpg文件  

	return 0;
}