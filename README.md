# GDAL从入门到放弃
	一直在用c#版的GDAL库，因为工作需要，转成了c++版，难度不并是GDAL本身，而是c# to c++

## GDAL来源

	官网： http://trac.osgeo.org/gdal/wiki/DownloadSource，下载的源码需要自己编译，网上有很多教程，但是，时间很长，还有失败的情况，这对于一个怕麻烦的人来说，是一件非常痛苦的事。后来，我选择坐在巨人的大腿上，直接下载巨人编好的库！

	官库：http://www.gisinternals.com/release.php，本例选择的版本是【2.2.1-vc2013-x64版】。
	链接分别：
		http://download.gisinternals.com/sdk/downloads/release-1800-x64-gdal-2-2-1-mapserver-7-0-6.zip
		包含依赖的DLL
		http://download.gisinternals.com/sdk/downloads/release-1800-x64-gdal-2-2-1-mapserver-7-0-6-libs.zip
		包含.h和.lib


## 引用GDAL方法 on VS2015

### 工程环境
	工程属性中：
		C/C++  -> 附加包含目录 ：指定到第二个包里解压出来的 include 目录
		链接器 -> 附加库目录 ：  指定到第二个包里解压出来的 lib 目录
		链接器 -> 输入 -> 附加依赖项 ：添加 gdal_i.lib

### 编译事件
	生成事件 -> 后期生成事件 -> 命令行 ：
		COPY D:\MyCode\GDAL221\bin\expat.dll $(TargetDir)
		COPY D:\MyCode\GDAL221\bin\freexl.dll $(TargetDir)
		COPY D:\MyCode\GDAL221\bin\gdal202.dll $(TargetDir)
		COPY D:\MyCode\GDAL221\bin\geos.dll $(TargetDir)
		COPY D:\MyCode\GDAL221\bin\geos_c.dll $(TargetDir)
		COPY D:\MyCode\GDAL221\bin\libeay32.dll $(TargetDir)
		COPY D:\MyCode\GDAL221\bin\libpq.dll $(TargetDir)
		COPY D:\MyCode\GDAL221\bin\libxml2.dll $(TargetDir)
		COPY D:\MyCode\GDAL221\bin\openjp2.dll $(TargetDir)
		COPY D:\MyCode\GDAL221\bin\proj.dll $(TargetDir)
		COPY D:\MyCode\GDAL221\bin\spatialite.dll $(TargetDir)
		COPY D:\MyCode\GDAL221\bin\sqlite3.dll $(TargetDir)
		COPY D:\MyCode\GDAL221\bin\ssleay32.dll $(TargetDir)
		COPY D:\MyCode\GDAL221\bin\iconv.dll $(TargetDir)

### 代码引用 
	#include "gdal_alg.h"
	#include "gdal_priv.h"
	#include "ogrsf_frmts.h"

### 验证代码
```
int main()
{
	GDALAllRegister();
	OGRRegisterAll();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	return 0;
}
```