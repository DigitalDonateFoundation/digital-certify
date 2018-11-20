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
		void InitData();
		bool CreateProject( std::string name, std::string path );
		bool CanCreateProject( std::string name, std::string path );

	private slots:
		void OnActionListTest();
		void OnActionListText();
		void OnShowListMenu( const QPoint& point );
		void OnProjectListItemDoubleClicked( const QModelIndex& index );

	private:
		QListView* m_list_view;
		QStandardItemModel* m_item_model;
		QSortFilterProxyModel* m_proxy_model;
		QMenu* m_menu_list;
		QAction* m_action_list_test;
		QAction* m_action_list_text;
		QVBoxLayout* m_layout_v;

	private:
		Project* m_project;
	};

} // namespace certify

#endif // CERTIFY_CERTIFY_PROJECT_DIALOG_H
