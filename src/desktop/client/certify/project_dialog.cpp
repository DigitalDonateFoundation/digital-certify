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
		: QDialog( parent ) {
		InitInterface();
	}

	ProjectDialog::~ProjectDialog() {
	}

	void ProjectDialog::InitInterface() {
		InitData();

		m_list_view = new QListView( this );
		m_item_delegate = new ProjectListItemDelegate( this );
		m_list_view->setItemDelegate( m_item_delegate );
		m_list_view->setSpacing( 15 );
		m_proxy_model = new QSortFilterProxyModel( m_list_view );
		m_proxy_model->setSourceModel( m_item_model );
		m_proxy_model->setFilterRole( Qt::UserRole );
		m_proxy_model->setDynamicSortFilter( true );
		m_list_view->setModel( m_proxy_model ); // 为委托设置模型
		m_list_view->setViewMode( QListView::ListMode ); // 设置图标显示
		m_list_view->setDragEnabled( true ); // 不允许拖动

		m_layout_v = new QVBoxLayout();
		m_layout_v->setContentsMargins( 0, 0, 0, 0 );
		m_layout_v->addWidget( m_list_view );

		setLayout( m_layout_v );
	}

	void ProjectDialog::InitData() {
		m_item_model = new QStandardItemModel();
		for( int32_t i = 0; i < 10; ++i ) {
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
			item->setData( item_status, Qt::UserRole ); // 单一存取
			item->setData( QVariant::fromValue( item_data ), Qt::UserRole + 1 ); // 整体存取
			m_item_model->appendRow( item );
		}
	}

} // namespace certify
