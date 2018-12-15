/*
* Copyright (c) 2018-2019 the Certify authors
* All rights reserved.
*
* The project sponsor and lead author is Xu Rendong.
* E-mail: xrd@ustc.edu, QQ: 277195007, WeChat: ustc_xrd
* See the contributors file for names of other contributors.
*
* Other copyright notice and conditions to be added...
*/

#ifndef CERTIFY_GLOBAL_DEFINE_H
#define CERTIFY_GLOBAL_DEFINE_H

// 软件信息
#define THE_APP_NAME "Certify" // 系统英文名称
#define THE_APP_NAME_CN "数 字 认 证 客 户 端" // 系统中文名称
#define THE_APP_VERSION "V0.1.0-Beta Build 20181215" // 系统版本号
#define THE_APP_DEVELOPER "Developed by the D.D.F." // 开发者声明
#define THE_APP_COMPANY "Digital Donate Foundation" // 公司声明
#define THE_APP_COPYRIGHT "Copyright 2018-2019 D.D.F. All Rights Reserved." // 版权声明
#define THE_APP_HOMEURL "http://www.digitalcertify.org" // 主页链接

#define TRAY_POP_TITLE "系统消息：" // 托盘气球型提示标题
#define TRAY_POP_START "Certify" // 托盘气球型提示启动

#define REGISTRY_KEY_ORG "D.D.F." // 注册表键值名称 - 组织
#define REGISTRY_KEY_APP "Digital_Certify_Client" // 注册表键值名称 - 应用

// 配置定义
#define CFG_MAX_WORD_LEN 64 // 字串最大字符数
#define CFG_MAX_PATH_LEN 256 // 路径最大字符数

#define NON_WARNING( n ) __pragma( warning( push ) ) __pragma( warning( disable : n ) )
#define USE_WARNING( n ) __pragma( warning( default : n ) ) __pragma( warning( pop ) )

// 网络数据包类型定义
#define NW_MSG_ATOM_TYPE_MIN 0 // 网络通信 元操作 类型起始
#define NW_MSG_TYPE_HEART_CHECK  NW_MSG_ATOM_TYPE_MIN + 0 // 连接心跳检测消息
#define NW_MSG_ATOM_TYPE_MAX 7 // 网络通信 元操作 类型终止

#define NW_MSG_USER_TYPE_MIN 8 // 网络通信 自定义 类型起始
#define NW_MSG_TYPE_USER_DATA    NW_MSG_USER_TYPE_MIN + 0 // 用户数据处理消息
#define NW_MSG_USER_TYPE_MAX 15 // 网络通信 自定义 类型终止

// 网络数据包编码定义
#define NW_MSG_CODE_TYPE_MIN 0 // 网络通信 编码 类型起始
#define NW_MSG_CODE_NONE         NW_MSG_CODE_TYPE_MIN + 0 // 适用于元操作
#define NW_MSG_CODE_STRING       NW_MSG_CODE_TYPE_MIN + 1 // 直接字符串
#define NW_MSG_CODE_JSON         NW_MSG_CODE_TYPE_MIN + 2 // Json格式
#define NW_MSG_CODE_BASE64       NW_MSG_CODE_TYPE_MIN + 3 // Base64格式
#define NW_MSG_CODE_PROTOBUF     NW_MSG_CODE_TYPE_MIN + 4 // ProtoBuf格式
#define NW_MSG_CODE_ZLIB         NW_MSG_CODE_TYPE_MIN + 5 // ZLib格式
#define NW_MSG_CODE_MSGPACK      NW_MSG_CODE_TYPE_MIN + 6 // MsgPack格式
#define NW_MSG_CODE_TYPE_MAX 15 // 网络通信 编码 类型终止

#endif // CERTIFY_GLOBAL_DEFINE_H
