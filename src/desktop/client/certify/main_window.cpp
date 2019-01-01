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
	, m_action_lang_english( nullptr )
	, m_action_lang_chinese( nullptr )
	, m_menu_bar( nullptr )
	, m_menu_file( nullptr )
	, m_menu_tool( nullptr )
	, m_menu_view( nullptr )
	, m_menu_view_tooler( nullptr )
	, m_menu_view_docker( nullptr )
	, m_menu_view_langts( nullptr )
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
		m_action_new_project->setText( QString::fromLocal8Bit( "�½�(&N)" ) );
		m_action_new_project->setToolTip( QString::fromLocal8Bit( "�����µ���Ŀ" ) );
		//m_action_new_project->setStatusTip( QString::fromLocal8Bit( "�����µ���Ŀ" ) );
		m_action_new_project->setShortcut( QKeySequence( "Ctrl+N" ) );
		m_action_new_project->setIcon( QIcon( ":/certify/resource/action_new_project.ico" ) );

		m_action_exit = new QAction( this );
		m_action_exit->setText( QString::fromLocal8Bit( "�˳�(&Q)" ) );
		m_action_exit->setToolTip( QString::fromLocal8Bit( "�˳���ǰϵͳ" ) );
		//m_action_exit->setStatusTip( QString::fromLocal8Bit( "�˳���ǰϵͳ" ) );
		m_action_exit->setShortcut( QKeySequence( "Ctrl+Q" ) );
		m_action_exit->setIcon( QIcon( ":/certify/resource/action_exit.ico" ) );

		m_action_about = new QAction( this );
		m_action_about->setText( QString::fromLocal8Bit( "����(&A)" ) );
		m_action_about->setToolTip( QString::fromLocal8Bit( "ϵͳ�����Ϣ" ) );
		//m_action_about->setStatusTip( QString::fromLocal8Bit( "ϵͳ�����Ϣ" ) );
		m_action_about->setShortcut( QKeySequence( "Ctrl+A" ) );
		m_action_about->setIcon( QIcon( ":/certify/resource/action_about.ico" ) );

		m_action_tests = new QAction( this );
		m_action_tests->setText( QString::fromLocal8Bit( "����(&T)" ) );
		m_action_tests->setToolTip( QString::fromLocal8Bit( "�����������" ) );
		//m_action_tests->setStatusTip( QString::fromLocal8Bit( "�����������" ) );
		m_action_tests->setShortcut( QKeySequence( "Ctrl+T" ) );
		m_action_tests->setIcon( QIcon( ":/certify/resource/action_tests.ico" ) );

		m_action_show_tool_bar_main = new QAction( this );
		m_action_show_tool_bar_main->setText( QString::fromLocal8Bit( "��ʾϵͳ������" ) );
		m_action_show_tool_bar_main->setCheckable( true );

		m_action_save_layout = new QAction( this );
		m_action_save_layout->setText( QString::fromLocal8Bit( "���沼��" ) );
		m_action_save_layout->setCheckable( true );

		m_action_auto_start = new QAction( this );
		m_action_auto_start->setText( QString::fromLocal8Bit( "��������" ) );
		m_action_auto_start->setCheckable( true );

		m_action_show_dock_project = new QAction( this );
		m_action_show_dock_project->setText( QString::fromLocal8Bit( "��Ŀ�б�" ) );
		m_action_show_dock_project->setCheckable( true );

		m_action_show_dock_1 = new QAction( this );
		m_action_show_dock_1->setText( QString::fromLocal8Bit( "����-01" ) );
		m_action_show_dock_1->setCheckable( true );

		m_action_show_dock_2 = new QAction( this );
		m_action_show_dock_2->setText( QString::fromLocal8Bit( "����-02" ) );
		m_action_show_dock_2->setCheckable( true );

		m_action_show_dock_3 = new QAction( this );
		m_action_show_dock_3->setText( QString::fromLocal8Bit( "����-03" ) );
		m_action_show_dock_3->setCheckable( true );

		m_action_lang_english = new QAction( this );
		m_action_lang_english->setText( tr( "English" ) );
		m_action_lang_english->setCheckable( true );

		m_action_lang_chinese = new QAction( this );
		m_action_lang_chinese->setText( tr( "Chinses" ) );
		m_action_lang_chinese->setCheckable( true );
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
		m_menu_bar->setStyle( QStyleFactory::create( "windowsxp" ) ); // ��ƽ��
		m_menu_bar->setGeometry( QRect( 0, 0, 800, 21 ) );
		setMenuBar( m_menu_bar );

		m_menu_file = new QMenu( m_menu_bar );
		m_menu_file->addAction( m_action_new_project );
		m_menu_file->addAction( m_action_exit );
		m_menu_file->setTitle( QString::fromLocal8Bit( "�ļ�(&F)" ) );
		m_menu_bar->addAction( m_menu_file->menuAction() );

		m_menu_tool = new QMenu( m_menu_bar );
		m_menu_tool->setTitle( QString::fromLocal8Bit( "����(&T)" ) );
		m_menu_bar->addAction( m_menu_tool->menuAction() );

		QMainWindow::setCorner( Qt::TopLeftCorner, Qt::LeftDockWidgetArea ); // Ĭ�϶�������
		QMainWindow::setCorner( Qt::TopRightCorner, Qt::RightDockWidgetArea ); // Ĭ�϶�������

		QMainWindow::setCorner( Qt::BottomRightCorner, Qt::BottomDockWidgetArea ); // Ĭ�ϵײ���չ
		QMainWindow::setCorner( Qt::BottomLeftCorner, Qt::BottomDockWidgetArea ); // Ĭ�ϵײ���չ

		m_menu_view = new QMenu( m_menu_bar );
		m_menu_view_tooler = m_menu_view->addMenu( QString::fromLocal8Bit( "������(&T)" ) );
		m_menu_view_tooler->setIcon( QIcon( ":/certify/resource/action_show_tool.ico" ) );
		m_menu_view_tooler->addAction( m_action_show_tool_bar_main );
		m_menu_view_docker = m_menu_view->addMenu( QString::fromLocal8Bit( "ͣ����(&D)" ) );
		m_menu_view_docker->setIcon( QIcon( ":/certify/resource/action_show_dock.ico" ) );
		m_menu_view_docker->addAction( m_action_show_dock_project );
		m_menu_view_docker->addAction( m_action_show_dock_1 );
		m_menu_view_docker->addAction( m_action_show_dock_2 );
		m_menu_view_docker->addAction( m_action_show_dock_3 );
		m_menu_view_langts = m_menu_view->addMenu( QString::fromLocal8Bit( "������(&L)" ) );
		m_menu_view_langts->setIcon( QIcon( ":/certify/resource/action_show_lang.ico" ) );
		m_menu_view_langts->addAction( m_action_lang_english );
		m_menu_view_langts->addAction( m_action_lang_chinese );
		m_menu_view->addAction( m_action_save_layout );
		m_menu_view->addAction( m_action_auto_start );
		m_menu_view->setTitle( QString::fromLocal8Bit( "��ͼ(&V)" ) );
		m_menu_bar->addAction( m_menu_view->menuAction() );

		m_menu_help = new QMenu( m_menu_bar );
		m_menu_help->addAction( m_action_about );
		m_menu_help->setTitle( QString::fromLocal8Bit( "����(&H)" ) );
		m_menu_bar->addAction( m_menu_help->menuAction() );

		m_main_tool_bar = new QToolBar( this );
		m_main_tool_bar->setIconSize( QSize( 16, 16 ) );
		m_main_tool_bar->setObjectName( "MainToolBar" ); //
		m_main_tool_bar->setToolTip( QString::fromLocal8Bit( "ϵͳ������" ) );
		m_main_tool_bar->setWindowTitle( QString::fromLocal8Bit( "ϵͳ������" ) );
		m_main_tool_bar->addAction( m_action_about );
		m_main_tool_bar->addAction( m_action_tests );
		m_main_tool_bar->installEventFilter( this ); // ������ر��¼�
		addToolBar( Qt::TopToolBarArea, m_main_tool_bar );

		m_mdi_area = new QMdiArea( this );
		m_mdi_area->setActivationOrder( QMdiArea::CreationOrder ); // ���ü���˳�� CreationOrder��StackingOrder��ActivationHistoryOrder
		//m_mdi_area->setBackground( QColor( 192, 192, 192 ) ); // ���ñ�����Ĭ�ϻ�ɫ
		//m_mdi_area->setDocumentMode( false ); // �����ĵ�ģʽ���� macOS �� tabs ������ Safari �� Terminal
		//m_mdi_area->setOption( QMdiArea::DontMaximizeSubWindowOnActivation, false ); // ֻ��һ��ѡ��������Ӵ���ʱ��������������Ĭ�� false ����
		m_mdi_area->setViewMode( QMdiArea::TabbedView ); // ��ͼģʽ SubWindowView��TabbedView
		m_mdi_area->setTabPosition( QTabWidget::North ); // ��ǩҳλ�� North��South��West��East
		m_mdi_area->setTabShape( QTabWidget::Rounded ); // ��ǩҳ��ʽ Rounded��Triangular
		m_mdi_area->setTabsClosable( true ); // �Ƿ�ɹر�
		m_mdi_area->setTabsMovable( true ); // �Ƿ���ƶ�
		m_mdi_area->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded );
		m_mdi_area->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
		// �� QMdiArea �� ViewMode ��Ϊ TabbedView ��Ż���� QTabBar
		QList<QTabBar*> list_tab_bar = m_mdi_area->findChildren<QTabBar*>();
		for( QList<QTabBar*>::Iterator it_tb = list_tab_bar.begin(); it_tb != list_tab_bar.end(); ++it_tb ) {
			(*it_tb)->setExpanding( false ); // ����£��������������ǩҳ
		}
		setCentralWidget( m_mdi_area );

		m_label_logo = new QLabel( THE_APP_NAME, this );
		m_label_logo->setStyleSheet( "color:rgb(128,0,0);font:bold;" );
		m_label_logo->setAlignment( Qt::AlignHCenter );

		m_label_info = new QLabel( QString::fromLocal8Bit( "����" ), this );
		m_label_info->setAlignment( Qt::AlignLeft );

		QDateTime qDateTime = QDateTime::currentDateTime();
		m_label_time = new QLabel( qDateTime.toString( "yyyy-MM-dd hh:mm:ss" ), this ); // "yyyy-MM-dd dddd hh:mm:ss"
		m_label_time->setAlignment( Qt::AlignRight );

		m_status_bar = new QStatusBar( this );
		//m_status_bar->setStyleSheet( QString( "QStatusBar::item{border:0px}" ) ); // �����ָ���
		m_status_bar->insertPermanentWidget( 0, m_label_logo );
		m_status_bar->insertPermanentWidget( 1, m_label_info, 1 );
		m_status_bar->insertPermanentWidget( 2, m_label_time );
		// �޷�������һ����ʾ��ʶ���ڶ�����ʾ�Զ���ʾ����������ʾʱ�䣬�Զ����µ����ǻ��ŵ�һ����ֻ��ʾ�û��Զ�����Ϣ��
		setStatusBar( m_status_bar );

		QMainWindow::setDockOptions( AnimatedDocks | AllowNestedDocks | AllowTabbedDocks );

		m_project_list_dialog = new ProjectListDialog( this );
		m_text_edit_1 = new QTextEdit( this );
		m_text_edit_2 = new QTextEdit( this );
		m_text_edit_3 = new QTextEdit( this );

		m_dock_widget_project = new QDockWidget( this );
		m_dock_widget_project->setObjectName( "DockWidget_Project" ); //
		m_dock_widget_project->setContentsMargins( -1, -1, -1, -1 );
		m_dock_widget_project->setWindowTitle( QString::fromLocal8Bit( "��Ŀ�б�" ) );
		m_dock_widget_project->setWidget( m_project_list_dialog );
		m_dock_widget_project->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
		m_dock_widget_project->installEventFilter( this ); // ������ر��¼�
		addDockWidget( Qt::LeftDockWidgetArea, m_dock_widget_project );
		m_vec_dock_widget.push_back( m_dock_widget_project );

		m_dock_widget_1 = new QDockWidget( this );
		m_dock_widget_1->setObjectName( "DockWidget_1" ); //
		m_dock_widget_1->setContentsMargins( -1, -1, -1, -1 );
		m_dock_widget_1->setWindowTitle( QString::fromLocal8Bit( "����-01" ) );
		m_dock_widget_1->setWidget( m_text_edit_1 );
		m_dock_widget_1->setAllowedAreas( Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea | Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
		m_dock_widget_1->installEventFilter( this ); // ������ر��¼�
		addDockWidget( Qt::RightDockWidgetArea, m_dock_widget_1 );
		m_vec_dock_widget.push_back( m_dock_widget_1 );

		m_dock_widget_2 = new QDockWidget( this );
		m_dock_widget_2->setObjectName( "DockWidget_2" ); //
		m_dock_widget_2->setContentsMargins( -1, -1, -1, -1 );
		m_dock_widget_2->setWindowTitle( QString::fromLocal8Bit( "����-02" ) );
		m_dock_widget_2->setWidget( m_text_edit_2 );
		m_dock_widget_2->setAllowedAreas( Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea | Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
		m_dock_widget_2->installEventFilter( this ); // ������ر��¼�
		addDockWidget( Qt::BottomDockWidgetArea, m_dock_widget_2 );
		m_vec_dock_widget.push_back( m_dock_widget_2 );

		m_dock_widget_3 = new QDockWidget( this );
		m_dock_widget_3->setObjectName( "DockWidget_3" ); //
		m_dock_widget_3->setContentsMargins( -1, -1, -1, -1 );
		m_dock_widget_3->setWindowTitle( QString::fromLocal8Bit( "����-03" ) );
		m_dock_widget_3->setWidget( m_text_edit_3 );
		m_dock_widget_3->setAllowedAreas( Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea | Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
		m_dock_widget_3->installEventFilter( this ); // ������ر��¼�
		addDockWidget( Qt::BottomDockWidgetArea, m_dock_widget_3 );
		m_vec_dock_widget.push_back( m_dock_widget_3 );

		tabifyDockWidget( m_dock_widget_2, m_dock_widget_3 );

		m_action_show_tool_bar_main->setChecked( true );
		m_action_show_dock_project->setChecked( true );
		m_action_show_dock_1->setChecked( true );
		m_action_show_dock_2->setChecked( true );
		m_action_show_dock_3->setChecked( true );
		m_action_lang_english->setChecked( true );
		m_action_lang_chinese->setChecked( false );
		m_action_save_layout->setChecked( true );
		m_action_auto_start->setChecked( false );

		installEventFilter( this ); // ���������¼�

		QObject::connect( m_action_new_project, SIGNAL( triggered() ), this, SLOT( OnActionNewProject() ) );
		QObject::connect( m_action_exit, SIGNAL( triggered() ), this, SLOT( OnActionExit() ) );
		QObject::connect( m_action_about, SIGNAL( triggered() ), this, SLOT( OnActionAbout() ) );
		QObject::connect( m_action_tests, SIGNAL( triggered() ), this, SLOT( OnActionTests() ) );
		QObject::connect( m_action_show_tool_bar_main, SIGNAL( triggered( bool ) ), this, SLOT( ShowToolBar_Main( bool ) ) );
		QObject::connect( m_action_show_dock_project, SIGNAL( triggered( bool ) ), this, SLOT( ShowDockWidget_Project( bool ) ) );
		QObject::connect( m_action_show_dock_1, SIGNAL( triggered( bool ) ), this, SLOT( ShowDockWidget_1( bool ) ) );
		QObject::connect( m_action_show_dock_2, SIGNAL( triggered( bool ) ), this, SLOT( ShowDockWidget_2( bool ) ) );
		QObject::connect( m_action_show_dock_3, SIGNAL( triggered( bool ) ), this, SLOT( ShowDockWidget_3( bool ) ) );
		QObject::connect( m_action_lang_english, SIGNAL( triggered( bool ) ), this, SLOT( ChooseLanguageEnglish( bool ) ) );
		QObject::connect( m_action_lang_chinese, SIGNAL( triggered( bool ) ), this, SLOT( ChooseLanguageChinese( bool ) ) );
		QObject::connect( m_action_save_layout, SIGNAL( triggered( bool ) ), this, SLOT( OnActionSaveLayout( bool ) ) );
		QObject::connect( m_action_auto_start, SIGNAL( triggered( bool ) ), this, SLOT( OnActionAutoStart( bool ) ) );
		QObject::connect( m_mdi_area, SIGNAL( subWindowActivated( QMdiSubWindow* ) ), this, SLOT( OnSelectProjectLabel( QMdiSubWindow* ) ) );

		m_project_list_dialog->LoadExistProject();
	}

	void MainWindow::ReTranslateUI() {
		m_action_lang_english->setText( tr( "English" ) );
		m_action_lang_chinese->setText( tr( "Chinses" ) );
	}

	// QDockWidget �� QToolBar �������ö��������������� saveState() �� restoreState() ״̬��λ�á���С��
	void MainWindow::ReadSettings() {
		QSettings settings( REGISTRY_KEY_ORG, REGISTRY_KEY_APP ); // λ�ڣ�HKEY_CURRENT_USER -> Software -> REGISTRY_KEY_ORG

		m_not_first_run = settings.value( "NotFirstRunFlag" ).toBool();
		if( false == m_not_first_run ) { // �״����У��ޱ���ֵ��Ϊ���Ҳ˵�����沼�֡�Ϊ��
			WriteSettings(); // ��ʱ������Ǵ��ڳ�ʼ��ʱ�Ĳ���
		}
		else { // ��ȡ�ϴ��˳�ʱ����Ĳ���
			settings.beginGroup( "MainWindow" );

			restoreState( settings.value( "MainWindowLayout" ).toByteArray() );
			restoreGeometry( settings.value( "MainWindowGeometry" ).toByteArray() );

			m_mdi_area->restoreGeometry( settings.value( "MainWidgetGeometry" ).toByteArray() );

			m_action_show_tool_bar_main->setChecked( settings.value( "ActionShowTool_Main" ).toBool() );
			m_action_show_dock_project->setChecked( settings.value( "ActionShowDock_Project" ).toBool() );
			m_action_show_dock_1->setChecked( settings.value( "ActionShowDock_1" ).toBool() );
			m_action_show_dock_2->setChecked( settings.value( "ActionShowDock_2" ).toBool() );
			m_action_show_dock_3->setChecked( settings.value( "ActionShowDock_3" ).toBool() );
			m_action_lang_english->setChecked( settings.value( "ChooseLanguageEnglish" ).toBool() );
			m_action_lang_chinese->setChecked( settings.value( "ChooseLanguageChinese" ).toBool() );
			m_action_save_layout->setChecked( settings.value( "ActionSaveLayout" ).toBool() );
			m_action_auto_start->setChecked( settings.value( "ActionAutoStart" ).toBool() );

			ChooseLanguageEnglish( settings.value( "ChooseLanguageEnglish" ).toBool() );
			ChooseLanguageChinese( settings.value( "ChooseLanguageChinese" ).toBool() );

			settings.endGroup();

			// ��������������ȡ
		}
	}

	// QDockWidget �� QToolBar �������ö��������������� saveState() �� restoreState() ״̬��λ�á���С��
	void MainWindow::WriteSettings() {
		QSettings settings( REGISTRY_KEY_ORG, REGISTRY_KEY_APP ); // λ�ڣ�HKEY_CURRENT_USER -> Software -> REGISTRY_KEY_ORG

		if( m_action_save_layout->isChecked() ) {
			settings.setValue( "NotFirstRunFlag", true ); // ʼ��Ϊ��

			settings.beginGroup( "MainWindow" );

			settings.setValue( "MainWindowLayout", saveState() );
			settings.setValue( "MainWindowGeometry", saveGeometry() );

			settings.setValue( "MainWidgetGeometry", m_mdi_area->saveGeometry() );

			settings.setValue( "ActionShowTool_Main", m_action_show_tool_bar_main->isChecked() );
			settings.setValue( "ActionShowDock_Project", m_action_show_dock_project->isChecked() );
			settings.setValue( "ActionShowDock_1", m_action_show_dock_1->isChecked() );
			settings.setValue( "ActionShowDock_2", m_action_show_dock_2->isChecked() );
			settings.setValue( "ActionShowDock_3", m_action_show_dock_3->isChecked() );
			settings.setValue( "ChooseLanguageEnglish", m_action_lang_english->isChecked() );
			settings.setValue( "ChooseLanguageChinese", m_action_lang_chinese->isChecked() );
			settings.setValue( "ActionSaveLayout", m_action_save_layout->isChecked() );
			settings.setValue( "ActionAutoStart", m_action_auto_start->isChecked() );

			settings.endGroup();

			// ��������������
		}
		else {
			settings.setValue( "NotFirstRunFlag", true ); // ʼ��Ϊ��

			settings.beginGroup( "MainWindow" );

			settings.setValue( "MainWindowLayout", settings.value( "MainWindowLayout" ).toByteArray() );
			settings.setValue( "MainWindowGeometry", settings.value( "MainWindowGeometry" ).toByteArray() );

			settings.setValue( "MainWidgetGeometry", settings.value( "MainWidgetGeometry" ).toByteArray() );

			settings.setValue( "ActionShowTool_Main", settings.value( "ActionShowTool_Main" ).toBool() );
			settings.setValue( "ActionShowDock_Project", settings.value( "ActionShowDock_Project" ).toBool() );
			settings.setValue( "ActionShowDock_1", settings.value( "ActionShowDock_1" ).toBool() );
			settings.setValue( "ActionShowDock_2", settings.value( "ActionShowDock_2" ).toBool() );
			settings.setValue( "ActionShowDock_3", settings.value( "ActionShowDock_3" ).toBool() );
			settings.setValue( "ChooseLanguageEnglish", settings.value( "ChooseLanguageEnglish" ).toBool() );
			settings.setValue( "ChooseLanguageChinese", settings.value( "ChooseLanguageChinese" ).toBool() );
			settings.setValue( "ActionSaveLayout", m_action_save_layout->isChecked() ); // ֻ�����������ֵ������������ԭֵ
			settings.setValue( "ActionAutoStart", settings.value( "ActionAutoStart" ).toBool() );

			settings.endGroup();

			// ��������������
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
			std::string log_info = "��ϵͳ��֧������ͼ�꣡";
			m_syslog->LogWrite( basicx::syslog_level::c_warn, m_log_cate, log_info );
			QMessageBox::warning( this, QString::fromLocal8Bit( "����" ), QString::fromLocal8Bit( log_info.c_str() ) );
			return;
		}

		QApplication::setQuitOnLastWindowClosed( false ); // ���������رհ�ť���������������Ҫ�����̲˵��˳�

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

		m_tray_icon->show(); // �ŵ� StartDialogThread() �����̻����

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
		if( QMessageBox::Yes == QMessageBox::question( this, QString::fromLocal8Bit( "ѯ��" ), QString::fromLocal8Bit( "ȷ���˳�ϵͳ��" ) ) ) {
			m_user_exit = true;
			WriteSettings(); // �����������
			m_mdi_area->closeAllSubWindows();
			CloseFloatDockWidget(); // �����и���ͣ�����򹤾���ʱ���ܵ��±���
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
		std::string info_text = "����������������";
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
		QMessageBox::information( this, QString::fromLocal8Bit( "��ʾ" ), QString::fromLocal8Bit( "С���ӹԹԣ����Ŷ�������" ) );
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

	void MainWindow::ChooseLanguageEnglish( bool choose ) {
		m_action_lang_english->setChecked( choose );
		m_action_lang_chinese->setChecked( !choose );
		if( true == choose ) {
			QApplication::removeTranslator( &m_translator );
			m_translator.load( "" );
			QApplication::installTranslator( &m_translator );
			ReTranslateUI();
		}
	}

	void MainWindow::ChooseLanguageChinese( bool choose ) {
		m_action_lang_english->setChecked( !choose );
		m_action_lang_chinese->setChecked( choose );
		if( true == choose ) {
			QApplication::removeTranslator( &m_translator );
			m_translator.load( "./tslangs/certify_cn.qm" );
			QApplication::installTranslator( &m_translator );
			ReTranslateUI();
		}
	}

	void MainWindow::OnActionSaveLayout( bool save ) {
		if( true == save )
		{
			std::string log_info = "�û� ȡ�� ���ڲ��ֱ��档";
			m_syslog->LogWrite( basicx::syslog_level::c_info, m_log_cate, log_info );
			QMessageBox::information( this, QString::fromLocal8Bit( "��ʾ" ), QString::fromLocal8Bit( "��ϵͳ�˳�ʱ�� ���� ���ڲ��֡�" ) );
		}
		else
		{
			std::string log_info = "�û� ���� ���ڲ��ֱ��档";
			m_syslog->LogWrite( basicx::syslog_level::c_info, m_log_cate, log_info );
			QMessageBox::information( this, QString::fromLocal8Bit( "��ʾ" ), QString::fromLocal8Bit( "��ϵͳ�˳�ʱ�� ���� ���ڲ��֡�" ) );
		}
	}

	// ����е���ע���֮���ϵͳ��ʾ��������Ҫ ����->����->������->�嵥�ļ�->UACִ�м���->requireAdministrator �Ի�ȡ����ԱȨ��
	void MainWindow::OnActionAutoStart( bool start ) {
#ifdef __OS_WINDOWS__
		wchar_t char_path[MAX_PATH] = { 0 };
		GetModuleFileName( NULL, char_path, MAX_PATH );
		std::string app_exec_path = basicx::StringToAnsiChar( char_path );
		std::string reg_key = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";
		QSettings* settings = new QSettings( reg_key.c_str(), QSettings::NativeFormat );
		if( true == start ) {
			settings->setValue( QString::fromLocal8Bit( REGISTRY_KEY_APP ), QString::fromLocal8Bit( app_exec_path.c_str() ) );
			std::string log_info = "�û� ���� ϵͳ�������С�";
			m_syslog->LogWrite( basicx::syslog_level::c_info, m_log_cate, log_info );
			QMessageBox::information( this, QString::fromLocal8Bit( "��ʾ" ), QString::fromLocal8Bit( "������ϵͳ�������С�" ) );
		}
		else {
			settings->remove( QString::fromLocal8Bit( REGISTRY_KEY_APP ) );
			std::string log_info = "�û� ȡ�� ϵͳ�������С�";
			m_syslog->LogWrite( basicx::syslog_level::c_info, m_log_cate, log_info );
			QMessageBox::information( this, QString::fromLocal8Bit( "��ʾ" ), QString::fromLocal8Bit( "��ȡ��ϵͳ�������С�" ) );
		}
		delete settings;
#endif
	}

	void MainWindow::OnSelectProjectLabel( QMdiSubWindow* mdi_sub_window ) {
		if( mdi_sub_window != nullptr ) {
			ProjectDialog* project_dialog = qobject_cast<ProjectDialog*>( mdi_sub_window->widget() );
			m_project_list_dialog->SetCurrentProject( project_dialog->m_project_gcid );
		}
		else { // �����ѱ��ر�ɾ��
			m_project_list_dialog->SetCurrentProject( "" ); // ȫ��ѡ
		}
	}

	void MainWindow::closeEvent( QCloseEvent* event ) {
		if( false == m_user_exit ) {
			//QMessageBox::information( this, QString::fromLocal8Bit( "��ʾ" ), QString::fromLocal8Bit( "���򽫼������У������˳������һ�����ͼ�ꡣ" ) );
			HideMainWindow();
			event->ignore();
		}
	}

	bool MainWindow::eventFilter( QObject* target, QEvent* event ) {
		if( event->type() == ProjectListItemClickedEvent::m_type ) { // �������Ѵ򿪵�ѡ�У�δ�򿪵Ĳ���
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
		else if( event->type() == ProjectListItemDoubleClickedEvent::m_type ) { // ˫�����Ѵ򿪵�ѡ�У�δ�򿪵Ĵ���
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

			//����removeSubWindow������ɾ��widget�����о��ǵ��subWindow��close ��ť��mdi�������removeSubWindow��ֻ������widget��Ҫ������ʾsubWindow��ֻ��subWindow->showNormal(), form->show();
			//������Ҳ����ֱ�ӵ���subWindow��hide��������
			//��ͳ�Ĺرպ���ʾ��addSubWindow��removeSubWindow�ĳɶԵ��á�
			//������������subWindow����widget��WA_DeleteOnClose���ԣ��ر�ʱ��ɾ��subWindow����widget������ʾʱ���ƴ���form��subWindow�������������Щ����»����б�Ҫ�ģ����ٽ�Լ�ڴ�

			//Public Slots
			//	void activateNextSubWindow()
			//	void activatePreviousSubWindow()
			//	void cascadeSubWindows()
			//	void closeActiveSubWindow()
			//	void closeAllSubWindows()
			//	void setActiveSubWindow( QMdiSubWindow* window )
			//	void tileSubWindows() // �������Ӵ����� area �Ŀ��Ӳ�����������
			//Signals
			//	void subWindowActivated( QMdiSubWindow* window ) // �л�����Ĵ���ʱ����
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
