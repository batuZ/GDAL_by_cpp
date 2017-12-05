#include "stdafx.h"
#include "dzx.h"

void getDZX()
{
	char* filePath = "C:\\temp\\outlinetest01.img";
	char* shpPath = "C:\\temp\\asf";
	int jiange = 2;
	GDALDataset * dsmDataSet = (GDALDataset *)GDALOpen(filePath, GA_ReadOnly);

	if (dsmDataSet != NULL)
	{
		cout << "��ȡͼ��" << filePath << endl;
		GDALRasterBand *band = dsmDataSet->GetRasterBand(1);

		//����ʸ��ͼ
		OGRSFDriver *shpDriver = (OGRSFDriver *)OGRGetDriverByName("ESRI Shapefile");
		OGRDataSource *shpDataSource = shpDriver->CreateDataSource(shpPath, NULL);
		OGRSpatialReference *dsmWKT = new OGRSpatialReference(dsmDataSet->GetProjectionRef());
		OGRLayer *dzxLayer = shpDataSource->CreateLayer("ddzx", dsmWKT, wkbLineString, NULL);

		//��ʸ��ͼ�д����߳�ֵ�ֶ�
		OGRFieldDefn ofieldDef1("Elevation", OFTInteger);
		dzxLayer->CreateField(&ofieldDef1);

		GDALContourGenerate(band, jiange, 0, 0, NULL, false, 0, (OGRLayerH)dzxLayer, -1, 0, NULL, NULL);

		GDALClose(shpDataSource);
		GDALClose(dsmDataSet);
	}
	else
		return;
}
