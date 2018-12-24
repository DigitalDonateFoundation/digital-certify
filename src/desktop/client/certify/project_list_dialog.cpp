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

#include "project_list_dialog.h"

namespace certify {

	ProjectListDialog::ProjectListDialog( QWidget* parent )
	: QDialog( parent )
	, m_parent( parent )
	, m_list_view( nullptr )
	, m_item_model( nullptr )
	, m_menu_list( nullptr )
	, m_action_new_project( nullptr )
	, m_action_list_text( nullptr )
	, m_layout_v( nullptr )
	, m_create_project_dialog( nullptr )
	, m_project( nullptr )
	, m_path_app_folder( "" )
	, m_path_cfg_folder( "" )
	, m_path_dbf_project( "" ) {
		m_project = Project::GetInstance();

#ifdef __OS_WINDOWS__
		wchar_t char_path[MAX_PATH] = { 0 };
		GetModuleFileName( NULL, char_path, MAX_PATH );
		std::string app_exec_path = basicx::StringToAnsiChar( char_path );
#endif
		size_t slash_index = app_exec_path.rfind( '\\' );
		m_path_app_folder = app_exec_path.substr( 0, slash_index );
		m_path_cfg_folder = m_path_app_folder + "\\configs";
		m_path_dbf_project = m_path_cfg_folder + "\\project.db";

		InitInterface();
	}

	ProjectListDialog::~ProjectListDialog() {
	}

	void ProjectListDialog::InitInterface() {
		setMaximumWidth( 250 );
		setContentsMargins( -1, -1, -1, -1 );

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
		m_list_view->setAutoScroll( true );
		m_list_view->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded );
		m_list_view->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
		m_item_model = new QStandardItemModel();
		m_list_view->setModel( m_item_model );
		
		m_menu_list = nullptr; // �˵�ѡ������Ƿ�ѡ���� OnShowListMenu() ���ж�̬��ʾ

		m_layout_v = new QVBoxLayout();
		m_layout_v->setContentsMargins( -1, -1, -1, -1 );
		m_layout_v->addWidget( m_list_view );

		setLayout( m_layout_v );

		m_create_project_dialog = new CreateProjectDialog( this );
		m_create_project_dialog->hide();

		// �б�˵�ѡ�� m_action_new_project �����Ƿ�ѡ���� OnShowListMenu() ���ж�̬��ʾ
		// �б�˵�ѡ�� m_action_list_text �����Ƿ�ѡ���� OnShowListMenu() ���ж�̬��ʾ
		QObject::connect( m_list_view, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SLOT( OnShowListMenu( const QPoint& ) ) ); // ��Ҫ setContextMenuPolicy( Qt::CustomContextMenu ) �б�
		QObject::connect( m_list_view, SIGNAL( clicked( const QModelIndex& ) ), this, SLOT( OnProjectListViewClicked( const QModelIndex& ) ) ); // �����б�հ״����ᴥ��
		QObject::connect( m_list_view, SIGNAL( doubleClicked( const QModelIndex& ) ), this, SLOT( OnProjectListItemDoubleClicked( const QModelIndex& ) ) ); // ˫���б�հ״����ᴥ�� // �������ܻ���˳�ͻ
	}

	int32_t ProjectListDialog::LoadExistProject() {
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
			user_data.m_project_gcid = project_item->GetGCID();
			user_data.m_project_name = project_item->GetName();
			user_data.m_project_create_time = project_item->GetCreateTime();
			item->setData( DEF_PROJECT_STATUS_CREATE, DEF_USER_ROLE_PROJECT_STATUS ); // ��һ��ȡ
			item->setData( QVariant::fromValue( user_data ), DEF_USER_ROLE_PROJECT_USERDATA ); // �����ȡ
			std::string tool_tip = "";
			FormatLibrary::StandardLibrary::FormatTo( tool_tip, "{0}\n{1}", project_item->GetGCID(), project_item->GetPath() );
			item->setToolTip( QString::fromLocal8Bit( tool_tip.c_str() ) );
			m_item_model->appendRow( item );
		}

		return (int32_t)project_size;
	}

	bool ProjectListDialog::CreateProject( std::string name, std::string path ) {
		ProjectItem* project_item = m_project->CreateProject( name, path );
		if( project_item != nullptr ) {
			QStandardItem* item = new QStandardItem;
			UserData user_data;
			user_data.m_project_gcid = project_item->GetGCID();
			user_data.m_project_name = project_item->GetName();
			user_data.m_project_create_time = project_item->GetCreateTime();
			item->setData( DEF_PROJECT_STATUS_CREATE, DEF_USER_ROLE_PROJECT_STATUS ); // ��һ��ȡ
			item->setData( QVariant::fromValue( user_data ), DEF_USER_ROLE_PROJECT_USERDATA ); // �����ȡ
			std::string tool_tip = "";
			FormatLibrary::StandardLibrary::FormatTo( tool_tip, "{0}\n{1}", project_item->GetGCID(), project_item->GetPath() );
			item->setToolTip( QString::fromLocal8Bit( tool_tip.c_str() ) );
			m_item_model->appendRow( item );
			return true;
		}
		return false;
	}

	bool ProjectListDialog::CanCreateProject( std::string name, std::string path ) {
		return m_project->CanCreateProject( name, path );
	}

	void ProjectListDialog::SetCurrentProject( std::string gcid ) { // ʹ�� QSortFilterProxyModel �ᵼ�� setCurrentIndex��scrollTo ʧЧ
		int32_t row_count = m_item_model->rowCount();
		if( row_count > 0 ) { // ���б���
			for( int32_t i = 0; i < row_count; ++i ) {
				QModelIndex index = m_item_model->index( i, 0 );
				UserData user_data = index.data( DEF_USER_ROLE_PROJECT_USERDATA ).value<UserData>();
				if( user_data.m_project_gcid == gcid ) {
					//m_list_view->selectionModel()->select( index, QItemSelectionModel::Select );
					m_list_view->setCurrentIndex( index );
					m_list_view->scrollTo( index );
				}
			}
		}
	}

	void ProjectListDialog::OnActionListText() {
		QMessageBox::information( this, QString::fromLocal8Bit( "��ʾ" ), QString::fromLocal8Bit( "Text" ) );
	}

	void ProjectListDialog::OnActionNewProject() {
		if( m_create_project_dialog != nullptr ) {
			if( QDialog::Accepted == m_create_project_dialog->exec() ) {
				std::string project_name = m_create_project_dialog->m_project_name;
				std::string project_path = m_create_project_dialog->m_project_path;
				if( "" == project_name ) {
					QMessageBox::information( this, QString::fromLocal8Bit( "��ʾ" ), QString::fromLocal8Bit( "�½���Ŀ ���� Ϊ�գ�" ) );
				}
				else if( "" == project_path ) {
					QMessageBox::information( this, QString::fromLocal8Bit( "��ʾ" ), QString::fromLocal8Bit( "�½���Ŀ ·�� Ϊ�գ�" ) );
				}
				else if( false == CanCreateProject( project_name, project_path ) ) {
					QMessageBox::information( this, QString::fromLocal8Bit( "��ʾ" ), QString::fromLocal8Bit( "��Ŀ���� �� ��Ŀ·�� �Ѿ����ڣ��޷��½���Ŀ��" ) );
				}
				else {
					if( false == CreateProject( project_name, project_path ) ) {
						QMessageBox::information( this, QString::fromLocal8Bit( "��ʾ" ), QString::fromLocal8Bit( "�½���Ŀʧ�ܣ�" ) );
					}
				}
			}
		}
	}

	void ProjectListDialog::OnShowListMenu( const QPoint& point ) {
		if( m_menu_list != nullptr ) {
			delete m_menu_list;
			m_menu_list = nullptr;
		}

		m_menu_list = new QMenu( m_list_view );

		QModelIndexList list_model_index = m_list_view->selectionModel()->selectedIndexes(); // ���л����
		if( list_model_index.size() > 0 ) { // ѡ����
			m_action_list_text = m_menu_list->addAction( QString::fromLocal8Bit( "ѡ��" ) );
			m_action_list_text->setIcon( QIcon( ":/certify/resource/certify.ico" ) );
			QObject::connect( m_action_list_text, SIGNAL( triggered() ), this, SLOT( OnActionListText() ) );
		}
		else { // δѡ��
			m_action_new_project = m_menu_list->addAction( QString::fromLocal8Bit( "�½�" ) );
			m_action_new_project->setIcon( QIcon( ":/certify/resource/action_new_project.ico" ) );
			QObject::connect( m_action_new_project, SIGNAL( triggered() ), this, SLOT( OnActionNewProject() ) );
		}

		int32_t row_count = m_item_model->rowCount();
		if( row_count > 0 ) { // ���б���
			m_action_list_text = m_menu_list->addAction( QString::fromLocal8Bit( "�ı�" ) );
			m_action_list_text->setIcon( QIcon( ":/certify/resource/certify.ico" ) );
			QObject::connect( m_action_list_text, SIGNAL( triggered() ), this, SLOT( OnActionListText() ) );
		}

		m_menu_list->exec( QCursor::pos() );
	}

	void ProjectListDialog::OnProjectListViewClicked( const QModelIndex& index ) {
		QModelIndexList list_model_index = m_list_view->selectionModel()->selectedIndexes(); // ���л����
		if( list_model_index.size() > 0 ) { // ѡ����
			for( int32_t i = 0; i < list_model_index.size(); ++i ) { // ֻ��һ��
				QModelIndex index = list_model_index[i];
				UserData user_data = index.data( DEF_USER_ROLE_PROJECT_USERDATA ).value<UserData>();
				ProjectListItemClickedEvent event_item( user_data.m_project_gcid );
				QApplication::sendEvent( m_parent, &event_item );
			}
		}
	}

	void ProjectListDialog::OnProjectListItemDoubleClicked( const QModelIndex& index ) {
		int32_t status = index.data( DEF_USER_ROLE_PROJECT_STATUS ).toInt();
		UserData user_data = index.data( DEF_USER_ROLE_PROJECT_USERDATA ).value<UserData>();
		ProjectListItemDoubleClickedEvent event_item( user_data.m_project_gcid );
		QApplication::sendEvent( m_parent, &event_item );
		//QMessageBox::information( this, QString::fromLocal8Bit( "��ʾ" ), QString( "DoubleClicked: %1 %2 %3 %4" ).arg( index.row() ).arg( status ).arg( QString::fromLocal8Bit( user_data.m_project_name.c_str() ) ).arg( QString::fromLocal8Bit( user_data.m_project_create_time.c_str() ) ) );
	}

} // namespace certify
