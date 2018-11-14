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

#ifndef CERTIFY_CERTIFY_MIAN_WINDOW_H
#define CERTIFY_CERTIFY_MIAN_WINDOW_H

#include <QtGui/QCloseEvent>
#include <QtCore/QTimer>
#include <QtCore/QDateTime>
#include <QtCore/QSettings>
#include <QtWidgets/QMenu>
#include <QtWidgets/QWidget>
#include <QtWidgets/QAction>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QApplication>
#include <QtWidgets/QSystemTrayIcon>

#include <syslog/syslog.h>

#include "about_dialog.h"

namespace certify {

	class MainWindow : public QMainWindow
	{
		Q_OBJECT

	public:
		MainWindow( QWidget* parent = nullptr );
		~MainWindow();

	public:
		void CreateActions();
		void InitInterface();

	public:
		void SystemStart();
		void LoadTrayIcon();
		void ShowTrayIconMsg( std::string msg );

	private slots:
		void UpdateTime();
		void OnActionExit();
		void OnActionAbout();
		void ShowMainWindow();
		void HideMainWindow();
		void TrayIconMsgClicked();
		void TrayIconActivated( QSystemTrayIcon::ActivationReason nReason );
		void OnActionAutoStart( bool auto_start );

	protected:
		void closeEvent( QCloseEvent* event );
		bool eventFilter( QObject* target, QEvent* event );

	private:
		QTimer* m_timer;
		bool m_user_exit;
		bool m_not_first_run;
		bool m_tray_icon_load;
		bool m_main_window_visible;
		QMenu* m_tray_icon_menu;
		QMenu* m_tray_icon_menu_auto_start;
		QSystemTrayIcon* m_tray_icon;

		QAction* m_action_show;
		QAction* m_action_hide;
		QAction* m_action_exit;
		QAction* m_action_about;
		QAction* m_action_auto_start;

		QStatusBar* m_status_bar;

		QWidget* m_main_widget; // 必须，否则底部停靠栏高度会无法向下调整

		QLabel* m_label_logo;
		QLabel* m_label_info;
		QLabel* m_label_time;

		AboutDialog* m_about_dialog;

	private:
		std::string m_log_cate;
		basicx::SysLog_S* m_syslog;
	};

} // namespace certify

#endif // CERTIFY_CERTIFY_MIAN_WINDOW_H
