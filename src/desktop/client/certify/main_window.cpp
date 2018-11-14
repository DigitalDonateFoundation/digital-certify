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

#include <common/sysdef.h>
#include <common/assist.h>
#include <common/common.h>

#include "global/define.h"

#include "main_window.h"

namespace certify {

	MainWindow::MainWindow( QWidget* parent )
	: QMainWindow( parent )
	, m_timer( nullptr )
	, m_user_exit( false )
	, m_not_first_run( false )
	, m_tray_icon_load( false )
	, m_main_window_visible( false )
	, m_tray_icon_menu( nullptr )
	, m_tray_icon_menu_auto_start( nullptr )
	, m_tray_icon( nullptr )
	, m_action_show( nullptr )
	, m_action_hide( nullptr )
	, m_action_exit( nullptr )
	, m_action_about( nullptr )
	, m_action_auto_start( nullptr )
	, m_status_bar( nullptr )
	, m_main_widget( nullptr )
	, m_label_logo( nullptr )
	, m_label_info( nullptr )
	, m_label_time( nullptr )
	, m_about_dialog( nullptr )
	, m_log_cate( "<WINDOW>" ) {
		m_syslog = basicx::SysLog_S::GetInstance();
		CreateActions();
		InitInterface();
	}

	MainWindow::~MainWindow() {
	}

	void MainWindow::CreateActions() {
		m_action_show = new QAction( this );
		m_action_show->setText( QString::fromLocal8Bit( "显示(&S)" ) );
		m_action_show->setToolTip( QString::fromLocal8Bit( "显示主界面" ) );
		//m_action_show->setStatusTip( QString::fromLocal8Bit( "显示主界面" ) );
		m_action_show->setShortcut( QKeySequence( "Ctrl+S" ) );
		m_action_show->setIcon( QIcon( ":/certify/resource/action_show.ico" ) );

		m_action_hide = new QAction( this );
		m_action_hide->setText( QString::fromLocal8Bit( "隐藏(&H)" ) );
		m_action_hide->setToolTip( QString::fromLocal8Bit( "隐藏主界面" ) );
		//m_action_hide->setStatusTip( QString::fromLocal8Bit( "隐藏主界面" ) );
		m_action_hide->setShortcut( QKeySequence( "Ctrl+H" ) );
		m_action_hide->setIcon( QIcon( ":/certify/resource/action_hide.ico" ) );

		m_action_exit = new QAction( this );
		m_action_exit->setText( QString::fromLocal8Bit( "退出(&Q)" ) );
		m_action_exit->setToolTip( QString::fromLocal8Bit( "退出当前系统" ) );
		//m_action_exit->setStatusTip( QString::fromLocal8Bit( "退出当前系统" ) );
		m_action_exit->setShortcut( QKeySequence( "Ctrl+Q" ) );
		m_action_exit->setIcon( QIcon( ":/certify/resource/action_exit.ico" ) );

		m_action_about = new QAction( this );
		m_action_about->setText( QString::fromLocal8Bit( "关于(&A)" ) );
		m_action_about->setToolTip( QString::fromLocal8Bit( "系统相关信息" ) );
		//m_action_about->setStatusTip( QString::fromLocal8Bit( "系统相关信息" ) );
		m_action_about->setShortcut( QKeySequence( "Ctrl+A" ) );
		m_action_about->setIcon( QIcon( ":/certify/resource/action_about.ico" ) );

		m_action_auto_start = new QAction( this );
		m_action_auto_start->setText( QString::fromLocal8Bit( "开机运行" ) );
		m_action_auto_start->setCheckable( true );
	}

	void MainWindow::InitInterface() {
		resize( 1200, 600 );
		std::string title;
		FormatLibrary::StandardLibrary::FormatTo( title, "{0} {1}", THE_APP_NAME, THE_APP_VERSION );
		setWindowTitle( QString::fromLocal8Bit( title.c_str() ) );
		setWindowIcon( QIcon( ":/certify/resource/certify.ico" ) );

		m_timer = new QTimer( this );
		connect( m_timer, SIGNAL( timeout() ), this, SLOT( UpdateTime() ) );
		m_timer->start( 1000 );

		// 必须，否则底部停靠栏高度会无法向下调整
		m_main_widget = new QWidget( this );
		m_main_widget->setFixedWidth( 0 ); // 使用 hide() 无效
		setCentralWidget( m_main_widget );

		m_label_logo = new QLabel( THE_APP_NAME, this );
		m_label_logo->setStyleSheet( "color:rgb(128,0,0);font:bold;" );
		m_label_logo->setAlignment( Qt::AlignHCenter );

		m_label_info = new QLabel( QString::fromLocal8Bit( "就绪" ), this );
		m_label_info->setAlignment( Qt::AlignLeft );

		QDateTime qDateTime = QDateTime::currentDateTime();
		m_label_time = new QLabel( qDateTime.toString( "yyyy-MM-dd dddd hh:mm:ss" ), this );
		m_label_time->setAlignment( Qt::AlignRight );

		m_status_bar = new QStatusBar( this );
		//m_status_bar->setStyleSheet( QString( "QStatusBar::item{border:0px}" ) ); // 消除分隔线
		m_status_bar->insertPermanentWidget( 0, m_label_logo );
		m_status_bar->insertPermanentWidget( 1, m_label_info, 1 );
		m_status_bar->insertPermanentWidget( 2, m_label_time );
		// 无法做到第一个显示标识，第二个显示自动提示，第三个显示时间，自动更新的总是会排第一，先只显示用户自定义信息吧
		setStatusBar( m_status_bar );

		QObject::connect( m_action_show, SIGNAL( triggered() ), this, SLOT( ShowMainWindow() ) );
		QObject::connect( m_action_hide, SIGNAL( triggered() ), this, SLOT( HideMainWindow() ) );
		QObject::connect( m_action_exit, SIGNAL( triggered() ), this, SLOT( OnActionExit() ) );
		QObject::connect( m_action_about, SIGNAL( triggered() ), this, SLOT( OnActionAbout() ) );
		QObject::connect( m_action_auto_start, SIGNAL( triggered( bool ) ), this, SLOT( OnActionAutoStart( bool ) ) );
	}

	void MainWindow::SystemStart() {
		LoadTrayIcon();
		std::string msg;
		FormatLibrary::StandardLibrary::FormatTo( msg, "{0}\n{1}", THE_APP_NAME, THE_APP_VERSION );
		ShowTrayIconMsg( msg );
	}

	void MainWindow::LoadTrayIcon() {
		if( !QSystemTrayIcon::isSystemTrayAvailable() ) {
			std::string log_info = "本系统不支持托盘图标！";
			m_syslog->LogWrite( basicx::syslog_level::c_warn, m_log_cate, log_info );
			QMessageBox::warning( this, QString::fromLocal8Bit( "警告" ), QString::fromLocal8Bit( log_info.c_str() ) );
			return;
		}

		QApplication::setQuitOnLastWindowClosed( false ); // 点击主界面关闭按钮将不会结束程序，需要从托盘菜单退出

		m_tray_icon_menu = new QMenu( this );
		m_tray_icon_menu->addAction( m_action_show );
		m_tray_icon_menu->addAction( m_action_hide );
		m_tray_icon_menu->addSeparator();
		m_tray_icon_menu_auto_start = m_tray_icon_menu->addMenu( QString::fromLocal8Bit( "运行(&R)" ) );
		m_tray_icon_menu_auto_start->setIcon( QIcon( ":/certify/resource/action_auto_start.ico" ) );
		m_tray_icon_menu_auto_start->addAction( m_action_auto_start );
		m_tray_icon_menu->addSeparator();
		m_tray_icon_menu->addAction( m_action_about );
		m_tray_icon_menu->addAction( m_action_exit );

		m_tray_icon = new QSystemTrayIcon( this );
		m_tray_icon->setContextMenu( m_tray_icon_menu );

		m_tray_icon->setToolTip( QString::fromLocal8Bit( TRAY_POP_START ) );
		m_tray_icon->setIcon( QIcon( ":/certify/resource/certify.ico" ) );

		QObject::connect( m_tray_icon, SIGNAL( messageClicked() ), this, SLOT( TrayIconMsgClicked() ) );
		QObject::connect( m_tray_icon, SIGNAL( activated( QSystemTrayIcon::ActivationReason ) ), this, SLOT( TrayIconActivated( QSystemTrayIcon::ActivationReason ) ) );

		m_action_show->setEnabled( false );
		m_action_hide->setEnabled( true );

		m_tray_icon->show(); // 放到 StartDialogThread() 中托盘会崩溃

		m_tray_icon_load = true;
	}

	void MainWindow::ShowTrayIconMsg( std::string msg ) {
		if( true == m_tray_icon_load ) {
			m_tray_icon->showMessage( QString::fromLocal8Bit( TRAY_POP_TITLE ), QString::fromLocal8Bit( msg.c_str() ) );
		}
	}

	void MainWindow::UpdateTime() {
		QDateTime date_time = QDateTime::currentDateTime();
		m_label_time->setText( date_time.toString( "yyyy-MM-dd dddd hh:mm:ss" ) );
	}

	void MainWindow::OnActionExit() {
		if( QMessageBox::Yes == QMessageBox::question( this, QString::fromLocal8Bit( "询问" ), QString::fromLocal8Bit( "确认退出系统？" ) ) ) {
			m_user_exit = true;
//			WriteSettings(); // 保存界面属性
//			CloseFloatDockWidget(); // 否则有浮动停靠栏或工具栏时可能导致崩溃
			QApplication::setQuitOnLastWindowClosed( true );
			m_tray_icon->hide();
			close();
			//QApplication::quit();
		}
	}

	void MainWindow::OnActionAbout() {
		if( nullptr == m_about_dialog ) {
			m_about_dialog = new AboutDialog( this );
		}
		m_about_dialog->show();
	}

	void MainWindow::ShowMainWindow() {
		show();
		m_main_window_visible = true;
		m_action_show->setEnabled( false );
		m_action_hide->setEnabled( true );
	}

	void MainWindow::HideMainWindow() {
		hide();
		m_main_window_visible = false;
		m_action_show->setEnabled( true );
		m_action_hide->setEnabled( false );
	}

	void MainWindow::TrayIconMsgClicked() {
		QMessageBox::information( this, QString::fromLocal8Bit( "提示" ), QString::fromLocal8Bit( "小兔子乖乖，把门儿开开。" ) );
	}

	void MainWindow::TrayIconActivated( QSystemTrayIcon::ActivationReason reason ) {
		switch( reason ) {
		case QSystemTrayIcon::Trigger:
			break;
		case QSystemTrayIcon::DoubleClick: {
			if( true == m_main_window_visible ) {
				HideMainWindow();
			}
			else {
				ShowMainWindow();
			}
		}
		break;
		case QSystemTrayIcon::MiddleClick:
			TrayIconMsgClicked();
			break;
		default:
			break;
		}
	}

	// 如果有导入注册表之类的系统提示，可能需要 工程->属性->链接器->清单文件->UAC执行级别->requireAdministrator 以获取管理员权限
	void MainWindow::OnActionAutoStart( bool auto_start ) {
#ifdef __OS_WINDOWS__
		wchar_t char_path[MAX_PATH] = { 0 };
		GetModuleFileName( NULL, char_path, MAX_PATH );
		std::string app_exec_path = basicx::StringToAnsiChar( char_path );
		std::string reg_key = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";
		QSettings* settings = new QSettings( reg_key.c_str(), QSettings::NativeFormat );
		if( true == auto_start ) {
			settings->setValue( QString::fromLocal8Bit( THE_APP_NAME ), QString::fromLocal8Bit( app_exec_path.c_str() ) );
			std::string log_info = "用户 启用 系统开机运行。";
			m_syslog->LogWrite( basicx::syslog_level::c_info, m_log_cate, log_info );
			QMessageBox::information( this, QString::fromLocal8Bit( "提示" ), QString::fromLocal8Bit( "已设置系统开机运行。" ) );
		}
		else {
			settings->remove( QString::fromLocal8Bit( THE_APP_NAME ) );
			std::string log_info = "用户 取消 系统开机运行。";
			m_syslog->LogWrite( basicx::syslog_level::c_info, m_log_cate, log_info );
			QMessageBox::information( this, QString::fromLocal8Bit( "提示" ), QString::fromLocal8Bit( "已取消系统开机运行。" ) );
		}
		delete settings;
#endif
	}

	void MainWindow::closeEvent( QCloseEvent* event ) {
		if( false == m_user_exit ) {
			//QMessageBox::information( this, QString::fromLocal8Bit( "提示" ), QString::fromLocal8Bit( "程序将继续运行，如需退出，请右击托盘图标。" ) );
			HideMainWindow();
			event->ignore();
		}
	}

	bool MainWindow::eventFilter( QObject* target, QEvent* event ) {
		if( event->type() == QEvent::Show ) {
		}
		if( event->type() == QEvent::Close ) {
		}
		return QMainWindow::eventFilter( target, event );
	}

} // namespace certify
