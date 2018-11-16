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

#ifndef CERTIFY_CERTIFY_PROJECT_LIST_ITEM_DELEGATE_H
#define CERTIFY_CERTIFY_PROJECT_LIST_ITEM_DELEGATE_H

#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <QtGui/QStandardItemModel>
#include <QtCore/QModelIndex>
#include <QtWidgets/QStyledItemDelegate>

namespace certify {

	class ProjectListItemDelegate : public QStyledItemDelegate
	{
		Q_OBJECT

	public:
		ProjectListItemDelegate( QObject* parent = nullptr );
		~ProjectListItemDelegate();

	public:
		void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const override;
		QSize sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const override;
	};

} // namespace certify

#endif // CERTIFY_CERTIFY_PROJECT_LIST_ITEM_DELEGATE_H
