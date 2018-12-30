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

#include <QtWidgets/QMenu>
#include <QtWidgets/QDialog>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSplitterHandle>
#include <QtWidgets/QListWidgetItem>
#include <QtWidgets/QStyledItemDelegate>
#include <QtWidgets/QStyleOptionViewItem>

#include "project/project.h"

namespace certify {

	class ProjectDialog : public QDialog {
		Q_OBJECT

	public:
		ProjectDialog( std::string project_gcid, QWidget* parent = nullptr );
		~ProjectDialog();

	public:
		void InitInterface();

	private slots:
		void OnActionListText();
		void OnActionNewArchive();
		void OnShowListMenu( const QPoint& point );
		void OnListItemClicked( QListWidgetItem* list_item );
		void OnListItemDoubleClicked( QListWidgetItem* list_item );

	public:
		QListWidget* m_list_widget;
		QTextEdit* m_text_edit;
		QSplitter* m_splitter_h;
		QVBoxLayout* m_layout_v;

		QMenu* m_menu_list;
		QAction* m_action_new_archive;
		QAction* m_action_list_text;

		std::string m_project_gcid;

	private:
		Project* m_project;
		ProjectItem* m_project_item;
	};

	class ItemDelegate_ProjectDialog : public QStyledItemDelegate { // 使选中的当前行文字颜色不随选中而改变 // 使用 QItemDelegate 会与 resizeColumnsToContents() 冲突导致崩溃
		Q_OBJECT

	public:
		void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const {
			QStyleOptionViewItem view_option( option );
			// 高亮显示与普通显示时的前景色一致（即选中行和未选中时的文字颜色一样）
			view_option.palette.setColor( QPalette::HighlightedText, index.data( Qt::TextColorRole ).value<QColor>() );
			QStyledItemDelegate::paint( painter, view_option, index );
		}
	};

} // namespace certify

#endif // CERTIFY_CERTIFY_PROJECT_DIALOG_H
