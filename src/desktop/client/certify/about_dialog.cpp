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

#include "about_dialog.h"

AboutDialog::AboutDialog( QWidget* parent )
: QDialog( parent ) {
	InitInterface();
}

AboutDialog::~AboutDialog() {
}

void AboutDialog::InitInterface() {
	int nWinSizeX = 390;
	int nWinSizeY = 230;

	setWindowOpacity( 0.85 );
	setFixedSize( QSize( nWinSizeX, nWinSizeY ) );
	setWindowFlags( Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Tool );

	m_widget = new QWidget( this );
	m_widget->setGeometry( QRect( 0, 0, nWinSizeX, nWinSizeY ) );
	m_widget->setStyleSheet( "background-color:white;border:1px;border-style:outset;border-color:rgb(240,130,0);" );

	QFont qFontApp;
	qFontApp.setPointSize( 20 );

	m_label_app_name = new QLabel( QString::fromLocal8Bit( THE_APP_NAME ), m_widget );
	m_label_app_name->setAlignment( Qt::AlignHCenter );
	m_label_app_name->setStyleSheet( "color:rgb(128,0,0);" );
	m_label_app_name->setFont( qFontApp );

	m_label_app_version = new QLabel( QString::fromLocal8Bit( THE_APP_VERSION ), m_widget );
	m_label_app_version->setAlignment( Qt::AlignHCenter );

	m_label_app_icon = new QLabel( m_widget );
	m_label_app_icon->setStyleSheet( "image:url(:/certify/resource/certify.ico);" );

	m_label_app_developer = new QLabel( QString::fromLocal8Bit( THE_APP_DEVELOPER ), m_widget );
	m_label_app_developer->setAlignment( Qt::AlignHCenter );

	m_label_app_company = new QLabel( QString::fromLocal8Bit( THE_APP_COMPANY ), m_widget );
	m_label_app_company->setAlignment( Qt::AlignHCenter );

	m_label_app_copyright = new QLabel( QString::fromLocal8Bit( THE_APP_COPYRIGHT ), m_widget );
	m_label_app_copyright->setAlignment( Qt::AlignHCenter );

	m_button_box = new QDialogButtonBox( m_widget );
	m_button_box->setStandardButtons( QDialogButtonBox::Ok );
	m_button_box->button( QDialogButtonBox::Ok )->setText( QString::fromLocal8Bit( "È· ¶¨" ) );
	m_button_box->setCenterButtons( true );

	m_layout_h_1 = new QHBoxLayout();
	m_layout_h_1->setContentsMargins( -1, 10, -1, 10 );
	m_phs_spacer_1 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	m_phs_spacer_2 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	m_layout_h_1->addItem( m_phs_spacer_1 );
	m_layout_h_1->addWidget( m_label_app_icon, 1 );
	m_layout_h_1->addItem( m_phs_spacer_2 );

	m_layout_h_2 = new QHBoxLayout();
	m_layout_h_2->setContentsMargins( -1, -1, -1, -1 );
	m_layout_h_2->addWidget( m_button_box );

	m_layout_v = new QVBoxLayout();
	m_layout_v->setContentsMargins( 50, 10, 50, 10 );
	m_phs_spacer_3 = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
	m_layout_v->addWidget( m_label_app_name );
	m_layout_v->addWidget( m_label_app_version );
	m_layout_v->addLayout( m_layout_h_1 );
	m_layout_v->addItem( m_phs_spacer_3 );
	m_layout_v->addWidget( m_label_app_developer );
	m_layout_v->addWidget( m_label_app_company );
	m_layout_v->addWidget( m_label_app_copyright );
	m_layout_v->addLayout( m_layout_h_2 );
	setLayout( m_layout_v );

	QObject::connect( m_button_box, SIGNAL( accepted() ), this, SLOT( close() ) );
}
