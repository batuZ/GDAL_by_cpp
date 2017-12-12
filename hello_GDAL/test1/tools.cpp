#include "stdafx.h"
#include "tools.h"


#pragma region 数组的最大,最小,平均,方差
double array_MaxValue(double a[], int n)
{
	double haha = a[0];
	for (int i = 1; i < n; i++)
	{
		if (a[i] > haha)
			haha = a[i];
	}
	return haha;
}

double array_AveValue(double a[], int n)
{
	int sum = 0;
	for (int i = 0; i < n; i++)
		sum += a[i];
	return sum / n;
}

double array_minValue(double a[], int n)
{
	int mid;
	if (n % 2 == 0)
		mid = (a[n / 2 - 1] + a[n / 2]) / 2;
	else
		mid = a[(n - 1) / 2];
	return mid;
}

double array_varianceValue(double a[], int n)
{
	double sum = 0;
	double average = array_AveValue(a, n);
	for (int i = 0; i < n; i++)
		sum += (a[i] - average)*(a[i] - average);
	return sum / n;
}
#pragma endregion

#pragma region GDAL的补充函数
//通过名字删除Layer
void deleteLayerByName(GDALDataset *ds, const char *name)
{
	if (ds != NULL)
		for (size_t i = 0; i < ds->GetLayerCount(); i++)
			if (!strcmp(ds->GetLayer(i)->GetName(), name))
			{
				ds->DeleteLayer(i);
				break;
			}
}

#pragma endregion