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
	, m_layout_v( nullptr ) {
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
		InitData();
		m_proxy_model = new QSortFilterProxyModel( m_list_view );
		m_proxy_model->setSourceModel( m_item_model );
		m_proxy_model->setFilterRole( Qt::UserRole );
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
			ItemData item_data;
			item_data.m_name = QString( "Name: %1" ).arg( i );
			item_data.m_phone = QString( "Phone: 1331234567%1" ).arg( i );
			int32_t rand_number = rand() % 3;
			ItemStatus item_status;
			switch( rand_number ) {
			case 0:
				item_status = S_R;
				break;
			case 1:
				item_status = S_G;
				break;
			case 2:
				item_status = S_B;
				break;
			}
			item->setData( item_status, Qt::UserRole ); // ��һ��ȡ
			item->setData( QVariant::fromValue( item_data ), Qt::UserRole + 1 ); // �����ȡ
			item->setToolTip( QString( "%1 1331234567%2" ).arg( i ).arg( i ) );
			m_item_model->appendRow( item );
		}
	}

	void ProjectDialog::CreateProject( std::string project_name, std::string project_path ) {
		bool is_exist = false;
		auto it_pn = m_map_project_name.find( project_name );
		if( it_pn != m_map_project_name.end() ) { // �Ѵ���
			is_exist = true;
		}
		auto it_pp = m_map_project_path.find( project_path );
		if( it_pp != m_map_project_path.end() ) { // �Ѵ���
			is_exist = true;
		}
		if( false == is_exist ) {
			m_map_project_name[project_name] = project_name;
			m_map_project_path[project_path] = project_path;
			
			QStandardItem* item = new QStandardItem;
			ItemData item_data;
			item_data.m_name = QString::fromLocal8Bit( project_name.c_str() );
			item_data.m_phone = QString::fromLocal8Bit( project_path.c_str() );
			int32_t rand_number = rand() % 3;
			ItemStatus item_status;
			switch( rand_number ) {
			case 0:
				item_status = S_R;
				break;
			case 1:
				item_status = S_G;
				break;
			case 2:
				item_status = S_B;
				break;
			}
			item->setData( item_status, Qt::UserRole ); // ��һ��ȡ
			item->setData( QVariant::fromValue( item_data ), Qt::UserRole + 1 ); // �����ȡ
			item->setToolTip( QString::fromLocal8Bit( project_path.c_str() ) );
			m_item_model->appendRow( item );
		}
	}

	bool ProjectDialog::CanCreateProject( std::string project_name, std::string project_path ) {
		bool is_exist = false;
		auto it_pn = m_map_project_name.find( project_name );
		if( it_pn != m_map_project_name.end() ) { // �Ѵ���
			is_exist = true;
		}
		auto it_pp = m_map_project_path.find( project_path );
		if( it_pp != m_map_project_path.end() ) { // �Ѵ���
			is_exist = true;
		}
		return !is_exist;
	}

	void ProjectDialog::OnActionListTest() {
		QMessageBox::information( this, QString::fromLocal8Bit( "��ʾ" ), QString::fromLocal8Bit( "Test" ) );
	}

	void ProjectDialog::OnActionListText() {
		QMessageBox::information( this, QString::fromLocal8Bit( "��ʾ" ), QString::fromLocal8Bit( "Text" ) );

		m_proxy_model->setFilterFixedString( QString::number( ItemStatus::S_B ) ); // ���ԣ�����ʾ��ɫ�� // setFilterFixedString( QString() ) ��ԭ
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

			m_proxy_model->setFilterFixedString( QString::number( ItemStatus::S_R ) ); // ���ԣ�����ʾ��ɫ�� // setFilterFixedString( QString() ) ��ԭ
		}
		else { // δѡ��
			m_action_list_test = m_menu_list->addAction( QString::fromLocal8Bit( "δѡ" ) );
			m_action_list_test->setIcon( QIcon( ":/certify/resource/certify.ico" ) );
			QObject::connect( m_action_list_test, SIGNAL( triggered() ), this, SLOT( OnActionListTest() ) );

			m_proxy_model->setFilterFixedString( QString::number( ItemStatus::S_G ) ); // ���ԣ�����ʾ��ɫ�� // setFilterFixedString( QString() ) ��ԭ
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
		ItemData data = index.data( Qt::UserRole + 1 ).value<ItemData>();
		ItemStatus status = (ItemStatus)( index.data( Qt::UserRole ).toInt() );
		QMessageBox::information( this, QString::fromLocal8Bit( "��ʾ" ), QString( "DoubleClicked: %1 %2 %3 %4" ).arg( index.row() ).arg( status ).arg( data.m_name ).arg( data.m_phone ) );

		// ��ʹ�ô���ģ�ͺ����ڿ����˶�̬����ģʽ������޸Ĵ���ģ�͵����ݣ��ڵ�һ��item�޸����ݺ���ܾͲ��ڵ�ǰ����ģ���У��ᱻ���˵��������item��QModelIndex�ͻ�仯�����º������޸�ʧ�ܡ�
		// �����������ɴ���һ�ǲ��޸Ĵ���ģ�Ͷ���ֱ���޸�Դģ�͵����ݣ��������޸�ģ�����ݵ�ʱ��رմ���ģ�͵Ķ�̬�������޸����ٿ�����
		// m_proxy_model->setDynamicSortFilter( false );
		// m_proxy_model->setDynamicSortFilter( true );
		QModelIndex index_test = m_proxy_model->mapToSource( index ); // �����Ȼ�ȡ Դ Model �� ModelIndex
		m_item_model->setData( index_test, ItemStatus::S_R, Qt::UserRole ); // ���ԣ���ɺ�ɫ
	}

} // namespace certify
