# Agora Live Shop 样本代码

*English Version： [English](README.md)*

此篇Readme文档描述如何运行 Android Agora Live Shop 样本代码

## 简介

基于带有SEI支持的Agora SDK v2.3.1。

本Android app 样本代码用来介绍如何在电商的场景下使用Agora Video SDK。

Android（以及iOS）平台的App作为电商品台的客户端，可以提供如下功能：

- 浏览、加入产品直播房间
- 在房间频道发送文本消息
- 观看产品展示视频
- 接收产品信息、回答小测试题

## 开发环境配置

- Android Studio 2.0 及后续版本
- Android设备, 例如Nexus 5X

注意：Agora推荐您使用实体Android设备，不保证在Android模拟器上正常运行

## 运行 App
### 1. 在[Agora.io](https://dashboard.agora.io/signin/)官网注册一个开发者用户, 并获取一份 App ID.

### 2. 替换您的 App ID 到 **app/src/main/res/values/strings.xml** 如下：

```
<string name="agora_app_id"><#YOUR APP ID#></string>
```
### 3. 集成 Agora Video SDK.
 
   * 从[这里](https://github.com/AgoraIO/Live-Shop-Use-Case/releases)下载带有SEI支持的 Agora Video SDK v2.3.1.
   * 解压下载到的 SDK package，并且:

   1) 拷贝 **libs**文件夹下的 ***.jar** 文件到 **app/libs**
   2) 拷贝 **libs**文件夹下的 **arm64-v8a** 、 **x86** 、 **armeabi-v7a** 三个文件夹到 **app/src/main/jniLibs/**.
   
   * 确保 **app**/**build.gradle** 文件中已经添加了以下依赖. 

    implementation fileTree(dir: 'libs', include: ['*.jar'])

### 6. 用Android Studio打开项目.
### 7. 连接您的Android到电脑.
### 8. 构建并运行您的应用.

   注：您也可以使用 `Gradle` 来构建和运行代码。 
 

## 联系我们

- 请到 [开发者中心](https://docs.agora.io/en/) 查看文档.
- 若有疑问请到 [issue](https://github.com/AgoraIO/Live-Shop-Use-Case/issues) 参与讨论.

## 开源许可协议
MIT License (MIT).
