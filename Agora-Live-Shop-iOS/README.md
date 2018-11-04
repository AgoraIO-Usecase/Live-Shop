# Agora Live Shop for iOS 

*其他语言版本： [简体中文](README.zh.md)*

This readme describes the steps and considerations for demonstrating the Agora Live shop iOS sample app.

## Introduction

Built upon the Agora Video SDK, the Agora Live Shop for iOS is an open-source demo that implement e-commerce shopping scene

This sample app allows you to:

- Join / leave channel
- Receive host and product video screens
- Display product information
- Display question information

## Preparing the Developer Environment

* Xcode 9.0 or later
* An iPhone or iPad

NOTE: The iOS emulator is NOT supported.

## Running the App
1. Create a developer account at [Agora.io](https://dashboard.agora.io/signin/), obtain an App ID.
2. Fill in the AppID in the *KeyCenter.swift*.
```
struct KeyCenter {
    static let AppId: String = <#Your App ID#>
}
```
3. Contact the business to get the **Agora Video SDK** which includes **SEI function**.
4. Unzip the downloaded **Agora Video SDK** and copy **libs/AgoraRtcEngineKit.framework** to the "Agora-Live-Shop-iOS" folder of your project.
5. Open AgoraLiveStreaming.xcodeproj, connect your iPhone／iPad device, set up your code signature, and run the sample app.

## Contact Us

- You can find the API documentation at the [Developer Center](https://docs.agora.io/en/).
- You can report issues about this demo at [issue](https://github.com/AgoraIO/Agora-Interactive-Broadcasting-Live-Streaming-iOS/issues).

## License

The MIT License (MIT). 
