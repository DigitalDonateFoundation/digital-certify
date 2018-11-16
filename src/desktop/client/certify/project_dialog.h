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
#include <QtWidgets/QDialog>
#include <QtWidgets/QListView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>

#include "project_list_item.h"
#include "project_list_item_delegate.h"

namespace certify {

	class ProjectDialog : public QDialog
	{
		Q_OBJECT

	public:
		ProjectDialog( QWidget* parent = nullptr );
		~ProjectDialog();

	public:
		void InitInterface();

	public:
		void InitData();

	private:
		QListView* m_list_view;
		QStandardItemModel* m_item_model;
		QSortFilterProxyModel* m_proxy_model;
		ProjectListItemDelegate* m_item_delegate;
		QVBoxLayout* m_layout_v;

		//QWidget* m_widget;
		//QTextEdit* m_text_edit;
		//QSpacerItem* m_phs_spacer;
		//QDialogButtonBox* m_button_box;
		//QVBoxLayout* m_layout_v;

		//QPoint m_point;
		//double m_transparent;
		//QTimer* m_timer_show;
		//QTimer* m_timer_stay;
		//QTimer* m_timer_hide;
		//int32_t m_desktop_height;

		//bool m_is_show;
		//std::string m_info_text;
	};

} // namespace certify

#endif // CERTIFY_CERTIFY_PROJECT_DIALOG_H
