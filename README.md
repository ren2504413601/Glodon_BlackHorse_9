广联达黑马大赛

- 时间：7月7日-7月20日

- 主题方向：数据处理-裂纹检测

- 文件结构

    Glodon_BlackHorse_9
    │  README.md
    │
    ├─Results 检测结果
    │      afterKernelRes.png
    │      canneyRes.png
    │      mixRes.png
    │      orgConnRes.png
    │      readme.md
    │      redRes.png
    │      result1.png
    │      true.png
    │
    ├─SrcFigures 待识别裂纹图片
    │      001.jpg
    │      001.mat
    │      004.jpg
    │      004.mat
    │      README.md
    │
    └─STRUCTURE 主要代码和安装包
        │  README.md
        │
        ├─01-App `Android App`代码和`app`
        │      README.md
        │
        └─02-Graphic `Opencv`图形开发代码
            │  README.md
            │
            └─01-SSR_sln
                ├─00-sln
                │  │  GraphicCmdSln.sln
                │  │
                │  ├─GraphicCmdSln
                │  │      GraphicCmdSln.vcxproj
                │  │      GraphicCmdSln.vcxproj.filters
                │  │      GraphicCmdSln.vcxproj.user
                │  │
                │  └─x64
                │      └─Debug
                │              SSR_Graphic.exe
                │              SSR_Graphic.ilk
                │              SSR_Graphic.pdb
                │
                └─01-Identify&Feature
                    │  CvGeFunctions.cpp
                    │  CvGeFunctions.h
                    │  CvProgramFunctions.cpp
                    │  CvProgramFunctions.h
                    │  EntryFun.cpp
                    │  EXIF.cpp
                    │  EXIF.h
                    │  GlobalParams.h
                    │  TestOpenCv.vcxproj
                    │  TestOpenCv.vcxproj.filters
                    │  TestOpenCv.vcxproj.user
                    │
                    └─SSR_Graphic
                        │  black.tiff
                        │  SSR_Graphic.vcxproj
                        │  SSR_Graphic.vcxproj.filters
                        │  SSR_Graphic.vcxproj.user
                        │
                        ├─Debug
                        │  │  EXIF.obj
                        │  │  SSR_Graphic.log
                        │  │  vc142.idb
                        │  │  vc142.pdb
                        │  │
                        │  └─SSR_Graphic.tlog
                        │          CL.command.1.tlog
                        │          CL.read.1.tlog
                        │          CL.write.1.tlog
                        │          SSR_Graphic.lastbuildstate
                        │          unsuccessfulbuild
                        │
                        └─x64
                            └─Debug
                                │  CvGeFunctions.obj
                                │  CvProgramFunctions.obj
                                │  EntryFun.obj
                                │  EXIF.obj
                                │  SSR_Graphic.Build.CppClean.log
                                │  SSR_Graphic.log
                                │  vc141.idb
                                │  vc141.pdb
                                │
                                └─SSR_Graphic.tlog
                                        CL.command.1.tlog
                                        CL.read.1.tlog
                                        CL.write.1.tlog
                                        link.command.1.tlog
                                        link.read.1.tlog
                                        link.write.1.tlog
                                        SSR_Graphic.lastbuildstate
                              

  - `SrcFigures` ：待识别裂纹图片存放位置
  - `STRUCTURE` : 主要代码和安装包
    - `STRUCTURE/01-App` : `Android App`代码和`app`
    - `STRUCTURE/02-Graphic` : `Opencv`图形开发代码

- 环境：
  - Opencv 4.3.0
  - Matlab2016b
