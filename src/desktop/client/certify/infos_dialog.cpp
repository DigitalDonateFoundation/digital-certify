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

#include "infos_dialog.h"

namespace certify {

	InfosDialog::InfosDialog( QWidget* parent )
	: QDialog( parent )
	, m_widget( nullptr )
	, m_text_edit( nullptr )
	, m_phs_spacer( nullptr )
	, m_button_box( nullptr )
	, m_layout_v( nullptr )
	, m_transparent( 0.0 )
	, m_timer_show( nullptr )
	, m_timer_stay( nullptr )
	, m_timer_hide( nullptr )
	, m_desktop_height( 0 )
	, m_is_show( false )
	, m_info_text( "" ) {
		InitInterface();
	}

	InfosDialog::~InfosDialog() {
	}

	void InfosDialog::InitInterface() {
		int32_t wind_size_x = 395;
		int32_t wind_size_y = 90; // ø…“‘»›ƒ…¡Ω––Œƒ◊÷
		//int32_t wind_size_y = 110; // ø…“‘»›ƒ…»˝––Œƒ◊÷

		setFixedSize( QSize( wind_size_x, wind_size_y ) );
		setWindowFlags( Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Tool );

		m_widget = new QWidget( this );
		m_widget->setGeometry( QRect( 0, 0, wind_size_x, wind_size_y ) );
		m_widget->setStyleSheet( "background-color:white;border:1px;border-style:outset;border-color:rgb(240,130,0);" );

		QFont font_text;
		font_text.setPointSize( 9 );
		font_text.setFamily( "SimSun" );

		m_text_edit = new QTextEdit( this );
		m_text_edit->setFont( font_text );
		m_text_edit->setReadOnly( true );
		m_text_edit->setLineWrapMode( QTextEdit::LineWrapMode::WidgetWidth );
		//m_text_edit->setAlignment( Qt::AlignCenter ); // √ª–ßπ˚
		m_text_edit->setStyleSheet( "border:0px;color:rgb(128,0,0);" );
		m_text_edit->document()->setDefaultTextOption( QTextOption( Qt::AlignCenter ) ); // ÷ªª·ÀÆ∆Ωæ”÷–

		m_button_box = new QDialogButtonBox( m_widget );
		m_button_box->setStandardButtons( QDialogButtonBox::Ok );
		m_button_box->button( QDialogButtonBox::Ok )->setText( QString::fromLocal8Bit( "“—  ‘ƒ" ) );
		m_button_box->setCenterButtons( true );

		m_layout_v = new QVBoxLayout();
		m_layout_v->setContentsMargins( -1, -1, -1, -1 );
		m_phs_spacer = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
		m_layout_v->addWidget( m_text_edit );
		m_layout_v->addItem( m_phs_spacer );
		m_layout_v->addWidget( m_button_box );
		setLayout( m_layout_v );

		m_timer_show = new QTimer( this );
		m_timer_stay = new QTimer( this );
		m_timer_hide = new QTimer( this );

		QObject::connect( m_button_box, SIGNAL( accepted() ), this, SLOT( OnButtonClose() ) );

		QObject::connect( m_timer_show, SIGNAL( timeout() ), this, SLOT( OnTimerShow() ) );
		QObject::connect( m_timer_stay, SIGNAL( timeout() ), this, SLOT( OnTimerStay() ) );
		QObject::connect( m_timer_hide, SIGNAL( timeout() ), this, SLOT( OnTimerHide() ) );
	}

	void InfosDialog::ShowMessage() {
		if( true == m_is_show ) {
			OnButtonClose();
		}

		QRect rect_desktop = QApplication::desktop()->availableGeometry();
		m_desktop_height = rect_desktop.height();
		m_point.setX( rect_desktop.width() - width() );
		m_point.setY( rect_desktop.height() - height() );

		move( m_point.x(), m_desktop_height );

		m_text_edit->clear();
		m_text_edit->append( QString::fromLocal8Bit( m_info_text.c_str() ) ); // ÷–Œƒ
	
		m_transparent = 0.9; // Õ∏√˜∂»
		setWindowOpacity( m_transparent );

		show();

		m_is_show = true;

		m_timer_show->start( 10 );
	}

	void InfosDialog::SetDialogInfos( std::string info_text ) {
		m_info_text = info_text;
		//m_info_text = "¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¡À¡À¿¥¡À¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¡À¡À¿¥¡À";
		//m_info_text = "¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¡À¡À¿¥¡À¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¡À¡À¿¥¡À¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿≤¿¥¡À";
	}

	void InfosDialog::OnTimerShow() {
		m_desktop_height--;
		move( m_point.x(), m_desktop_height );
		if( m_desktop_height <= m_point.y() ) {
			m_timer_show->stop();
			m_timer_stay->start( 8000 );
		}
	}

	void InfosDialog::OnTimerStay() {
		m_timer_stay->stop();
		m_timer_hide->start( 100 );
	}

	void InfosDialog::OnTimerHide() {
		m_transparent -= 0.05;
		if( m_transparent <= 0.0 ) {
			m_timer_hide->stop();
			m_is_show = false;
			close();
		}
		else {
			setWindowOpacity( m_transparent );
		}
	}

	void InfosDialog::OnButtonClose() {
		m_timer_show->stop();
		m_timer_stay->stop();
		m_timer_hide->stop();
		m_is_show = false;
		close();
	}

} // namespace certify
