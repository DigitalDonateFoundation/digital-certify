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
		m_project_item = m_project->GetProject( project_gcid ); // Ӧ�ÿ϶�����
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

		setAttribute( Qt::WA_DeleteOnClose ); // ���ڵ�� MdiSubWindow �� Close ��ťʱɾ�� MdiSubWindow ������ Widget ����
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
		m_list_widget->setItemDelegate( new ItemDelegate_ProjectDialog ); // ʹѡ�еĵ�ǰ��������ɫ����ѡ�ж��ı�
		//m_list_widget->setEditTriggers( QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked ); // ˫����ѡ�е���ʱ��ʾ�༭��
		m_list_widget->setEditTriggers( QAbstractItemView::NoEditTriggers ); // ���ñ༭������˫���Żᴥ��
		m_list_widget->setSelectionBehavior( QAbstractItemView::SelectRows ); // ѡ������ // SelectRows��MultiSelection
		m_list_widget->setSelectionMode( QAbstractItemView::SingleSelection ); // ѡ��ʽ // SingleSelection �� ExtendedSelection ģʽ��Ӱ��ѡ��������Ӱ�쵯���˵����б������
		m_list_widget->setContextMenuPolicy( Qt::CustomContextMenu ); // �Ҽ��˵�
		m_list_widget->setAlternatingRowColors( true ); // ��ż�б���ɫ
		m_list_widget->setViewMode( QListView::ListMode ); // ��ʾģʽ��ListMode��IconMode
		m_list_widget->setDragEnabled( false ); // �������϶�
		//m_list_widget->setToolTip( QString::fromLocal8Bit( "123" ) );
		m_list_widget->setAutoScroll( true );
		m_list_widget->horizontalScrollBar()->setDisabled( true );
		m_list_widget->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded );
		m_list_widget->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
		m_list_widget->setStyleSheet( "QListWidget{outline:0px;}" // ȥ��ѡ������ֵ����߿�
			"QListWidget::Item{border:1px solid transparent;}"
			"QListWidget::Item:hover{background-color:rgb(229,241,255);}"
			"QListWidget::item:focus{background-color:rgb(229,241,255); border:1px solid rgb(0,128,255);}"
			"QListWidget::item:selected{background-color:rgb(229,241,255); border:1px solid rgb(0,128,255);}"
			"QListWidget::item:selected:!active{background-color:rgb(229,241,255); border:1px solid rgb(0,128,255);}" // Ŀǰ���ı���ɫ
		);

		m_text_edit = new QTextEdit( this );

		m_splitter_h = new QSplitter( Qt::Horizontal ); // ������
		//m_splitter_h->setHandleWidth( 1 );
		m_splitter_h->addWidget( m_list_widget ); // 0
		m_splitter_h->addWidget( m_text_edit ); // 1
		m_splitter_h->setStretchFactor( 0, 1 );
		m_splitter_h->setStretchFactor( 1, 4 );
	    //m_splitter_h->setOpaqueResize( false ); // �϶�ʱ�Ƿ�ʵʱ���£�Ĭ��Ϊ true
	    //m_splitter_h->setStyleSheet( "QSplitter::handle { background-color: black }" );
		// ����кܶ����ֽ��ߣ�ϣ��ֻ�޸ĵ�һ����ע�ⲻ�Ǵ� 0 ��ʼ����������Ҳ����ֽ��ߣ������� NULL ֵ����ʵ handle( 0 ) �Ǵ��ڵģ������޷�������д���
		//QSplitterHandle* splitter_handle = m_splitter_h->handle( 1 );
		//if( splitter_handle ) {
		//	splitter_handle->setFixedWidth( 10 ); // ���÷ֽ��߿��
		//	splitter_handle->setDisabled( true ); // ��ֹ�϶��ֽ���
		//}

		m_menu_list = nullptr; // �˵�ѡ������Ƿ�ѡ���� OnShowListMenu() ���ж�̬��ʾ

		m_layout_v = new QVBoxLayout();
		m_layout_v->setContentsMargins( -1, -1, -1, -1 );
		m_layout_v->addWidget( m_splitter_h );

		setLayout( m_layout_v );

		// �б�˵�ѡ�� m_action_new_archive �����Ƿ�ѡ���� OnShowListMenu() ���ж�̬��ʾ
		// �б�˵�ѡ�� m_action_list_text �����Ƿ�ѡ���� OnShowListMenu() ���ж�̬��ʾ
		QObject::connect( m_list_widget, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SLOT( OnShowListMenu( const QPoint& ) ) ); // ��Ҫ setContextMenuPolicy( Qt::CustomContextMenu ) �б�
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
		QMessageBox::information( this, QString::fromLocal8Bit( "��ʾ" ), QString::fromLocal8Bit( "Text" ) );
	}

	void ProjectDialog::OnActionNewArchive() {
		QMessageBox::information( this, QString::fromLocal8Bit( "��ʾ" ), QString::fromLocal8Bit( "News" ) );
	}

	void ProjectDialog::OnShowListMenu( const QPoint& point ) {
		if( m_menu_list != nullptr ) {
			delete m_menu_list;
			m_menu_list = nullptr;
		}

		m_menu_list = new QMenu( m_list_widget );

		// setSelectionMode Ϊ ExtendedSelection ʱ�� QModelIndexList list_model_index = m_list_widget->selectionModel()->selectedIndexes();
		QModelIndex index = m_list_widget->indexAt( point );
		if( index.row() >= 0 ) { // ѡ���� // if( list_model_index.size() > 0 )
			m_action_list_text = m_menu_list->addAction( QString::fromLocal8Bit( "ѡ��" ) );
			m_action_list_text->setIcon( QIcon( ":/certify/resource/certify.ico" ) );
			QObject::connect( m_action_list_text, SIGNAL( triggered() ), this, SLOT( OnActionListText() ) );
		}
		else { // δѡ��
			m_action_new_archive = m_menu_list->addAction( QString::fromLocal8Bit( "�½�" ) );
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
