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

#include "packer/tester.h"

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
	, m_action_new_project( nullptr )
	, m_action_exit( nullptr )
	, m_action_about( nullptr )
	, m_action_tests( nullptr )
	, m_action_show_tool_bar_main( nullptr )
	, m_action_save_layout( nullptr )
	, m_action_auto_start( nullptr )
	, m_action_show_dock_project( nullptr )
	, m_action_show_dock_1( nullptr )
	, m_action_show_dock_2( nullptr )
	, m_action_show_dock_3( nullptr )
	, m_menu_bar( nullptr )
	, m_menu_file( nullptr )
	, m_menu_tool( nullptr )
	, m_menu_view( nullptr )
	, m_menu_view_tooler( nullptr )
	, m_menu_view_docker( nullptr )
	, m_menu_help( nullptr )
	, m_main_tool_bar( nullptr )
	, m_status_bar( nullptr )
	, m_mdi_area( nullptr )
	, m_label_logo( nullptr )
	, m_label_info( nullptr )
	, m_label_time( nullptr )
	, m_dock_widget_project( nullptr )
	, m_dock_widget_1( nullptr )
	, m_dock_widget_2( nullptr )
	, m_dock_widget_3( nullptr )
	, m_text_edit_1( nullptr )
	, m_text_edit_2( nullptr )
	, m_text_edit_3( nullptr )
	, m_about_dialog( nullptr )
	, m_infos_dialog( nullptr )
	, m_project_list_dialog( nullptr )
	, m_log_cate( "<WINDOW>" ) {
		m_syslog = basicx::SysLog_S::GetInstance();
		CreateActions();
		InitInterface();
	}

	MainWindow::~MainWindow() {
	}

	void MainWindow::CreateActions() {
		m_action_new_project = new QAction( this );
		m_action_new_project->setText( QString::fromLocal8Bit( "新建(&N)" ) );
		m_action_new_project->setToolTip( QString::fromLocal8Bit( "创建新的项目" ) );
		//m_action_new_project->setStatusTip( QString::fromLocal8Bit( "创建新的项目" ) );
		m_action_new_project->setShortcut( QKeySequence( "Ctrl+N" ) );
		m_action_new_project->setIcon( QIcon( ":/certify/resource/action_new_project.ico" ) );

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

		m_action_tests = new QAction( this );
		m_action_tests->setText( QString::fromLocal8Bit( "测试(&T)" ) );
		m_action_tests->setToolTip( QString::fromLocal8Bit( "测试相关内容" ) );
		//m_action_tests->setStatusTip( QString::fromLocal8Bit( "测试相关内容" ) );
		m_action_tests->setShortcut( QKeySequence( "Ctrl+T" ) );
		m_action_tests->setIcon( QIcon( ":/certify/resource/action_tests.ico" ) );

		m_action_show_tool_bar_main = new QAction( this );
		m_action_show_tool_bar_main->setText( QString::fromLocal8Bit( "显示系统工具栏" ) );
		m_action_show_tool_bar_main->setCheckable( true );

		m_action_save_layout = new QAction( this );
		m_action_save_layout->setText( QString::fromLocal8Bit( "保存布局" ) );
		m_action_save_layout->setCheckable( true );

		m_action_auto_start = new QAction( this );
		m_action_auto_start->setText( QString::fromLocal8Bit( "开机运行" ) );
		m_action_auto_start->setCheckable( true );

		m_action_show_dock_project = new QAction( this );
		m_action_show_dock_project->setText( QString::fromLocal8Bit( "项目列表" ) );
		m_action_show_dock_project->setCheckable( true );

		m_action_show_dock_1 = new QAction( this );
		m_action_show_dock_1->setText( QString::fromLocal8Bit( "窗口-01" ) );
		m_action_show_dock_1->setCheckable( true );

		m_action_show_dock_2 = new QAction( this );
		m_action_show_dock_2->setText( QString::fromLocal8Bit( "窗口-02" ) );
		m_action_show_dock_2->setCheckable( true );

		m_action_show_dock_3 = new QAction( this );
		m_action_show_dock_3->setText( QString::fromLocal8Bit( "窗口-03" ) );
		m_action_show_dock_3->setCheckable( true );
	}

	void MainWindow::InitInterface() {
		resize( 1200, 600 );
		std::string title;
		FormatLibrary::StandardLibrary::FormatTo( title, "{0} {1}", THE_APP_NAME, THE_APP_VERSION );
		setWindowTitle( QString::fromLocal8Bit( title.c_str() ) );
		setWindowIcon( QIcon( ":/certify/resource/certify.ico" ) );

		m_timer = new QTimer( this );
		QObject::connect( m_timer, SIGNAL( timeout() ), this, SLOT( UpdateTime() ) );
		m_timer->start( 1000 );

		m_menu_bar = new QMenuBar( this );
		m_menu_bar->setStyle( QStyleFactory::create( "windowsxp" ) ); // 扁平的
		m_menu_bar->setGeometry( QRect( 0, 0, 800, 21 ) );
		setMenuBar( m_menu_bar );

		m_menu_file = new QMenu( m_menu_bar );
		m_menu_file->addAction( m_action_new_project );
		m_menu_file->addAction( m_action_exit );
		m_menu_file->setTitle( QString::fromLocal8Bit( "文件(&F)" ) );
		m_menu_bar->addAction( m_menu_file->menuAction() );

		m_menu_tool = new QMenu( m_menu_bar );
		m_menu_tool->setTitle( QString::fromLocal8Bit( "工具(&T)" ) );
		m_menu_bar->addAction( m_menu_tool->menuAction() );

		QMainWindow::setCorner( Qt::TopLeftCorner, Qt::LeftDockWidgetArea ); // 默认顶部收缩
		QMainWindow::setCorner( Qt::TopRightCorner, Qt::RightDockWidgetArea ); // 默认顶部收缩

		QMainWindow::setCorner( Qt::BottomRightCorner, Qt::BottomDockWidgetArea ); // 默认底部扩展
		QMainWindow::setCorner( Qt::BottomLeftCorner, Qt::BottomDockWidgetArea ); // 默认底部扩展

		m_menu_view = new QMenu( m_menu_bar );
		m_menu_view_tooler = m_menu_view->addMenu( QString::fromLocal8Bit( "工具条(&T)" ) );
		m_menu_view_tooler->setIcon( QIcon( ":/certify/resource/action_show_tool.ico" ) );
		m_menu_view_tooler->addAction( m_action_show_tool_bar_main );
		m_menu_view_docker = m_menu_view->addMenu( QString::fromLocal8Bit( "停靠栏(&D)" ) );
		m_menu_view_docker->setIcon( QIcon( ":/certify/resource/action_show_dock.ico" ) );
		m_menu_view_docker->addAction( m_action_show_dock_project );
		m_menu_view_docker->addAction( m_action_show_dock_1 );
		m_menu_view_docker->addAction( m_action_show_dock_2 );
		m_menu_view_docker->addAction( m_action_show_dock_3 );
		m_menu_view->addAction( m_action_save_layout );
		m_menu_view->addAction( m_action_auto_start );
		m_menu_view->setTitle( QString::fromLocal8Bit( "视图(&V)" ) );
		m_menu_bar->addAction( m_menu_view->menuAction() );

		m_menu_help = new QMenu( m_menu_bar );
		m_menu_help->addAction( m_action_about );
		m_menu_help->setTitle( QString::fromLocal8Bit( "帮助(&H)" ) );
		m_menu_bar->addAction( m_menu_help->menuAction() );

		m_main_tool_bar = new QToolBar( this );
		m_main_tool_bar->setIconSize( QSize( 16, 16 ) );
		m_main_tool_bar->setObjectName( "MainToolBar" ); //
		m_main_tool_bar->setToolTip( QString::fromLocal8Bit( "系统工具栏" ) );
		m_main_tool_bar->setWindowTitle( QString::fromLocal8Bit( "系统工具栏" ) );
		m_main_tool_bar->addAction( m_action_about );
		m_main_tool_bar->addAction( m_action_tests );
		m_main_tool_bar->installEventFilter( this ); // 监视其关闭事件
		addToolBar( Qt::TopToolBarArea, m_main_tool_bar );

		m_mdi_area = new QMdiArea( this );
		m_mdi_area->setActivationOrder( QMdiArea::CreationOrder ); // 设置激活顺序 CreationOrder、StackingOrder、ActivationHistoryOrder
		//m_mdi_area->setBackground( QColor( 192, 192, 192 ) ); // 设置背景，默认灰色
		//m_mdi_area->setDocumentMode( false ); // 设置文档模式，在 macOS 中 tabs 会类似 Safari 的 Terminal
		//m_mdi_area->setOption( QMdiArea::DontMaximizeSubWindowOnActivation, false ); // 只有一个选项，即创建子窗口时不充满整个区域，默认 false 充满
		m_mdi_area->setViewMode( QMdiArea::TabbedView ); // 视图模式 SubWindowView、TabbedView
		m_mdi_area->setTabPosition( QTabWidget::North ); // 标签页位置 North、South、West、East
		m_mdi_area->setTabShape( QTabWidget::Rounded ); // 标签页样式 Rounded、Triangular
		m_mdi_area->setTabsClosable( true ); // 是否可关闭
		m_mdi_area->setTabsMovable( true ); // 是否可移动
		m_mdi_area->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded );
		m_mdi_area->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
		// 在 QMdiArea 的 ViewMode 设为 TabbedView 后才会存在 QTabBar
		QList<QTabBar*> list_tab_bar = m_mdi_area->findChildren<QTabBar*>();
		for( QList<QTabBar*>::Iterator it_tb = list_tab_bar.begin(); it_tb != list_tab_bar.end(); ++it_tb ) {
			(*it_tb)->setExpanding( false ); // 向左靠拢，不填满整个标签页
		}
		setCentralWidget( m_mdi_area );

		m_label_logo = new QLabel( THE_APP_NAME, this );
		m_label_logo->setStyleSheet( "color:rgb(128,0,0);font:bold;" );
		m_label_logo->setAlignment( Qt::AlignHCenter );

		m_label_info = new QLabel( QString::fromLocal8Bit( "就绪" ), this );
		m_label_info->setAlignment( Qt::AlignLeft );

		QDateTime qDateTime = QDateTime::currentDateTime();
		m_label_time = new QLabel( qDateTime.toString( "yyyy-MM-dd hh:mm:ss" ), this ); // "yyyy-MM-dd dddd hh:mm:ss"
		m_label_time->setAlignment( Qt::AlignRight );

		m_status_bar = new QStatusBar( this );
		//m_status_bar->setStyleSheet( QString( "QStatusBar::item{border:0px}" ) ); // 消除分隔线
		m_status_bar->insertPermanentWidget( 0, m_label_logo );
		m_status_bar->insertPermanentWidget( 1, m_label_info, 1 );
		m_status_bar->insertPermanentWidget( 2, m_label_time );
		// 无法做到第一个显示标识，第二个显示自动提示，第三个显示时间，自动更新的总是会排第一，先只显示用户自定义信息吧
		setStatusBar( m_status_bar );

		QMainWindow::setDockOptions( AnimatedDocks | AllowNestedDocks | AllowTabbedDocks );

		m_project_list_dialog = new ProjectListDialog( this );
		m_text_edit_1 = new QTextEdit( this );
		m_text_edit_2 = new QTextEdit( this );
		m_text_edit_3 = new QTextEdit( this );

		m_dock_widget_project = new QDockWidget( this );
		m_dock_widget_project->setObjectName( "DockWidget_Project" ); //
		m_dock_widget_project->setContentsMargins( -1, -1, -1, -1 );
		m_dock_widget_project->setWindowTitle( QString::fromLocal8Bit( "项目列表" ) );
		m_dock_widget_project->setWidget( m_project_list_dialog );
		m_dock_widget_project->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
		m_dock_widget_project->installEventFilter( this ); // 监视其关闭事件
		addDockWidget( Qt::LeftDockWidgetArea, m_dock_widget_project );
		m_vec_dock_widget.push_back( m_dock_widget_project );

		m_dock_widget_1 = new QDockWidget( this );
		m_dock_widget_1->setObjectName( "DockWidget_1" ); //
		m_dock_widget_1->setContentsMargins( -1, -1, -1, -1 );
		m_dock_widget_1->setWindowTitle( QString::fromLocal8Bit( "窗口-01" ) );
		m_dock_widget_1->setWidget( m_text_edit_1 );
		m_dock_widget_1->setAllowedAreas( Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea | Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
		m_dock_widget_1->installEventFilter( this ); // 监视其关闭事件
		addDockWidget( Qt::RightDockWidgetArea, m_dock_widget_1 );
		m_vec_dock_widget.push_back( m_dock_widget_1 );

		m_dock_widget_2 = new QDockWidget( this );
		m_dock_widget_2->setObjectName( "DockWidget_2" ); //
		m_dock_widget_2->setContentsMargins( -1, -1, -1, -1 );
		m_dock_widget_2->setWindowTitle( QString::fromLocal8Bit( "窗口-02" ) );
		m_dock_widget_2->setWidget( m_text_edit_2 );
		m_dock_widget_2->setAllowedAreas( Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea | Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
		m_dock_widget_2->installEventFilter( this ); // 监视其关闭事件
		addDockWidget( Qt::BottomDockWidgetArea, m_dock_widget_2 );
		m_vec_dock_widget.push_back( m_dock_widget_2 );

		m_dock_widget_3 = new QDockWidget( this );
		m_dock_widget_3->setObjectName( "DockWidget_3" ); //
		m_dock_widget_3->setContentsMargins( -1, -1, -1, -1 );
		m_dock_widget_3->setWindowTitle( QString::fromLocal8Bit( "窗口-03" ) );
		m_dock_widget_3->setWidget( m_text_edit_3 );
		m_dock_widget_3->setAllowedAreas( Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea | Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
		m_dock_widget_3->installEventFilter( this ); // 监视其关闭事件
		addDockWidget( Qt::BottomDockWidgetArea, m_dock_widget_3 );
		m_vec_dock_widget.push_back( m_dock_widget_3 );

		tabifyDockWidget( m_dock_widget_2, m_dock_widget_3 );

		m_action_show_tool_bar_main->setChecked( true );
		m_action_show_dock_project->setChecked( true );
		m_action_show_dock_1->setChecked( true );
		m_action_show_dock_2->setChecked( true );
		m_action_show_dock_3->setChecked( true );
		m_action_save_layout->setChecked( true );
		m_action_auto_start->setChecked( false );

		installEventFilter( this ); // 监视自身事件

		QObject::connect( m_action_new_project, SIGNAL( triggered() ), this, SLOT( OnActionNewProject() ) );
		QObject::connect( m_action_exit, SIGNAL( triggered() ), this, SLOT( OnActionExit() ) );
		QObject::connect( m_action_about, SIGNAL( triggered() ), this, SLOT( OnActionAbout() ) );
		QObject::connect( m_action_tests, SIGNAL( triggered() ), this, SLOT( OnActionTests() ) );
		QObject::connect( m_action_show_tool_bar_main, SIGNAL( triggered( bool ) ), this, SLOT( ShowToolBar_Main( bool ) ) );
		QObject::connect( m_action_show_dock_project, SIGNAL( triggered( bool ) ), this, SLOT( ShowDockWidget_Project( bool ) ) );
		QObject::connect( m_action_show_dock_1, SIGNAL( triggered( bool ) ), this, SLOT( ShowDockWidget_1( bool ) ) );
		QObject::connect( m_action_show_dock_2, SIGNAL( triggered( bool ) ), this, SLOT( ShowDockWidget_2( bool ) ) );
		QObject::connect( m_action_show_dock_3, SIGNAL( triggered( bool ) ), this, SLOT( ShowDockWidget_3( bool ) ) );
		QObject::connect( m_action_save_layout, SIGNAL( triggered( bool ) ), this, SLOT( OnActionSaveLayout( bool ) ) );
		QObject::connect( m_action_auto_start, SIGNAL( triggered( bool ) ), this, SLOT( OnActionAutoStart( bool ) ) );
		QObject::connect( m_mdi_area, SIGNAL( subWindowActivated( QMdiSubWindow* ) ), this, SLOT( OnSelectProjectLabel( QMdiSubWindow* ) ) );

		m_project_list_dialog->LoadExistProject();
	}

	// QDockWidget 和 QToolBar 都需设置对象名，这样才能 saveState() 和 restoreState() 状态、位置、大小等
	void MainWindow::ReadSettings() {
		QSettings settings( REGISTRY_KEY_ORG, REGISTRY_KEY_APP ); // 位于：HKEY_CURRENT_USER -> Software -> REGISTRY_KEY_ORG

		m_not_first_run = settings.value( "NotFirstRunFlag" ).toBool();
		if( false == m_not_first_run ) { // 首次运行，无保存值，为否，且菜单项“保存布局”为是
			WriteSettings(); // 此时保存的是窗口初始化时的布局
		}
		else { // 读取上次退出时保存的布局
			settings.beginGroup( "MainWindow" );

			restoreState( settings.value( "MainWindowLayout" ).toByteArray() );
			restoreGeometry( settings.value( "MainWindowGeometry" ).toByteArray() );

			m_mdi_area->restoreGeometry( settings.value( "MainWidgetGeometry" ).toByteArray() );

			m_action_show_tool_bar_main->setChecked( settings.value( "ActionShowTool_Main" ).toBool() );
			m_action_show_dock_project->setChecked( settings.value( "ActionShowDock_Project" ).toBool() );
			m_action_show_dock_1->setChecked( settings.value( "ActionShowDock_1" ).toBool() );
			m_action_show_dock_2->setChecked( settings.value( "ActionShowDock_2" ).toBool() );
			m_action_show_dock_3->setChecked( settings.value( "ActionShowDock_3" ).toBool() );
			
			m_action_save_layout->setChecked( settings.value( "ActionSaveLayout" ).toBool() );
			m_action_auto_start->setChecked( settings.value( "ActionAutoStart" ).toBool() );

			settings.endGroup();

			// 添加其他设置项读取
		}
	}

	// QDockWidget 和 QToolBar 都需设置对象名，这样才能 saveState() 和 restoreState() 状态、位置、大小等
	void MainWindow::WriteSettings() {
		QSettings settings( REGISTRY_KEY_ORG, REGISTRY_KEY_APP ); // 位于：HKEY_CURRENT_USER -> Software -> REGISTRY_KEY_ORG

		if( m_action_save_layout->isChecked() ) {
			settings.setValue( "NotFirstRunFlag", true ); // 始终为真

			settings.beginGroup( "MainWindow" );

			settings.setValue( "MainWindowLayout", saveState() );
			settings.setValue( "MainWindowGeometry", saveGeometry() );

			settings.setValue( "MainWidgetGeometry", m_mdi_area->saveGeometry() );

			settings.setValue( "ActionShowTool_Main", m_action_show_tool_bar_main->isChecked() );
			settings.setValue( "ActionShowDock_Project", m_action_show_dock_project->isChecked() );
			settings.setValue( "ActionShowDock_1", m_action_show_dock_1->isChecked() );
			settings.setValue( "ActionShowDock_2", m_action_show_dock_2->isChecked() );
			settings.setValue( "ActionShowDock_3", m_action_show_dock_3->isChecked() );
			
			settings.setValue( "ActionSaveLayout", m_action_save_layout->isChecked() );
			settings.setValue( "ActionAutoStart", m_action_auto_start->isChecked() );

			settings.endGroup();

			// 添加其他设置项保存
		}
		else {
			settings.setValue( "NotFirstRunFlag", true ); // 始终为真

			settings.beginGroup( "MainWindow" );

			settings.setValue( "MainWindowLayout", settings.value( "MainWindowLayout" ).toByteArray() );
			settings.setValue( "MainWindowGeometry", settings.value( "MainWindowGeometry" ).toByteArray() );

			settings.setValue( "MainWidgetGeometry", settings.value( "MainWidgetGeometry" ).toByteArray() );

			settings.setValue( "ActionShowTool_Main", settings.value( "ActionShowTool_Main" ).toBool() );
			settings.setValue( "ActionShowDock_Project", settings.value( "ActionShowDock_Project" ).toBool() );
			settings.setValue( "ActionShowDock_1", settings.value( "ActionShowDock_1" ).toBool() );
			settings.setValue( "ActionShowDock_2", settings.value( "ActionShowDock_2" ).toBool() );
			settings.setValue( "ActionShowDock_3", settings.value( "ActionShowDock_3" ).toBool() );
			
			settings.setValue( "ActionSaveLayout", m_action_save_layout->isChecked() ); // 只有这个保存现值，其他均保存原值
			settings.setValue( "ActionAutoStart", settings.value( "ActionAutoStart" ).toBool() );

			settings.endGroup();

			// 添加其他设置项保存
		}
	}

	void MainWindow::RemoveSettings( std::string key ) {
		QSettings settings( REGISTRY_KEY_ORG, REGISTRY_KEY_APP );
		settings.remove( key.c_str() );
	}

	void MainWindow::CloseFloatDockWidget() {
		size_t dock_widget_number = m_vec_dock_widget.size();
		for( size_t i = 0; i < dock_widget_number; ++i ) {
			if( m_vec_dock_widget[i]->isFloating() ) {
				m_vec_dock_widget[i]->close();
			}
		}
		if( m_main_tool_bar->isFloating() ) {
			m_main_tool_bar->close();
		}
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
		m_tray_icon_menu->addAction( m_action_about );
		m_tray_icon_menu->addSeparator();
		m_tray_icon_menu->addAction( m_action_exit );

		m_tray_icon = new QSystemTrayIcon( this );
		m_tray_icon->setContextMenu( m_tray_icon_menu );

		m_tray_icon->setToolTip( QString::fromLocal8Bit( TRAY_POP_START ) );
		m_tray_icon->setIcon( QIcon( ":/certify/resource/certify.ico" ) );

		QObject::connect( m_tray_icon, SIGNAL( messageClicked() ), this, SLOT( TrayIconMsgClicked() ) );
		QObject::connect( m_tray_icon, SIGNAL( activated( QSystemTrayIcon::ActivationReason ) ), this, SLOT( TrayIconActivated( QSystemTrayIcon::ActivationReason ) ) );

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
		m_label_time->setText( date_time.toString( "yyyy-MM-dd hh:mm:ss" ) ); // "yyyy-MM-dd dddd hh:mm:ss"
	}

	void MainWindow::OnActionNewProject() {
		m_project_list_dialog->OnActionNewProject();
	}

	void MainWindow::OnActionExit() {
		if( QMessageBox::Yes == QMessageBox::question( this, QString::fromLocal8Bit( "询问" ), QString::fromLocal8Bit( "确认退出系统？" ) ) ) {
			m_user_exit = true;
			WriteSettings(); // 保存界面属性
			m_mdi_area->closeAllSubWindows();
			CloseFloatDockWidget(); // 否则有浮动停靠栏或工具栏时可能导致崩溃
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

	void MainWindow::OnActionTests() {
		std::string info_text = "啦啦啦，啦啦啦。";
		if( nullptr == m_infos_dialog ) {
			m_infos_dialog = new InfosDialog( this );
		}
		m_infos_dialog->SetDialogInfos( info_text );
		m_infos_dialog->ShowMessage();

		//Test_SysDBI_B();
	}

	void MainWindow::ShowMainWindow() {
		show();
		m_main_window_visible = true;
	}

	void MainWindow::HideMainWindow() {
		hide();
		m_main_window_visible = false;
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

	void MainWindow::ShowToolBar_Main( bool show ) {
		if( true == show ) {
			m_main_tool_bar->show();
		}
		else {
			m_main_tool_bar->hide();
		}
	}

	void MainWindow::ShowDockWidget_Project( bool show ) {
		if( true == show ) {
			m_dock_widget_project->show();
		}
		else {
			m_dock_widget_project->hide();
		}
	}

	void MainWindow::ShowDockWidget_1( bool show ) {
		if( true == show ) {
			m_dock_widget_1->show();
		}
		else {
			m_dock_widget_1->hide();
		}
	}

	void MainWindow::ShowDockWidget_2( bool show ) {
		if( true == show ) {
			m_dock_widget_2->show();
		}
		else {
			m_dock_widget_2->hide();
		}
	}

	void MainWindow::ShowDockWidget_3( bool show ) {
		if( true == show ) {
			m_dock_widget_3->show();
		}
		else {
			m_dock_widget_3->hide();
		}
	}

	void MainWindow::OnActionSaveLayout( bool save ) {
		if( true == save )
		{
			std::string log_info = "用户 取消 窗口布局保存。";
			m_syslog->LogWrite( basicx::syslog_level::c_info, m_log_cate, log_info );
			QMessageBox::information( this, QString::fromLocal8Bit( "提示" ), QString::fromLocal8Bit( "在系统退出时将 保存 窗口布局。" ) );
		}
		else
		{
			std::string log_info = "用户 启用 窗口布局保存。";
			m_syslog->LogWrite( basicx::syslog_level::c_info, m_log_cate, log_info );
			QMessageBox::information( this, QString::fromLocal8Bit( "提示" ), QString::fromLocal8Bit( "在系统退出时将 忽略 窗口布局。" ) );
		}
	}

	// 如果有导入注册表之类的系统提示，可能需要 工程->属性->链接器->清单文件->UAC执行级别->requireAdministrator 以获取管理员权限
	void MainWindow::OnActionAutoStart( bool start ) {
#ifdef __OS_WINDOWS__
		wchar_t char_path[MAX_PATH] = { 0 };
		GetModuleFileName( NULL, char_path, MAX_PATH );
		std::string app_exec_path = basicx::StringToAnsiChar( char_path );
		std::string reg_key = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";
		QSettings* settings = new QSettings( reg_key.c_str(), QSettings::NativeFormat );
		if( true == start ) {
			settings->setValue( QString::fromLocal8Bit( REGISTRY_KEY_APP ), QString::fromLocal8Bit( app_exec_path.c_str() ) );
			std::string log_info = "用户 启用 系统开机运行。";
			m_syslog->LogWrite( basicx::syslog_level::c_info, m_log_cate, log_info );
			QMessageBox::information( this, QString::fromLocal8Bit( "提示" ), QString::fromLocal8Bit( "已设置系统开机运行。" ) );
		}
		else {
			settings->remove( QString::fromLocal8Bit( REGISTRY_KEY_APP ) );
			std::string log_info = "用户 取消 系统开机运行。";
			m_syslog->LogWrite( basicx::syslog_level::c_info, m_log_cate, log_info );
			QMessageBox::information( this, QString::fromLocal8Bit( "提示" ), QString::fromLocal8Bit( "已取消系统开机运行。" ) );
		}
		delete settings;
#endif
	}

	void MainWindow::OnSelectProjectLabel( QMdiSubWindow* mdi_sub_window ) {
		if( mdi_sub_window != nullptr ) {
			ProjectDialog* project_dialog = qobject_cast<ProjectDialog*>( mdi_sub_window->widget() );
			m_project_list_dialog->SetCurrentProject( project_dialog->m_project_gcid );
		}
		else { // 可能已被关闭删除
			m_project_list_dialog->SetCurrentProject( "" ); // 全不选
		}
	}

	void MainWindow::closeEvent( QCloseEvent* event ) {
		if( false == m_user_exit ) {
			//QMessageBox::information( this, QString::fromLocal8Bit( "提示" ), QString::fromLocal8Bit( "程序将继续运行，如需退出，请右击托盘图标。" ) );
			HideMainWindow();
			event->ignore();
		}
	}

	bool MainWindow::eventFilter( QObject* target, QEvent* event ) {
		if( event->type() == ProjectListItemClickedEvent::m_type ) { // 单击，已打开的选中，未打开的不管
			ProjectListItemClickedEvent* event_item = dynamic_cast<ProjectListItemClickedEvent*>( event );
			std::string project_gcid = event_item->m_gcid;
			QList<QMdiSubWindow*> list_mdi_window = m_mdi_area->subWindowList();
			for( int32_t i = 0; i < list_mdi_window.size(); ++i ) {
				QMdiSubWindow* mdi_sub_window = list_mdi_window.at( i );
				ProjectDialog* project_dialog = qobject_cast<ProjectDialog*>( mdi_sub_window->widget() );
				if( project_dialog->m_project_gcid == project_gcid ) {
					m_mdi_area->setActiveSubWindow( mdi_sub_window );
					break;
				}
			}
		}
		else if( event->type() == ProjectListItemDoubleClickedEvent::m_type ) { // 双击，已打开的选中，未打开的创建
			ProjectListItemDoubleClickedEvent* event_item = dynamic_cast<ProjectListItemDoubleClickedEvent*>( event );
			std::string project_gcid = event_item->m_gcid;
			bool is_mdi_sub_window_exist = false;
			QList<QMdiSubWindow*> list_mdi_window = m_mdi_area->subWindowList();
			for( int32_t i = 0; i < list_mdi_window.size(); ++i ) {
				QMdiSubWindow* mdi_sub_window = list_mdi_window.at( i );
				ProjectDialog* project_dialog = qobject_cast<ProjectDialog*>( mdi_sub_window->widget() );
				if( project_dialog->m_project_gcid == project_gcid ) {
					is_mdi_sub_window_exist = true;
					m_mdi_area->setActiveSubWindow( mdi_sub_window );
					break;
				}
			}
			if( false == is_mdi_sub_window_exist ) {
				ProjectDialog* project_dialog = new ProjectDialog( project_gcid );
				m_mdi_area->addSubWindow( project_dialog );
				project_dialog->show();
			}

			//首先removeSubWindow，不会删除widget，再有就是点击subWindow的close 按钮，mdi不会调用removeSubWindow，只是隐藏widget，要重新显示subWindow，只用subWindow->showNormal(), form->show();
			//想隐藏也可以直接调用subWindow的hide（）函数
			//正统的关闭和显示是addSubWindow和removeSubWindow的成对调用。
			//可以设置设置subWindow和其widget的WA_DeleteOnClose属性，关闭时会删除subWindow和其widget，再显示时复制创建form和subWindow，这种情况在有些情况下还是有必要的，至少节约内存

			//Public Slots
			//	void activateNextSubWindow()
			//	void activatePreviousSubWindow()
			//	void cascadeSubWindows()
			//	void closeActiveSubWindow()
			//	void closeAllSubWindows()
			//	void setActiveSubWindow( QMdiSubWindow* window )
			//	void tileSubWindows() // 将所有子窗口在 area 的可视部分排列整齐
			//Signals
			//	void subWindowActivated( QMdiSubWindow* window ) // 切换激活的窗口时发出
		}
		else if( event->type() == QEvent::Show ) {
			if( target == m_main_tool_bar ) {
				m_action_show_tool_bar_main->setChecked( true );
			}
			else if( target == m_dock_widget_project ) {
				m_action_show_dock_project->setChecked( true );
			}
			else if( target == m_dock_widget_1 ) {
				m_action_show_dock_1->setChecked( true );
			}
			else if( target == m_dock_widget_2 ) {
				m_action_show_dock_2->setChecked( true );
			}
			else if( target == m_dock_widget_3 ) {
				m_action_show_dock_3->setChecked( true );
			}
		}
		else if( event->type() == QEvent::Close ) {
			if( target == m_main_tool_bar ) {
				m_action_show_tool_bar_main->setChecked( false );
			}
			else if( target == m_dock_widget_project ) {
				m_action_show_dock_project->setChecked( false );
			}
			else if( target == m_dock_widget_1 ) {
				m_action_show_dock_1->setChecked( false );
			}
			else if( target == m_dock_widget_2 ) {
				m_action_show_dock_2->setChecked( false );
			}
			else if( target == m_dock_widget_3 ) {
				m_action_show_dock_3->setChecked( false );
			}
		}
		return QMainWindow::eventFilter( target, event );
	}

} // namespace certify
