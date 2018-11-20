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

#ifndef CERTIFY_CERTIFY_CREATE_PROJECT_DIALOG_H
#define CERTIFY_CERTIFY_CREATE_PROJECT_DIALOG_H

#include <QtGui/QCloseEvent>
#include <QtWidgets/QLabel>
#include <QtWidgets/QDialog>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QDialogButtonBox>

namespace certify {

	class CreateProjectDialog : public QDialog {
		Q_OBJECT

	public:
		CreateProjectDialog( QWidget* parent = nullptr );
		~CreateProjectDialog();

	public:
		void InitInterface();

	private:
		bool eventFilter( QObject* target, QEvent* event ) override;

	private slots:
		void HandleAccepted();
		void HandleRejected();
		void OnButtonBrowseFolder();

	public:
		std::string m_project_name;
		std::string m_project_path;

	private:
		QWidget* m_widget;
		QLabel* m_label_project_name;
		QLineEdit* m_line_edit_project_name;
		QLabel* m_label_project_path;
		QLineEdit* m_line_edit_project_path;
		QPushButton* m_button_browse_folder;
		QDialogButtonBox* m_button_box;
		QHBoxLayout* m_layout_h_1;
		QHBoxLayout* m_layout_h_2;
		QHBoxLayout* m_layout_h_3;
		QVBoxLayout* m_layout_v;
		QSpacerItem* m_phs_spacer_1;
		QSpacerItem* m_phs_spacer_2;
		QSpacerItem* m_phs_spacer_3;
		QSpacerItem* m_phs_spacer_4;
		QSpacerItem* m_phs_spacer_5;
		QSpacerItem* m_phs_spacer_6;

		std::string m_pre_folder_path;
	};

} // namespace certify

#endif // CERTIFY_CERTIFY_CREATE_PROJECT_DIALOG_H
