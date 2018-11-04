# AgoraLiveShop

*Read this in other languages: [English](README.md)*

使用到的库：

* agora sdk 2.3.1 special version(for sei)
* zlib(for generate token)
* obs 20.1(for capture obs video source and audio)
* sqlite3 (for store local data)

这个开源示例项目演示了如何快速集成电商直播答题

在这个示例项目中包含了以下功能：

* 通过sei发送直播题目和产品广告
* 直播主讲，yy伴侣实现视频特效
* 双摄像头，有一个可以作为产品广告
* obs studio 可以作为视频源
* 本地存储题库和产品广告信息

本开源项目使用 **C++** 语言

## 运行示例程序
首先在 [Agora.io 注册](https://dashboard.agora.io/cn/signup/) 注册账号，并创建自己的测试项目，获取到 AppID。修改配置文件HQConfig.ini

```
[LoginInfo]

AppId=

AppCertificateId=

LoginUid=

AppCertEnable=

VideoSolutinIndex=

CameraDeviceName=

CameraDeviceID=

```

最后使用 VC++2013 打开 AgoraLiveShop-Windows.sln，编译整个解决方案即可运行，

在程序主目录下有一个restart.bat脚本，需要将restart.bat copy到执行目录下.

## 运行环境
* VC++ 2013(或更高版本)
* Windows7(或更高版本)

## 联系我们

- 完整的 API 文档见 [文档中心](https://docs.agora.io/cn/)
- 如果在集成中遇到问题，你可以到 [开发者社区](https://dev.agora.io/cn/) 提问
* 如果有售前咨询问题，可以拨打 400 632 6626，或加入官方Q群 12742516 提问
- 如果需要售后技术支持，你可以在 [Agora Dashboard](https://dashboard.agora.io) 提交工单
- 如果发现了示例代码的bug，欢迎提交 [issue](https://github.com/AgoraIO/OpenLive-Windows/issues)

## 代码许可

The MIT License (MIT).

#如何集成声网sdk实现以上功能

## 集成声网sdk

SDK的集成可以参考
[官网文档](!https://docs.agora.io/cn/?_ga=2.119862408.1634715518.1540813689-2048440635.1534656968)

有两点需要注意：

### 本地生成token

**正常的流程：**

* 要由客户服务器生成token
* 客户端向服务器请求token
* 然后在joinChannel的时候使用请求的token。

**目前为了方便简化流程**

* 在本地客户端生成token(GenerateToken)，生成token会使用到开元库zlib。关于生成token代码，在vs2013中的AgoraHQ的AgoraObject下面的token。
* 然后在joinChannel的时候使用请求的token。

关于token的详细文档说明请参考：
[官网token文档](!https://docs.agora.io/cn/Interactive%20Broadcast/token?platform=All%20Platforms)

### 使用SEI发送信息

* 该版本sdk是基于2.3.1的特殊版本sdk，主要是为了提供SEI发送题库和产品信息
* 添加了两个回调函数

    * onSendSEI： 导播端目前只用到onSendSEI
    * onReceiveSEI
  
    **注意**onSendSEI的info只有1024个字节，所以发送的消息长度注意控制。

* [zlib下载地址](!http://gnuwin32.sourceforge.net/packages/zlib.htm)

## 使用liobs(obs studio)库

obs库的具体细节可以参考[obs studio的官网文档](!obsproject.com)

该obs库是基于开元的obs studio修改，增加了一个插件win-agora，该插件只是负责将obs的视频合图和音频混音的数据导出到一个共享的buffer，然后由agora sdk裸数据接口获得buffer中的数据复制到agora sdk的buffer中，可以利用obs的一些高级功能。

目前用到的obs的视频源包括：

* camera source
* media source(ffmpeg)
* Image source

每种source都支持抠图效果，为source添加filter效果即可实现。

关于声网修改之后的libobs库已经放到**obs-sdk**文件夹中，头文件和lib文件放在obs-studio中。

使用obs作为采集源需要注意两点：

* 调用AgoraObject::EnableExtendAudioCapture和AgoraObject::EnableExtendVideooCapture，参数为TRUE,注册声网裸数据接口。参数为FALSE，注销裸数据接口。
* 通过AgoraOutputHandler::SetEncoderCallback注册obs的视频和音频回调函数，最终obs的合图和混音数据都会通过注册的回调函数提供给agora sdk。

[裸数据官网文档](!https://docs.agora.io/cn/Interactive%20Broadcast/rawdata_windows?platform=Windows)

**注意：** 在导播端的demo代码中所有注释**OBS for ExtCapture**的地方为obs相关代码开始的地方

## 关于双摄像头如何实现

声网sdk目前是单引擎实例，也就是意味着一个进程只能有一路视频。有些特殊情况要使用多录视频就必须使用到多进程。双摄像的就是基于此实现的，原理很简单：

* 起两个进程，每个进程占用一个uid。二者都以主播身份进入频道
* 主播视频由主进程发送，也发送音频(AgoraHQ.exe)，注意不接收产品广告进程的视频(mutRemoteVideo)。
* 另一个进程(AdvertiseVideoSource.exe)负责发送产品广告，不需要发送音频也不需要接收任何人的音视频。
* 主进程启动产品广告进程的时候，可以给产品广告进程传送一些初始化参数信息比如appid等

**如何在自己的程序中使用demo的代码**

* 将两个进程通信依赖的公共代码拷贝到自己的代码下，配置好相关路径。
* 将AdvertiseVideoSource整个project添加到自己的代码下
* 将AgoraHQ项目下的将agora_video_source.h和cpp文件拷贝到自己的代码下面。
* 在自己的代码中初始化AgoraVideoSource和CAgoraVideoSourceEventHandler，通过AgoraVideoSource的接口可以和产品广告进程进行通信。比如：设置摄像头id(AgoraVideoSource::setCurrentCamera)等。
产品广告的进程也可以通过CAgoraVideoSourceEventHandler获得一些通知事件。

**两个进程之间的通信可以进行扩展，以主进程通知产品广告进程joinChannel为例：**

* 定义一个消息AGORA\_IPC\_JOIN
* 定义一个通知结构JoinChannelCmd
* 主进程AgoraVideoSource中调用IAgoraIpc::SendMsg发送消息和Cmd。
* 在产品进程的AgoraVideoSource::onMessage中处理消息AGORA\_IPC\_JOIN。

**注意两点：**

* 目前进程处理的命令队列最大为10(VIDEO\_SOURCE\_BLOCK\_NUM)，
* 每个命令的最大值为1024(VIDEO\_SOURCE\_BLOCK\_SIZE)

## sqlite3存储本地数据库

关于sqlite3的具体使用可以参考[sqlite3官网](!http://www.sqlite.org	)
为了方便使用了开源的CPPSqlite3[github地址](!)。
主要在HQDataBase.h和cpp文件中时间。
sqlite3属于纯工具行库，再次不做详述。



## project solution的一点说明

### AdvertiseVideoSource

作为一个独立进程发送一个视频画面，由主进程发送通知来控制，也可以给主进程发送通知反馈。

### AgoraLiveShop

* AdvertiseVideo

    专门负责支持主进程和AdvertiseVideoSource进程通信的一些功能的类。CAgoraHQDlg会用到这些控制主进程和子进程的通信。
* AgoraObject

    对agora sdk的一些封装和扩展，包括IRtcEngine的封装、裸数据接口(ExtendObserverForObsStudio)和IRtcEngineEventHandler的实现，以及token和设备管理的实现。
* Dlg

    MFC对话框的实现。
* LocalQuestionAndAdvertiseData

    本地存储题库和产品广告的实现，发送的产品广告信息和题目信息都由这部分获取。AgoraHQ.db存储在exe运行目录下，需要使用sqlite DB Brower存储数据。
* obs-studio

    obs studio使用的部分。

