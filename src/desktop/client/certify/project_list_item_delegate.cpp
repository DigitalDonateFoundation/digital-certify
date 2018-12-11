﻿/*
* Copyright (c) 2018-2019 the Certify authors
* All rights reserved.
*
* The project sponsor and lead author is Xu Rendong.
* E-mail: xrd@ustc.edu, QQ: 277195007, WeChat: ustc_xrd
* See the contributors file for names of other contributors.
*
* Other copyright notice and conditions to be added...
*/

#include "project_list_item_delegate.h"

namespace certify {

	ProjectListItemDelegate::ProjectListItemDelegate( QObject* parent )
	: QStyledItemDelegate( parent ) {
	}

	ProjectListItemDelegate::~ProjectListItemDelegate() {
	}

	void ProjectListItemDelegate::paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const {
		if( index.isValid() ) {
			painter->save();

			int32_t status = index.data( DEF_USER_ROLE_PROJECT_STATUS ).toInt();
			UserData user_data = index.data( DEF_USER_ROLE_PROJECT_USERDATA ).value<UserData>();
			
			QStyleOptionViewItem view_option( option ); // 用来在视图中画一个列表项

			QRectF rect;
			rect.setX( option.rect.x() );
			rect.setY( option.rect.y() );
			rect.setWidth( option.rect.width() - 1 );
			rect.setHeight( option.rect.height() - 1 );

			// 画圆角矩形
			const qreal radius = 0.0; // 8.0
			QPainterPath path;
			path.moveTo( rect.topRight() - QPointF( radius, 0 ) );
			path.lineTo( rect.topLeft() + QPointF( radius, 0 ) );
			path.quadTo( rect.topLeft(), rect.topLeft() + QPointF( 0, radius ) );
			path.lineTo( rect.bottomLeft() + QPointF( 0, -radius ) );
			path.quadTo( rect.bottomLeft(), rect.bottomLeft() + QPointF( radius, 0 ) );
			path.lineTo( rect.bottomRight() - QPointF( radius, 0 ) );
			path.quadTo( rect.bottomRight(), rect.bottomRight() + QPointF( 0, -radius ) );
			path.lineTo( rect.topRight() + QPointF( 0, radius ) );
			path.quadTo( rect.topRight(), rect.topRight() + QPointF( -radius, -0 ) );

			if( option.state.testFlag( QStyle::State_Selected ) ) {
				painter->setPen( QPen( QColor( 0, 128, 255 ) ) );
				painter->setBrush( QColor( 229, 241, 255 ) );
				painter->drawPath( path );
			}
			else if( option.state.testFlag( QStyle::State_MouseOver ) ) {
				painter->setPen( QPen( Qt::gray ) );
				painter->setBrush( QColor( 229, 241, 255 ) );
				painter->drawPath( path );
			}
			else {
				painter->setPen( QPen( Qt::gray ) );
				painter->setBrush( Qt::NoBrush );
				painter->drawPath( path );
			}

			// 绘制数据位置
			QRect rect_name = QRect( rect.left() + 20, rect.top() + 5, rect.width() - 30, 20 );
			QRect rect_create_time = QRect( rect.left() + 20, rect.bottom() - 20, rect.width() - 10, 20 );
			QRect circle = QRect( rect.left() + 5, rect.top() + 10, 10, 10 );
			
			switch( status ) {
			case DEF_PROJECT_STATUS_CREATE:
				painter->setBrush( QColor( 128, 128, 128 ) );
				painter->setPen( QPen( QColor( 128, 128, 128 ) ) );
				break;
			case DEF_PROJECT_STATUS_MODIFY:
				painter->setBrush( QColor( 255, 192, 0 ) );
				painter->setPen( QPen( QColor( 255, 192, 0 ) ) );
				break;
			case DEF_PROJECT_STATUS_COMMIT:
				painter->setBrush( QColor( 0, 128, 0 ) );
				painter->setPen( QPen( QColor( 0, 128, 0 ) ) );
				break;
			case DEF_PROJECT_STATUS_ERRORS:
				painter->setBrush( QColor( 255, 0, 0 ) );
				painter->setPen( QPen( QColor( 255, 0, 0 ) ) );
				break;
			}

			painter->drawEllipse( circle ); // 画圆圈

			painter->setPen( QPen( Qt::black ) );
			painter->setFont( QFont( "Times", 11, QFont::Bold ) );
			painter->drawText( rect_name, Qt::AlignLeft, user_data.m_project_name ); // 绘制名字

			painter->setPen( QPen( Qt::gray ) );
			painter->setFont( QFont( "Times", 9 ) );
			painter->drawText( rect_create_time, Qt::AlignLeft, user_data.m_project_create_time ); // 绘制创建时间

			painter->restore();
		}
	}

	QSize ProjectListItemDelegate::sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const {
		return QSize( 150, 45 );
	}

} // namespace certify
