# Agora Live Shop Sample

*其他语言版本： [简体中文](README.zh.md)*

This readme describes how to run the Android Agora Live Shop sample app.

## Introduction

Built upon the Agora Video SDK v2.3.1 with SEI support.

The is an open-source sample that introduces how to use our Android video SDK in an online-shop use case. 

The Android (and also the iOS) sample serves as the custom-side application which allows you to:

- Browse and join a product streaming room
- Send text messages to the room channel
- Watch the product videos
- Receive product information and answer quizes

## Development Environment

- Android Studio 2.0 or later
- Real Android devices, for example Nexus 5X

NOTE: Agora recommends using a real device instead of an emulator.

## Running the App
### 1. Create a developer account at [Agora.io](https://dashboard.agora.io/signin/), and obtain an App ID.

### 2. Fill in your App ID in **app/src/main/res/values/strings.xml**.

```
<string name="agora_app_id"><#YOUR APP ID#></string>
```
### 3. Integrate the Agora Video SDK.

   * Download the Agora Video SDK v2.3.1 with SEI support from [Here](https://github.com/AgoraIO/Live-Shop-Use-Case/releases).
   * Unzip the downloaded SDK package and:

   1) copy ***.jar** under **/libs** to **app/libs**.
     
   2) copy **libs**/**arm64-v8a**/**x86**/**armeabi-v7a** to **app/src/main/jniLibs**.
     
   * Add the following command line to the dependency property of **app**/**build.gradle**. 

       implementation fileTree(dir: 'libs', include: ['*.jar'])

### 6. Open your project using Android Studio.
### 7. Connect your Android device to your computer.
### 8. Build and run the sample app.

   NOTE: You can also use `Gradle` to build and run the sample app.


## Contact Us

- You can find the API documentation at [Developer Center](https://docs.agora.io/en/).
- You can report issues about this demo at [issue](https://github.com/AgoraIO/Live-Shop-Use-Case/issues).

## License
The MIT License (MIT).
