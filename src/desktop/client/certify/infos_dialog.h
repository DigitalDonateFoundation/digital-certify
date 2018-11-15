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

#ifndef CERTIFY_CERTIFY_INFOS_DIALOG_H
#define CERTIFY_CERTIFY_INFOS_DIALOG_H

#include <QtCore/QTimer>
#include <QtWidgets/QLabel>
#include <QtWidgets/QDialog>
#include <QtWidgets/QWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QDialogButtonBox>

namespace certify {

	class InfosDialog : public QDialog
	{
		Q_OBJECT

	public:
		InfosDialog( QWidget* parent = nullptr );
		~InfosDialog();

	public:
		void InitInterface();

	private slots:
		void OnTimerShow();
		void OnTimerStay();
		void OnTimerHide();
		void OnButtonClose();

	public:
		void ShowMessage();
		void SetDialogInfos( std::string info_text );

	private:
		QWidget* m_widget;
		QTextEdit* m_text_edit;
		QSpacerItem* m_phs_spacer;
		QDialogButtonBox* m_button_box;
		QVBoxLayout* m_layout_v;
	
		QPoint m_point;
		double m_transparent;
		QTimer* m_timer_show;
		QTimer* m_timer_stay;
		QTimer* m_timer_hide;
		int32_t m_desktop_height;

		bool m_is_show;
		std::string m_info_text;
	};

} // namespace certify

#endif // CERTIFY_CERTIFY_INFOS_DIALOG_H
