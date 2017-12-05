# GDAL从入门到放弃

## 测试GDAL及一些常用函数

### 引用GDAL

	#include "../gdal/gdal_priv.h"
	#include "../gdal/gdal_alg.h"
	#include "../gdal/ogrsf_frmts.h"
	在资源文件夹里引入 gdal_i.lib 和 gdal14.dll

	编译事件
	COPY $(SolutionDir)gdal\gdal14.dll $(TargetDir)
	COPY $(SolutionDir)gdal\geos.dll $(TargetDir)
	COPY $(SolutionDir)gdal\geos_c.dll $(TargetDir)