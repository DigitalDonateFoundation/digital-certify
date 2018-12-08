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

#include "project_dialog.h"

namespace certify {

	ProjectDialog::ProjectDialog( QWidget* parent )
	: QDialog( parent )
	, m_list_view( nullptr )
	, m_item_model( nullptr )
	, m_proxy_model( nullptr )
	, m_menu_list( nullptr )
	, m_action_list_test( nullptr )
	, m_action_list_text( nullptr )
	, m_layout_v( nullptr )
	, m_project( nullptr )
	, m_path_app_folder( "" )
	, m_path_cfg_folder( "" )
	, m_path_dbf_project( "" ) {
		m_project = Project::GetInstance();
		InitInterface();
	}

	ProjectDialog::~ProjectDialog() {
	}

	void ProjectDialog::InitInterface() {
		setMaximumWidth( 250 );

		QFont font_text;
		font_text.setPointSize( 9 );
		font_text.setFamily( "SimSun" );
		setFont( font_text );

		m_list_view = new QListView( this );
		m_list_view->setSpacing( 5 );
		m_list_view->setFont( font_text );
		m_list_view->setItemDelegate( new ProjectListItemDelegate );
		//m_list_view->setEditTriggers( QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked ); // ˫����ѡ�е���ʱ��ʾ�༭��
		m_list_view->setEditTriggers( QAbstractItemView::NoEditTriggers ); // ���ñ༭������˫���Żᴥ��
		m_list_view->setSelectionBehavior( QAbstractItemView::SelectRows ); // ѡ������ // SelectRows��MultiSelection
		m_list_view->setSelectionMode( QAbstractItemView::ExtendedSelection ); // ѡ��ʽ
		m_list_view->setContextMenuPolicy( Qt::CustomContextMenu ); // �Ҽ��˵�
		//m_list_view->setAlternatingRowColors( true ); // ��ż�б���ɫ
		m_list_view->setViewMode( QListView::ListMode ); // ������ʾģʽ
		m_list_view->setDragEnabled( false ); // �������϶�
		//m_list_view->setToolTip( QString::fromLocal8Bit( "123" ) );
		m_item_model = new QStandardItemModel();
//		InitData();
		m_proxy_model = new QSortFilterProxyModel( m_list_view );
		m_proxy_model->setSourceModel( m_item_model );
		m_proxy_model->setFilterRole( DEF_USER_ROLE_PROJECT_STATUS );
		m_proxy_model->setDynamicSortFilter( true );
		m_list_view->setModel( m_proxy_model );
		
		m_menu_list = nullptr; // �˵�ѡ������Ƿ�ѡ���� OnShowListMenu() ���ж�̬��ʾ

		m_layout_v = new QVBoxLayout();
		m_layout_v->setContentsMargins( -1, -1, -1, -1 );
		m_layout_v->addWidget( m_list_view );

		setLayout( m_layout_v );

		// �б�˵�ѡ�� m_action_list_test �����Ƿ�ѡ���� OnShowListMenu() ���ж�̬��ʾ
		// �б�˵�ѡ�� m_action_list_text �����Ƿ�ѡ���� OnShowListMenu() ���ж�̬��ʾ
		QObject::connect( m_list_view, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SLOT( OnShowListMenu( const QPoint& ) ) ); // ��Ҫ setContextMenuPolicy( Qt::CustomContextMenu ) �б�
		QObject::connect( m_list_view, SIGNAL( doubleClicked( const QModelIndex& ) ), this, SLOT( OnProjectListItemDoubleClicked( const QModelIndex& ) ) ); // ��������˳�ͻ
	}

	void ProjectDialog::InitData() {
		for( int32_t i = 0; i < 3; ++i ) {
			QStandardItem* item = new QStandardItem;
			UserData user_data;
			user_data.m_project_name = QString( "Name: %1" ).arg( i );
			user_data.m_project_path = QString( "Path: 1331234567%1" ).arg( i );
			item->setData( rand() % 4, DEF_USER_ROLE_PROJECT_STATUS ); // ��һ��ȡ
			item->setData( QVariant::fromValue( user_data ), DEF_USER_ROLE_PROJECT_USERDATA ); // �����ȡ
			item->setToolTip( QString( "%1 1331234567%2" ).arg( i ).arg( i ) );
			m_item_model->appendRow( item );
		}
	}

	int32_t ProjectDialog::LoadExistProject() {
#ifdef __OS_WINDOWS__
		wchar_t char_path[MAX_PATH] = { 0 };
		GetModuleFileName( NULL, char_path, MAX_PATH );
		std::string app_exec_path = basicx::StringToAnsiChar( char_path );
#endif
		size_t slash_index = app_exec_path.rfind( '\\' );
		m_path_app_folder = app_exec_path.substr( 0, slash_index );
		m_path_cfg_folder = m_path_app_folder + "\\configs";
		m_path_dbf_project = m_path_cfg_folder + "\\project.db";

		int32_t project_load = m_project->LoadExistProject( m_path_dbf_project );
		if( project_load < 0 ) {
			std::string log_info = "����Ŀ��Ϣ�ļ���ȡ������Ŀʧ�ܣ�";
			QMessageBox::warning( this, QString::fromLocal8Bit( "����" ), QString::fromLocal8Bit( log_info.c_str() ) );
		}

		std::vector<ProjectItem*> vec_project = m_project->GetAllProject();
		size_t project_size = vec_project.size();
		for( size_t i = 0; i < project_size; i++ ) {
			ProjectItem* project_item = vec_project[i];
			QStandardItem* item = new QStandardItem;
			UserData user_data;
			user_data.m_project_name = QString( project_item->m_name.c_str() );
			user_data.m_project_path = QString( project_item->m_path.c_str() );
			item->setData( DEF_PROJECT_STATUS_CREATE, DEF_USER_ROLE_PROJECT_STATUS ); // ��һ��ȡ
			item->setData( QVariant::fromValue( user_data ), DEF_USER_ROLE_PROJECT_USERDATA ); // �����ȡ
			item->setToolTip( QString( project_item->m_path.c_str() ));
			m_item_model->appendRow( item );
		}

		return project_size;
	}

	bool ProjectDialog::CreateProject( std::string name, std::string path ) {
		if( true == m_project->CreateProject( name, path ) ) {
			QStandardItem* item = new QStandardItem;
			UserData user_data;
			user_data.m_project_name = QString::fromLocal8Bit( name.c_str() );
			user_data.m_project_path = QString::fromLocal8Bit( path.c_str() );
			item->setData( DEF_PROJECT_STATUS_CREATE, DEF_USER_ROLE_PROJECT_STATUS ); // ��һ��ȡ
			item->setData( QVariant::fromValue( user_data ), DEF_USER_ROLE_PROJECT_USERDATA ); // �����ȡ
			item->setToolTip( QString::fromLocal8Bit( path.c_str() ) );
			m_item_model->appendRow( item );
			return true;
		}
		return false;
	}

	bool ProjectDialog::CanCreateProject( std::string name, std::string path ) {
		return m_project->CanCreateProject( name, path );
	}

	void ProjectDialog::OnActionListTest() {
		QMessageBox::information( this, QString::fromLocal8Bit( "��ʾ" ), QString::fromLocal8Bit( "Test" ) );
	}

	void ProjectDialog::OnActionListText() {
		QMessageBox::information( this, QString::fromLocal8Bit( "��ʾ" ), QString::fromLocal8Bit( "Text" ) );

//		m_proxy_model->setFilterFixedString( QString::number( DEF_PROJECT_STATUS_MODIFY ) ); // ���ԣ�����ʾ��ɫ�� // setFilterFixedString( QString() ) ��ԭ
	}

	void ProjectDialog::OnShowListMenu( const QPoint& point ) {
		if( m_menu_list != nullptr ) {
			delete m_menu_list;
			m_menu_list = nullptr;
		}

		m_menu_list = new QMenu( m_list_view );

		QModelIndexList list_model_index = m_list_view->selectionModel()->selectedIndexes(); // ���л����
		if( list_model_index.size() > 0 ) { // ѡ����
			m_action_list_test = m_menu_list->addAction( QString::fromLocal8Bit( "ѡ��" ) );
			m_action_list_test->setIcon( QIcon( ":/certify/resource/certify.ico" ) );
			QObject::connect( m_action_list_test, SIGNAL( triggered() ), this, SLOT( OnActionListTest() ) );

//			m_proxy_model->setFilterFixedString( QString::number( DEF_PROJECT_STATUS_ERRORS ) ); // ���ԣ�����ʾ��ɫ�� // setFilterFixedString( QString() ) ��ԭ
		}
		else { // δѡ��
			m_action_list_test = m_menu_list->addAction( QString::fromLocal8Bit( "δѡ" ) );
			m_action_list_test->setIcon( QIcon( ":/certify/resource/certify.ico" ) );
			QObject::connect( m_action_list_test, SIGNAL( triggered() ), this, SLOT( OnActionListTest() ) );

//			m_proxy_model->setFilterFixedString( QString::number( DEF_PROJECT_STATUS_COMMIT ) ); // ���ԣ�����ʾ��ɫ�� // setFilterFixedString( QString() ) ��ԭ
		}

		int32_t row_count = m_item_model->rowCount();
		if( row_count > 0 ) { // ���б���
			m_action_list_text = m_menu_list->addAction( QString::fromLocal8Bit( "�ı�" ) );
			m_action_list_text->setIcon( QIcon( ":/certify/resource/certify.ico" ) );
			QObject::connect( m_action_list_text, SIGNAL( triggered() ), this, SLOT( OnActionListText() ) );
		}

		m_menu_list->exec( QCursor::pos() );
	}

	void ProjectDialog::OnProjectListItemDoubleClicked( const QModelIndex& index ) {
		int32_t status = index.data( DEF_USER_ROLE_PROJECT_STATUS ).toInt();
		UserData user_data = index.data( DEF_USER_ROLE_PROJECT_USERDATA ).value<UserData>();
		QMessageBox::information( this, QString::fromLocal8Bit( "��ʾ" ), QString( "DoubleClicked: %1 %2 %3 %4" ).arg( index.row() ).arg( status ).arg( user_data.m_project_name ).arg( user_data.m_project_path ) );

		// ��ʹ�ô���ģ�ͺ����ڿ����˶�̬����ģʽ������޸Ĵ���ģ�͵����ݣ��ڵ�һ��item�޸����ݺ���ܾͲ��ڵ�ǰ����ģ���У��ᱻ���˵��������item��QModelIndex�ͻ�仯�����º������޸�ʧ�ܡ�
		// �����������ɴ���һ�ǲ��޸Ĵ���ģ�Ͷ���ֱ���޸�Դģ�͵����ݣ��������޸�ģ�����ݵ�ʱ��رմ���ģ�͵Ķ�̬�������޸����ٿ�����
		// m_proxy_model->setDynamicSortFilter( false );
		// m_proxy_model->setDynamicSortFilter( true );
//		QModelIndex index_test = m_proxy_model->mapToSource( index ); // �����Ȼ�ȡ Դ Model �� ModelIndex
//		m_item_model->setData( index_test, DEF_PROJECT_STATUS_ERRORS, DEF_USER_ROLE_PROJECT_STATUS ); // ���ԣ���ɺ�ɫ
	}

} // namespace certify
