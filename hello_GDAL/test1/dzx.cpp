#include "stdafx.h"
#include "dzx.h"


char* filePath = "C:\\temp\\outlinetest01.img";
char* shpPath = "C:\\temp\\asf";
int jiange = 2;
double mimLength = 200;
double maxLength = 2000;
bool isDelete = true;
//img
GDALDataset * dsmDataSet;

//shp
GDALDriver *shpDriver;
GDALDataset *shpDataSet;

OGRSpatialReference *dsmWKT;


//���
OGRLayer * dzxRes()
{

	//��ȡͼ��
	dsmDataSet = (GDALDataset *)GDALOpen(filePath, GA_ReadOnly);
	//�ļ����ڣ��ų�3����Ӱ��
	if (dsmDataSet == NULL || dsmDataSet->GetRasterCount() != 1)return NULL;
	//����shp
	shpDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName("ESRI Shapefile");
	shpDataSet = shpDriver->Create(shpPath, 0, 0, 0, GDT_Unknown, NULL);
	//��ȡͶӰ  
	const char* dsmwkt = dsmDataSet->GetProjectionRef();
	dsmWKT = strcmp(dsmwkt, "") ? new OGRSpatialReference(dsmwkt) : NULL;

	OGRLayer * dzx = getDZX();
	OGRLayer * dzxClean = cleanDZX(dzx);

	/*GDALClose(shpDataSet);
	GDALClose(dsmDataSet);*/
	return dzxClean;
}

OGRLayer * getDZX()
{
	GDALRasterBand *band = dsmDataSet->GetRasterBand(1);

	//����ʸ��ͼ
	deleteLayerByName(shpDataSet, "dzx");
	OGRLayer *dzxLayer = shpDataSet->CreateLayer("dzx", dsmWKT, wkbLineString, NULL);

	//��ʸ��ͼ�д����߳�ֵ�ֶ�
	OGRFieldDefn ofieldDef1("Elevation", OFTInteger);
	dzxLayer->CreateField(&ofieldDef1);

	GDALContourGenerate(band, jiange, 0, 0, NULL, false, 0, (OGRLayerH)dzxLayer, -1, 0, NULL, NULL);

	GDALClose(band);
	return dzxLayer;
}

OGRLayer * cleanDZX(OGRLayer * layer)
{
	deleteLayerByName(shpDataSet, "dzxClean");
	OGRLayer *cleanLayer = shpDataSet->CreateLayer("dzxClean", dsmWKT, wkbLineString, NULL);
	double *aue = new double(), *bzc = new double(), maxV, minV;
	getBZC(layer, aue, bzc);
	minV = *aue - *bzc * jiange;
	maxV = *aue + *bzc * jiange;
	int featCount = layer->GetFeatureCount();
	for (size_t i = 0; i < featCount; i++)
	{
#pragma region �ж�����
		OGRFeature * af = layer->GetFeature(i);
		OGRGeometry *geo = af->GetGeometryRef();
		OGRwkbGeometryType geoType = geo->getGeometryType();
		OGRLineString * geoLine = (OGRLineString *)geo;
		//�߳�
		double lth = geoLine->get_Length();
		//�߶�ֵ
		double val = af->GetFieldAsDouble("Elevation");
		//�Ƿ�պ�
		int pointCount = geoLine->getNumPoints();
		double p1X = geoLine->getX(0);
		double p1Y = geoLine->getY(0);
		double peX = geoLine->getX(pointCount - 1);
		double peY = geoLine->getY(pointCount - 1);
#pragma endregion

		if (lth > mimLength && lth < maxLength && val > minV && val < maxV&&p1X == peX && p1Y == peY)
		{
			cleanLayer->CreateFeature(af);
		}
		OGRFeature::DestroyFeature(af);
	}
	if (isDelete)
		deleteLayerByName(shpDataSet, layer->GetName());

	return cleanLayer;
}

void getBZC(OGRLayer * layer, double * aue, double * bzc)
{
	int featCount = layer->GetFeatureCount();
	double * values = new double[featCount];
	for (int i = 0; i < featCount; i++)
		values[i] = layer->GetFeature(i)->GetFieldAsDouble("Elevation");
	*aue = array_AveValue(values, featCount);
	*bzc = sqrt(array_varianceValue(values, featCount));
	delete values;
}