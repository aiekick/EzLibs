[![Win](https://github.com/aiekick/EzLibs/actions/workflows/Win.yml/badge.svg)](https://github.com/aiekick/EzLibs/actions/workflows/Win.yml)
[![Linux](https://github.com/aiekick/EzLibs/actions/workflows/Linux.yml/badge.svg)](https://github.com/aiekick/EzLibs/actions/workflows/Linux.yml)
[![Osx](https://github.com/aiekick/EzLibs/actions/workflows/Osx.yml/badge.svg)](https://github.com/aiekick/EzLibs/actions/workflows/Osx.yml)

# EzLibs

its my collection of Cpp11 cross platform self made "one header file only" libs

# dependencies 

No External libraries dependencies needed except Cpp11 standard library

Just the lib [EzGL](doc/EzGL.md) is depending of a Opengl Loader like [Glad](https://github.com/Dav1dde/glad/tree/c)

# HowTo 

you just need to include the libs in your project and use it. thats all.

Some libs are in inter dependencies, so you may need to include the 
EzLibs dir in your building system

Some libs need to use a define one time for create implementation 
and let the linker know where are defined methods. But is a rare sheme.

ex: (cf EzLog)

main.cpp : One time only defined
```cpp
#define EZ_LOG_IMPLEMENTATION
#include <EzLibs/EzLog.hpp>
```

toto.cpp : No more define needed
```cpp
#include <EzLibs/EzLog.hpp> 
```

# Available Libs categories :

# App :

|Lib|Doc|Tested|Status|Description|
|-|-|-|-|-|
|[EzApp](doc/EzApp.md)|:no_entry:|:no_entry:|:construction:|App Manipulation, Current working dir|
|[EzArgs](doc/EzArgs.md)|:construction:|:heavy_check_mark:|:heavy_check_mark:|Command Options parsing|

# Comp :

|Lib|Doc|Tested|Status|Description|
|-|-|-|-|-|
|[EzLzw](doc/EzMLzw.md)|:no_entry:|:construction:|:construction:|Compress/Decomp LZW|

# File :

|Lib|Doc|Tested|Status|Description|
|-|-|-|-|-|
|[EzFile](doc/EzFile.md)|:no_entry:|:no_entry:|:construction:|File Manipulation|

|Data|Doc|Tested|Status|Description|
|-|-|-|-|-|
|[EzIni](doc/EzIni.md)|:no_entry:|:no_entry:|:construction:|Ini File Reader/Writer|
|[EzLog](doc/EzLog.md)|:no_entry:|:no_entry:|:construction:|Log File Writer|
|[EzXmlConfig](doc/EzXmlConfig.md)|:no_entry:|:heavy_check_mark:|:heavy_check_mark:|xml config File Reader/Writer|

|2D Img|Doc|Tested|Status|Description|
|-|-|-|-|-|
|[EzBmp](doc/EzBmp.md)|:no_entry:|:construction:|:construction:|Bitmap Reader/Writer|
|[EzGif](doc/EzGif.md)|:no_entry:|:construction:|:construction:|Gif Reader/Writer|
|[EzPng](doc/EzPng.md)|:no_entry:|:no_entry:|:construction:|Gif Reader/Writer|

|3D Model|Doc|Tested|Status|Description|
|-|-|-|-|-|
|[EzVdbWriter](doc/EzVdbWriter.md)|:no_entry:|:no_entry:|:construction:|OpenVdb partial file format writer|
|[EzVoxWriter](doc/EzVoxWriter.md)|:no_entry:|:no_entry:|:construction:|Magicavoxel vox file format writer|

# Math :

|Lib|Doc|Tested|Status|Description|
|-|-|-|-|-|
|[EzExpr](doc/EzExpr.md)|:construction:|:heavy_check_mark:|:heavy_check_mark:|Math expression parsing and computing|
|[EzMath](doc/EzMath.md)|:no_entry:|:construction:|:construction:|Math methods|
|[EzVec2](doc/EzVec2.md)|:no_entry:|:construction:|:heavy_check_mark:|Vec2 Manipulation|
|[EzVec3](doc/EzVec3.md)|:no_entry:|:construction:|:heavy_check_mark:|Vec3 Manipulation|
|[EzVec4](doc/EzVec4.md)|:no_entry:|:construction:|:heavy_check_mark:|Vec4 Manipulation|
|[EzQuat](doc/EzQuat.md)|:no_entry:|:no_entry:|:construction:|Quaternion Manipulation|
|[EzPlane](doc/EzPlane.md)|:no_entry:|:no_entry:|:construction:|***WIP***|
|[EzAABB](doc/EzAABB.md)|:no_entry:|:no_entry:|:construction:|***WIP***|
|[EzAABBCCC](doc/EzAABBCCC.md)|:no_entry:|:no_entry:|:construction:|***WIP***|
|[EzVariant](doc/EzVariant.md)|:no_entry:|:no_entry:|:construction:|***WIP***|

# Misc :

|Lib|Doc|Tested|Status|Description|
|-|-|-|-|-|
|[EzGraph](doc/EzGraph.md)|:no_entry:|:heavy_check_mark:|:heavy_check_mark:|Internal Node Graph System|
|[EzStr](doc/EzStr.md)|:no_entry:|:construction:|:construction:|String Manipulation|
|[EzXml](doc/EzXml.md)|:no_entry:|:heavy_check_mark:|:heavy_check_mark:|xml Parsing|

# Opengl :

|Lib|Doc|Tested|Status|Description|
|-|-|-|-|-|
|[EzGL](doc/EzGL.md)|:no_entry:|:construction:|:construction:|Opengl manipulation|

# Time :

|Lib|Doc|Tested|Status|Description|
|-|-|-|-|-|
|[EzTime](doc/EzTime.md)|:no_entry:|:no_entry:|:construction:|Time manipulation|
|[EzCron](doc/EzCron.md)|:construction:|:heavy_check_mark:|:heavy_check_mark:|Crontab parsing and time execution|

# License

Copyright :copyright: 2014-2024 Stephane Cuillerdier

See the [License file](LICENSE)
