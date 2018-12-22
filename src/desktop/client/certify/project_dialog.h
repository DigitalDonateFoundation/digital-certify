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

#ifndef CERTIFY_CERTIFY_PROJECT_DIALOG_H
#define CERTIFY_CERTIFY_PROJECT_DIALOG_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>

#include "project/project.h"

namespace certify {

	class ProjectDialog : public QDialog {
		Q_OBJECT

	public:
		ProjectDialog( std::string project_gcid, QWidget* parent = nullptr );
		~ProjectDialog();

	public:
		void InitInterface();

	public:
		QTextEdit* m_text_edit;
		QVBoxLayout* m_layout_v;

		std::string m_project_gcid;

	private:
		Project* m_project;
		ProjectItem* m_project_item;
	};

} // namespace certify

#endif // CERTIFY_CERTIFY_PROJECT_DIALOG_H
