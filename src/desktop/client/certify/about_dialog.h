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

#ifndef CERTIFY_CERTIFY_ABOUT_DIALOG_H
#define CERTIFY_CERTIFY_ABOUT_DIALOG_H

#include <QtWidgets/QLabel>
#include <QtWidgets/QDialog>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QDialogButtonBox>

#include "global/define.h"

class AboutDialog : public QDialog
{
	Q_OBJECT

public:
	AboutDialog( QWidget* parent = nullptr );
	~AboutDialog();

public:
	void InitInterface();

private:
	QWidget* m_widget;
	QLabel* m_label_app_name;
	QLabel* m_label_app_version;
	QLabel* m_label_app_icon;
	QLabel* m_label_app_developer;
	QLabel* m_label_app_company;
	QLabel* m_label_app_copyright;
	QDialogButtonBox* m_button_box;
	QHBoxLayout* m_layout_h_1;
	QHBoxLayout* m_layout_h_2;
	QVBoxLayout* m_layout_v;
	QSpacerItem* m_phs_spacer_1;
	QSpacerItem* m_phs_spacer_2;
	QSpacerItem* m_phs_spacer_3;
};

#endif // CERTIFY_CERTIFY_ABOUT_DIALOG_H
