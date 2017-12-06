#include "stdafx.h"
#include "dzx.h"



char* filePath = "C:\\temp\\outlinetest01.img";
char* shpPath = "C:\\temp\\asf";
int jiange = 2;
GDALDataset * dsmDataSet = (GDALDataset *)GDALOpen(filePath, GA_ReadOnly);
OGRSFDriver *shpDriver = (OGRSFDriver *)OGRGetDriverByName("ESRI Shapefile");
OGRDataSource *shpDataSource = shpDriver->CreateDataSource(shpPath, NULL);
OGRSpatialReference *dsmWKT = new OGRSpatialReference(dsmDataSet->GetProjectionRef());


void cleanDZX(OGRLayer * layer)
{
	OGRLayer *cleanLayer = shpDataSource->CreateLayer("dzxClean", dsmWKT, wkbLineString, NULL);

}

void getBZC(OGRLayer * layer, double * aue, double * bzc)
{
	int featCount = layer->GetFeatureCount();
	double* values = new double(featCount);
	for (int i = 0; i < featCount; i++)
		values[i] = layer->GetFeature(i)->GetFieldAsDouble("Elevation");

	*aue = array_AveValue(values, featCount);
	*bzc = sqrt(array_varianceValue(values, featCount));

	delete values;
}

void getDZX()
{
	if (dsmDataSet != NULL)
	{
		cout << "获取图像" << filePath << endl;
		GDALRasterBand *band = dsmDataSet->GetRasterBand(1);

		//创建矢量图
		OGRLayer *dzxLayer = shpDataSource->CreateLayer("dzx", dsmWKT, wkbLineString, NULL);

		//在矢量图中创建高程值字段
		OGRFieldDefn ofieldDef1("Elevation", OFTInteger);
		dzxLayer->CreateField(&ofieldDef1);

		GDALContourGenerate(band, jiange, 0, 0, NULL, false, 0, (OGRLayerH)dzxLayer, -1, 0, NULL, NULL);
		
		//GDALClose(shpDataSource);
		//GDALClose(dsmDataSet);
	}
	else
		return;
}
