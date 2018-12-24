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
		//m_list_view->setEditTriggers( QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked ); // 双击或选中单击时显示编辑框
		m_list_view->setEditTriggers( QAbstractItemView::NoEditTriggers ); // 禁用编辑，这样双击才会触发
		m_list_view->setSelectionBehavior( QAbstractItemView::SelectRows ); // 选中整行 // SelectRows、MultiSelection
		m_list_view->setSelectionMode( QAbstractItemView::ExtendedSelection ); // 选择方式
		m_list_view->setContextMenuPolicy( Qt::CustomContextMenu ); // 右键菜单
		//m_list_view->setAlternatingRowColors( true ); // 奇偶行背景色
		m_list_view->setViewMode( QListView::ListMode ); // 设置显示模式
		m_list_view->setDragEnabled( false ); // 不允许拖动
		//m_list_view->setToolTip( QString::fromLocal8Bit( "123" ) );
		m_list_view->setAutoScroll( true );
		m_list_view->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded );
		m_list_view->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
		m_item_model = new QStandardItemModel();
		m_list_view->setModel( m_item_model );
		
		m_menu_list = nullptr; // 菜单选项根据是否选中由 OnShowListMenu() 进行动态显示

		m_layout_v = new QVBoxLayout();
		m_layout_v->setContentsMargins( -1, -1, -1, -1 );
		m_layout_v->addWidget( m_list_view );

		setLayout( m_layout_v );

		m_create_project_dialog = new CreateProjectDialog( this );
		m_create_project_dialog->hide();

		// 列表菜单选项 m_action_new_project 根据是否选中由 OnShowListMenu() 进行动态显示
		// 列表菜单选项 m_action_list_text 根据是否选中由 OnShowListMenu() 进行动态显示
		QObject::connect( m_list_view, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SLOT( OnShowListMenu( const QPoint& ) ) ); // 需要 setContextMenuPolicy( Qt::CustomContextMenu ) 列表
		QObject::connect( m_list_view, SIGNAL( clicked( const QModelIndex& ) ), this, SLOT( OnProjectListViewClicked( const QModelIndex& ) ) ); // 单击列表空白处不会触发
		QObject::connect( m_list_view, SIGNAL( doubleClicked( const QModelIndex& ) ), this, SLOT( OnProjectListItemDoubleClicked( const QModelIndex& ) ) ); // 双击列表空白处不会触发 // 单击可能会与此冲突
	}

	int32_t ProjectListDialog::LoadExistProject() {
		int32_t project_load = m_project->LoadExistProject( m_path_dbf_project );
		if( project_load < 0 ) {
			std::string log_info = "从项目信息文件获取已有项目失败！";
			QMessageBox::warning( this, QString::fromLocal8Bit( "错误" ), QString::fromLocal8Bit( log_info.c_str() ) );
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
			item->setData( DEF_PROJECT_STATUS_CREATE, DEF_USER_ROLE_PROJECT_STATUS ); // 单一存取
			item->setData( QVariant::fromValue( user_data ), DEF_USER_ROLE_PROJECT_USERDATA ); // 整体存取
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
			item->setData( DEF_PROJECT_STATUS_CREATE, DEF_USER_ROLE_PROJECT_STATUS ); // 单一存取
			item->setData( QVariant::fromValue( user_data ), DEF_USER_ROLE_PROJECT_USERDATA ); // 整体存取
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

	void ProjectListDialog::SetCurrentProject( std::string gcid ) { // 使用 QSortFilterProxyModel 会导致 setCurrentIndex、scrollTo 失效
		int32_t row_count = m_item_model->rowCount();
		if( row_count > 0 ) { // 有列表项
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
		QMessageBox::information( this, QString::fromLocal8Bit( "提示" ), QString::fromLocal8Bit( "Text" ) );
	}

	void ProjectListDialog::OnActionNewProject() {
		if( m_create_project_dialog != nullptr ) {
			if( QDialog::Accepted == m_create_project_dialog->exec() ) {
				std::string project_name = m_create_project_dialog->m_project_name;
				std::string project_path = m_create_project_dialog->m_project_path;
				if( "" == project_name ) {
					QMessageBox::information( this, QString::fromLocal8Bit( "提示" ), QString::fromLocal8Bit( "新建项目 名称 为空！" ) );
				}
				else if( "" == project_path ) {
					QMessageBox::information( this, QString::fromLocal8Bit( "提示" ), QString::fromLocal8Bit( "新建项目 路径 为空！" ) );
				}
				else if( false == CanCreateProject( project_name, project_path ) ) {
					QMessageBox::information( this, QString::fromLocal8Bit( "提示" ), QString::fromLocal8Bit( "项目名称 或 项目路径 已经存在，无法新建项目！" ) );
				}
				else {
					if( false == CreateProject( project_name, project_path ) ) {
						QMessageBox::information( this, QString::fromLocal8Bit( "提示" ), QString::fromLocal8Bit( "新建项目失败！" ) );
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

		QModelIndexList list_model_index = m_list_view->selectionModel()->selectedIndexes(); // 单行或多行
		if( list_model_index.size() > 0 ) { // 选中行
			m_action_list_text = m_menu_list->addAction( QString::fromLocal8Bit( "选中" ) );
			m_action_list_text->setIcon( QIcon( ":/certify/resource/certify.ico" ) );
			QObject::connect( m_action_list_text, SIGNAL( triggered() ), this, SLOT( OnActionListText() ) );
		}
		else { // 未选中
			m_action_new_project = m_menu_list->addAction( QString::fromLocal8Bit( "新建" ) );
			m_action_new_project->setIcon( QIcon( ":/certify/resource/action_new_project.ico" ) );
			QObject::connect( m_action_new_project, SIGNAL( triggered() ), this, SLOT( OnActionNewProject() ) );
		}

		int32_t row_count = m_item_model->rowCount();
		if( row_count > 0 ) { // 有列表项
			m_action_list_text = m_menu_list->addAction( QString::fromLocal8Bit( "文本" ) );
			m_action_list_text->setIcon( QIcon( ":/certify/resource/certify.ico" ) );
			QObject::connect( m_action_list_text, SIGNAL( triggered() ), this, SLOT( OnActionListText() ) );
		}

		m_menu_list->exec( QCursor::pos() );
	}

	void ProjectListDialog::OnProjectListViewClicked( const QModelIndex& index ) {
		QModelIndexList list_model_index = m_list_view->selectionModel()->selectedIndexes(); // 单行或多行
		if( list_model_index.size() > 0 ) { // 选中行
			for( int32_t i = 0; i < list_model_index.size(); ++i ) { // 只有一个
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
		//QMessageBox::information( this, QString::fromLocal8Bit( "提示" ), QString( "DoubleClicked: %1 %2 %3 %4" ).arg( index.row() ).arg( status ).arg( QString::fromLocal8Bit( user_data.m_project_name.c_str() ) ).arg( QString::fromLocal8Bit( user_data.m_project_create_time.c_str() ) ) );
	}

} // namespace certify
