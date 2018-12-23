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
	, m_text_edit( nullptr )
	, m_layout_v( nullptr )
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

		m_text_edit = new QTextEdit( this );

		m_layout_v = new QVBoxLayout();
		m_layout_v->setContentsMargins( -1, -1, -1, -1 );
		m_layout_v->addWidget( m_text_edit );

		setLayout( m_layout_v );

		std::string show_text;
		FormatLibrary::StandardLibrary::FormatTo( show_text, "{0}\n\n{1}\n\n{2}\n\n{3}\n\n{4}", 
			m_project_item->GetName(), m_project_item->GetCreateTime(), m_project_item->GetGCID(), m_project_item->GetPath(), m_project_item->GetHome() );
		m_text_edit->setText( QString::fromLocal8Bit( show_text.c_str() ) );
	}

} // namespace certify
