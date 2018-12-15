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

#include <QtCore/QSortFilterProxyModel>
#include <QtWidgets/QMenu>
#include <QtWidgets/QDialog>
#include <QtWidgets/QListView>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>

#include "create_project_dialog.h"
#include "project_list_item_delegate.h"

namespace certify {

	class ProjectDialog : public QDialog {
		Q_OBJECT

	public:
		ProjectDialog( QWidget* parent = nullptr );
		~ProjectDialog();

	public:
		void InitInterface();

	public:
		int32_t LoadExistProject();
		bool CreateProject( std::string name, std::string path );
		bool CanCreateProject( std::string name, std::string path );

	public slots:
		void OnActionNewProject();

	private slots:
		void OnActionListText();
		void OnShowListMenu( const QPoint& point );
		void OnProjectListItemDoubleClicked( const QModelIndex& index );

	private:
		QListView* m_list_view;
		QStandardItemModel* m_item_model;
		QSortFilterProxyModel* m_proxy_model;
		QMenu* m_menu_list;
		QAction* m_action_new_project;
		QAction* m_action_list_text;
		QVBoxLayout* m_layout_v;
		CreateProjectDialog* m_create_project_dialog;

	private:
		Project* m_project;
		std::string m_path_app_folder;
		std::string m_path_cfg_folder;
		std::string m_path_dbf_project;
	};

} // namespace certify

#endif // CERTIFY_CERTIFY_PROJECT_DIALOG_H
