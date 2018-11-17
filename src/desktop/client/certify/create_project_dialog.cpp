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

#include "create_project_dialog.h"

namespace certify {

	CreateProjectDialog::CreateProjectDialog( QWidget* parent )
	: QDialog( parent )
	, m_project_name( "" )
	, m_project_path( "" )
	, m_widget( nullptr )
	, m_label_project_name( nullptr )
	, m_line_edit_project_name( nullptr )
	, m_label_project_path( nullptr )
	, m_line_edit_project_path( nullptr )
	, m_button_browse_folder( nullptr )
	, m_button_box( nullptr )
	, m_layout_h_1( nullptr )
	, m_layout_h_2( nullptr )
	, m_layout_h_3( nullptr )
	, m_layout_v( nullptr )
	, m_phs_spacer_1( nullptr )
	, m_phs_spacer_2( nullptr )
	, m_phs_spacer_3( nullptr )
	, m_phs_spacer_4( nullptr )
	, m_phs_spacer_5( nullptr )
	, m_phs_spacer_6( nullptr )
	, m_pre_folder_path( "C:\\" ) {
		InitInterface();
	}

	CreateProjectDialog::~CreateProjectDialog() {
	}

	void CreateProjectDialog::InitInterface() {
		int wind_size_x = 600;
		int wind_size_y = 100;

		setFixedSize( QSize( wind_size_x, wind_size_y ) );
		setWindowTitle( QString::fromLocal8Bit( "创建项目" ) );

		m_widget = new QWidget( this );
		m_widget->setGeometry( QRect( 0, 0, wind_size_x, wind_size_y ) );

		QFont font_text;
		font_text.setPointSize( 9 );
		font_text.setFamily( "SimSun" );
		setFont( font_text );

		m_label_project_name = new QLabel( QString::fromLocal8Bit( "项目名称" ), m_widget );
		m_label_project_name->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );
		m_line_edit_project_name = new QLineEdit( m_widget );
		m_line_edit_project_name->setFixedWidth( 200 );
		m_line_edit_project_name->setAlignment( Qt::AlignLeft );

		m_label_project_path = new QLabel( QString::fromLocal8Bit( "项目路径" ), m_widget );
		m_label_project_path->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );
		m_line_edit_project_path = new QLineEdit( m_widget );
		m_line_edit_project_path->setFixedWidth( 400 );
		m_line_edit_project_path->setAlignment( Qt::AlignLeft );

		m_button_browse_folder = new QPushButton( QString::fromLocal8Bit( "浏览..." ), m_widget );
		m_button_browse_folder->setFont( font_text );
		m_button_browse_folder->setFixedWidth( 75 );

		m_button_box = new QDialogButtonBox( m_widget );
		m_button_box->setStandardButtons( QDialogButtonBox::Ok );
		m_button_box->button( QDialogButtonBox::Ok )->setText( QString::fromLocal8Bit( "确 定" ) );
		m_button_box->setCenterButtons( true );

		m_layout_h_1 = new QHBoxLayout();
		m_layout_h_1->setContentsMargins( -1, -1, -1, -1 );
		m_phs_spacer_1 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum );
		m_phs_spacer_2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
		m_layout_h_1->addItem( m_phs_spacer_1 );
		m_layout_h_1->addWidget( m_label_project_name );
		m_layout_h_1->addWidget( m_line_edit_project_name );
		m_layout_h_1->addItem( m_phs_spacer_2 );

		m_layout_h_2 = new QHBoxLayout();
		m_layout_h_2->setContentsMargins( -1, -1, -1, -1 );
		m_phs_spacer_3 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum );
		m_phs_spacer_4 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
		m_layout_h_2->addItem( m_phs_spacer_3 );
		m_layout_h_2->addWidget( m_label_project_path );
		m_layout_h_2->addWidget( m_line_edit_project_path );
		m_layout_h_2->addWidget( m_button_browse_folder );
		m_layout_h_2->addItem( m_phs_spacer_4 );

		m_layout_h_3 = new QHBoxLayout();
		m_layout_h_3->setContentsMargins( -1, -1, -1, -1 );
		m_phs_spacer_5 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
		m_phs_spacer_6 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
		m_layout_h_3->addItem( m_phs_spacer_5 );
		m_layout_h_3->addWidget( m_button_box, 1 );
		m_layout_h_3->addItem( m_phs_spacer_6 );

		m_layout_v = new QVBoxLayout();
		m_layout_v->setContentsMargins( -1, -1, -1, -1 );
		m_layout_v->addLayout( m_layout_h_1 );
		m_layout_v->addLayout( m_layout_h_2 );
		m_layout_v->addLayout( m_layout_h_3 );
		setLayout( m_layout_v );

		installEventFilter( this ); // 监视其关闭事件

		QObject::connect( m_button_box, SIGNAL( accepted() ), this, SLOT( HandleAccepted() ) );
		QObject::connect( m_button_box, SIGNAL( rejected() ), this, SLOT( HandleRejected() ) );
		QObject::connect( m_button_browse_folder, SIGNAL( clicked() ), this, SLOT( OnButtonBrowseFolder() ) );
	}

	bool CreateProjectDialog::eventFilter( QObject* target, QEvent* event ) {
		if( event->type() == QEvent::Show ) {
			m_project_name = "";
			m_project_path = "";
			m_line_edit_project_name->setText( "" );
			m_line_edit_project_path->setText( "" );
		}
		if( event->type() == QEvent::Close ) {
		}
		return QDialog::eventFilter( target, event );
	}

	void CreateProjectDialog::HandleAccepted() {
		m_project_name = m_line_edit_project_name->text().toLocal8Bit(); //
		QDialog::accept();
	}

	void CreateProjectDialog::HandleRejected() {
		close();
	}

	void CreateProjectDialog::OnButtonBrowseFolder() {
		QFileDialog* file_dialog = new QFileDialog( this );
		std::string path_temp = file_dialog->getExistingDirectory( this, QString::fromLocal8Bit( "项目文件夹位置" ), QString::fromLocal8Bit( m_pre_folder_path.c_str() ) ).toLocal8Bit();
		if( path_temp != "" ) {
			m_project_path = path_temp; //
			m_pre_folder_path = path_temp; //
			m_line_edit_project_path->setText( QString::fromLocal8Bit( path_temp.c_str() ) );
		}
	}

} // namespace certify
