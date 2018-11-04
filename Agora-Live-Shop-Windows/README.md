#  Agora Live Shop

*其他语言版本： [简体中文](README.zh.md)*

This demo is written in **C++**

## Third party libs included
* agora sdk 2.3.1 special version(for sei)
* zlib(for generate token)
* obs 20.1(for capture obs video source and audio)
* sqlite3 (for store local data)


## Running the App
First, create a developer account at [Agora.io](https://dashboard.agora.io/signin/), and obtain an App ID. 

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

Finally, Open AgoraLiveShop-Windows.sln with your VC++ 2013(or higher) and build all solution and run.

There is a Restart.bat script in the home directory. You need to copy the script to the execution directory during the actual running.

## Developer Environment Requirements
* VC++ 2013(or higher)
* win7(or higher)


## How to guides
### Using SEI
* This demo is based on 2.3.1 Native SDK special version. You can find the SDK [here](https://github.com/AgoraIO/Live-Shop-Use-Case/releases/download/release%2F1.0/Agora_Native_SDK_for_Windows.x86._v2.3.1_sei_FULL_20181024_1132.1.zip). Note SEI feature is not available in official SDK release.
* This special version SDK has two new APIs available,

  * onSendSEI: put extra data into video SEI frame
  * onReceiveSEI: callback when receiving SEI data
  
  **CAUTION** SEI info currently supports **1024bytes** maximum
* [zlib Download](http://gnuwin32.sourceforge.net/packages/zlib.htm)

### Using libobs
Instructions of obs can be found [here](obsproject.com)

We have provided a new plugin win-agora which is based on open source obs studio. The plugin is used to export mixed obs video and audio data into a shared buffer. The buffer can then be copied to agora sdk to take advantage of advanced features of obs.

Supported obs video sources:
* camera source
* media source(ffmpeg)
* image source

The modified libobs can be found at **obs-sdk** folder, while the headers and lib file can be found at **obs-studio** folder.

Pay attention
* While calling *AgoraObject::EnableExtendAudioCapture* and *AgoraObject::EnableExtendVideooCapture*, pass true parameter to register Agora raw data interface and pass false to unregister
* While registering obs video/audio callback function via *AgoraOutputHandler::SetEncoderCallback*, the eventual obs mixed video/audio data will be provided to agora sdk via registered callback
* In this demo, all obs related code starts with **OBS for ExtCapture** comments

Official raw data documentations can be found [here](https://docs.agora.io/cn/Interactive%20Broadcast/rawdata_windows?platform=Windows)

### Dual camera support
For the moment you can only instantiate one Agora SDK within one process, while in this demo we need to support multiple SDK instance to push multiple streams on one PC. To achieve this,
* We need to start two processes, each uses a uid, while both processes join channel as broadcaster
* The Main process push out the host video/audio, note you need to muteRemoteVideo if not needed
* The Sub process push out the product video, note you don't need to send audio and you should also mute remote videos
* While the main process is up, we need to pass relevant parameters (e.g. appid) to sub processes via ipc

#### Communications between two process can be extended via following way, take joinChannel as an example
* Define a message AGORA\_IPC\_JOIN
* Define a notification structure JoinChannelCmd
* Main process calls IAgoraIpc::SendMsg to send message and cmd to sub process
* In sub process onMessage process AGORA\_IPC\_JOIN

#### ATTENTION
* The process cmd queue is maximum 10
* Each cmd supports 1024 bytes maximum

### sqlite3 to store local db
* For instructions of sqlite3 can be found [here](http://www.sqlite.org)
* For sake of convenience CPPSqlite3 is used
* Mainly used in HQDataBase.h and HQDataBase.cpp

## Connect Us

- You can find full API document at [Document Center](https://docs.agora.io/en/)
- You can fire bugs about this demo at [issue](https://github.com/AgoraIO/OpenLive-Windows/issues)

## License

The MIT License (MIT).
