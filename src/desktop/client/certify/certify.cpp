﻿/*
* Copyright (c) 2018-2019 the Certify authors
* All rights reserved.
*
* The project sponsor and lead author is Xu Rendong.
* E-mail: xrd@ustc.edu, QQ: 277195007, WeChat: ustc_xrd
* See the contributors file for names of other contributors.
*
* Other copyright notice and conditions to be added...
*/

#include <stdlib.h>
#include <stdint.h>

#include <QtGui/QFont>
#include <QtCore/QStringList>

#include "global/define.h"
#include "project/project.h"

#include "main_window.h"
#include "certify.h"

int32_t main( int32_t argc, char* argv[] ) {
	basicx::SysLog_S syslog_s( THE_APP_NAME ); // 唯一实例 // 01
	basicx::SysLog_S* syslog = basicx::SysLog_S::GetInstance();
	syslog->SetThreadSafe( true );
	syslog->SetLocalCache( true );
	syslog->SetActiveFlush( false );
	syslog->SetActiveSync( false );
	syslog->SetWorkThreads( 1 );
	syslog->SetFileStreamBuffer( DEF_SYSLOG_FSBM_NONE );
	syslog->InitSysLog( THE_APP_NAME, THE_APP_VERSION, THE_APP_COMPANY, THE_APP_COPYRIGHT );
	syslog->WriteSysInfo();

	certify::Project project; // 唯一实例 // 02

	QStringList qt_plugins_path;
	qt_plugins_path.push_back( "./extdlls" );
	QApplication::setLibraryPaths( qt_plugins_path );

	QFont font( "YaHei", 9, QFont::Normal, false ); // SimSun、Times、YaHei
	QApplication::setFont( font );

	QCoreApplication::setAttribute( Qt::AA_EnableHighDpiScaling ); // 高分屏支持，要求 QT 版本 >= 5.6.0 // 在 QApplication 创建之前

	QApplication application( argc, argv );

	certify::MainWindow* main_window = new certify::MainWindow;
	main_window->ReadSettings(); // 读取界面属性
	main_window->show();
	main_window->SystemStart(); // 初始化各模块

	return application.exec();
}

namespace certify {

} // namespace certify
