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

#include <common/common.h>

#include "project_dialog.h"

namespace certify {

	ProjectDialog::ProjectDialog( std::string project_gcid, QWidget* parent )
	: QDialog( parent )
	, m_list_widget( nullptr )
	, m_text_edit( nullptr )
	, m_splitter_h( nullptr )
	, m_layout_v( nullptr )
	, m_menu_list( nullptr )
	, m_action_new_archive( nullptr )
	, m_action_list_text( nullptr )
	, m_project( nullptr ) {
		m_project = Project::GetInstance();
		m_project_item = m_project->GetProject( project_gcid ); // 应该肯定存在
		m_project_gcid = m_project_item->GetGCID();

		InitInterface();
	}

	ProjectDialog::~ProjectDialog() {
	}

	void ProjectDialog::InitInterface() {
		QFont font_text;
		font_text.setPointSize( 9 );
		font_text.setFamily( "SimSun" );
		setFont( font_text );

		setAttribute( Qt::WA_DeleteOnClose ); // 将在点击 MdiSubWindow 的 Close 按钮时删除 MdiSubWindow 和所含 Widget 对象
		setContentsMargins( -1, -1, -1, -1 );
		setWindowTitle( QString::fromLocal8Bit( m_project_item->GetName().c_str() ) );

		m_list_widget = new QListWidget( this );
		//m_list_widget->setSpacing( 5 );
		m_list_widget->setFont( font_text );
		m_list_widget->setMinimumWidth( 130 );
		m_list_widget->setMaximumWidth( 180 );
		m_list_widget->setMovement( QListView::Static );
		m_list_widget->setResizeMode( QListView::Adjust );
		//m_list_widget->setFrameShape( QListWidget::NoFrame );
		m_list_widget->setItemDelegate( new ItemDelegate_ProjectDialog ); // 使选中的当前行文字颜色不随选中而改变
		//m_list_widget->setEditTriggers( QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked ); // 双击或选中单击时显示编辑框
		m_list_widget->setEditTriggers( QAbstractItemView::NoEditTriggers ); // 禁用编辑，这样双击才会触发
		m_list_widget->setSelectionBehavior( QAbstractItemView::SelectRows ); // 选中整行 // SelectRows、MultiSelection
		m_list_widget->setSelectionMode( QAbstractItemView::SingleSelection ); // 选择方式 // SingleSelection 和 ExtendedSelection 模式会影响选中项焦点进而影响弹出菜单和列表项绘制
		m_list_widget->setContextMenuPolicy( Qt::CustomContextMenu ); // 右键菜单
		m_list_widget->setAlternatingRowColors( true ); // 奇偶行背景色
		m_list_widget->setViewMode( QListView::ListMode ); // 显示模式，ListMode、IconMode
		m_list_widget->setDragEnabled( false ); // 不允许拖动
		//m_list_widget->setToolTip( QString::fromLocal8Bit( "123" ) );
		m_list_widget->setAutoScroll( true );
		m_list_widget->horizontalScrollBar()->setDisabled( true );
		m_list_widget->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded );
		m_list_widget->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
		m_list_widget->setStyleSheet( "QListWidget{outline:0px;}" // 去除选中项出现的虚线框
			"QListWidget::Item{border:1px solid transparent;}"
			"QListWidget::Item:hover{background-color:rgb(229,241,255);}"
			"QListWidget::item:focus{background-color:rgb(229,241,255); border:1px solid rgb(0,128,255);}"
			"QListWidget::item:selected{background-color:rgb(229,241,255); border:1px solid rgb(0,128,255);}"
			"QListWidget::item:selected:!active{background-color:rgb(229,241,255); border:1px solid rgb(0,128,255);}" // 目前不改变颜色
		);

		m_text_edit = new QTextEdit( this );

		m_splitter_h = new QSplitter( Qt::Horizontal ); // 从左到右
		//m_splitter_h->setHandleWidth( 1 );
		m_splitter_h->addWidget( m_list_widget ); // 0
		m_splitter_h->addWidget( m_text_edit ); // 1
		m_splitter_h->setStretchFactor( 0, 1 );
		m_splitter_h->setStretchFactor( 1, 4 );
	    //m_splitter_h->setOpaqueResize( false ); // 拖动时是否实时更新，默认为 true
	    //m_splitter_h->setStyleSheet( "QSplitter::handle { background-color: black }" );
		// 如果有很多条分界线，希望只修改第一条，注意不是从 0 开始索引，如果找不到分界线，将返回 NULL 值，其实 handle( 0 ) 是存在的，但是无法对其进行处理
		//QSplitterHandle* splitter_handle = m_splitter_h->handle( 1 );
		//if( splitter_handle ) {
		//	splitter_handle->setFixedWidth( 10 ); // 设置分界线宽度
		//	splitter_handle->setDisabled( true ); // 禁止拖动分界线
		//}

		m_menu_list = nullptr; // 菜单选项根据是否选中由 OnShowListMenu() 进行动态显示

		m_layout_v = new QVBoxLayout();
		m_layout_v->setContentsMargins( -1, -1, -1, -1 );
		m_layout_v->addWidget( m_splitter_h );

		setLayout( m_layout_v );

		// 列表菜单选项 m_action_new_archive 根据是否选中由 OnShowListMenu() 进行动态显示
		// 列表菜单选项 m_action_list_text 根据是否选中由 OnShowListMenu() 进行动态显示
		QObject::connect( m_list_widget, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SLOT( OnShowListMenu( const QPoint& ) ) ); // 需要 setContextMenuPolicy( Qt::CustomContextMenu ) 列表
		QObject::connect( m_list_widget, SIGNAL( itemClicked( QListWidgetItem* ) ), this, SLOT( OnListItemClicked( QListWidgetItem* ) ) );
		QObject::connect( m_list_widget, SIGNAL( itemDoubleClicked( QListWidgetItem* ) ), this, SLOT( OnListItemDoubleClicked( QListWidgetItem* ) ) );

		QListWidgetItem* list_item_01 = new QListWidgetItem();
		list_item_01->setText( "2018-12-28 15:12:25" );
		list_item_01->setTextAlignment( Qt::AlignCenter );
		list_item_01->setIcon( QIcon( ":/certify/resource/archive_his.ico" ) );
		m_list_widget->addItem( list_item_01 );
		QListWidgetItem* list_item_02 = new QListWidgetItem();
		list_item_02->setText( "2018-12-29 15:12:25" );
		list_item_02->setTextAlignment( Qt::AlignCenter );
		list_item_02->setIcon( QIcon( ":/certify/resource/archive_his.ico" ) );
		m_list_widget->addItem( list_item_02 );
		QListWidgetItem* list_item_03 = new QListWidgetItem();
		list_item_03->setText( "2018-12-30 15:12:25" );
		list_item_03->setTextAlignment( Qt::AlignCenter );
		list_item_03->setIcon( QIcon( ":/certify/resource/archive_his.ico" ) );
		m_list_widget->addItem( list_item_03 );
		QListWidgetItem* list_item_04 = new QListWidgetItem();
		list_item_04->setText( "2018-12-31 15:12:25" );
		list_item_04->setTextAlignment( Qt::AlignCenter );
		list_item_04->setIcon( QIcon( ":/certify/resource/archive_his.ico" ) );
		m_list_widget->addItem( list_item_04 );

		m_list_widget->setCurrentRow( 0 );

		std::string show_text;
		FormatLibrary::StandardLibrary::FormatTo( show_text, "{0}\n\n{1}\n\n{2}\n\n{3}\n\n{4}", 
			m_project_item->GetName(), m_project_item->GetCreateTime(), m_project_item->GetGCID(), m_project_item->GetPath(), m_project_item->GetHome() );
		m_text_edit->setText( QString::fromLocal8Bit( show_text.c_str() ) );
	}

	void ProjectDialog::OnActionListText() {
		QMessageBox::information( this, QString::fromLocal8Bit( "提示" ), QString::fromLocal8Bit( "Text" ) );
	}

	void ProjectDialog::OnActionNewArchive() {
		QMessageBox::information( this, QString::fromLocal8Bit( "提示" ), QString::fromLocal8Bit( "News" ) );
	}

	void ProjectDialog::OnShowListMenu( const QPoint& point ) {
		if( m_menu_list != nullptr ) {
			delete m_menu_list;
			m_menu_list = nullptr;
		}

		m_menu_list = new QMenu( m_list_widget );

		// setSelectionMode 为 ExtendedSelection 时用 QModelIndexList list_model_index = m_list_widget->selectionModel()->selectedIndexes();
		QModelIndex index = m_list_widget->indexAt( point );
		if( index.row() >= 0 ) { // 选中行 // if( list_model_index.size() > 0 )
			m_action_list_text = m_menu_list->addAction( QString::fromLocal8Bit( "选中" ) );
			m_action_list_text->setIcon( QIcon( ":/certify/resource/certify.ico" ) );
			QObject::connect( m_action_list_text, SIGNAL( triggered() ), this, SLOT( OnActionListText() ) );
		}
		else { // 未选中
			m_action_new_archive = m_menu_list->addAction( QString::fromLocal8Bit( "新建" ) );
			m_action_new_archive->setIcon( QIcon( ":/certify/resource/archive_new.ico" ) );
			QObject::connect( m_action_new_archive, SIGNAL( triggered() ), this, SLOT( OnActionNewArchive() ) );
		}

		m_menu_list->exec( QCursor::pos() );
	}

	void ProjectDialog::OnListItemClicked( QListWidgetItem* list_item ) {

	}

	void ProjectDialog::OnListItemDoubleClicked( QListWidgetItem* list_item ) {

	}

} // namespace certify
