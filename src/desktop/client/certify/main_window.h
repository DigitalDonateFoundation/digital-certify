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
#include <QtGui/QKeySequence>
#include <QtCore/QTimer>
#include <QtCore/QDateTime>
#include <QtCore/QSettings>
#include <QtWidgets/QMenu>
#include <QtWidgets/QWidget>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QApplication>
#include <QtWidgets/QStyleFactory>
#include <QtWidgets/QSystemTrayIcon>

#include <syslog/syslog.h>

#include "about_dialog.h"
#include "infos_dialog.h"

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
		void ReadSettings();
		void WriteSettings();
		void RemoveSettings( std::string key );
		void CloseFloatDockWidget();

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
		void TrayIconActivated( QSystemTrayIcon::ActivationReason reason );
		void OnActionAutoStart( bool start );
		void ShowToolBar_Main( bool show );
		void ShowDockWidget_1( bool show );
		void ShowDockWidget_2( bool show );
		void ShowDockWidget_3( bool show );
		void OnActionSaveLayout( bool save );

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

		QAction* m_action_exit;
		QAction* m_action_about;
		QAction* m_action_show_tool_bar_main;
		QAction* m_action_save_layout;
		QAction* m_action_auto_start;
		QAction* m_action_show_dock_1;
		QAction* m_action_show_dock_2;
		QAction* m_action_show_dock_3;

		QMenuBar* m_menu_bar;
		QMenu* m_menu_file;
		QMenu* m_menu_tool;
		QMenu* m_menu_view;
		QMenu* m_menu_view_tooler;
		QMenu* m_menu_view_docker;
		QMenu* m_menu_help;
		QToolBar* m_main_tool_bar;
		QStatusBar* m_status_bar;

		QWidget* m_main_widget; // ���룬����ײ�ͣ�����߶Ȼ��޷����µ���

		QLabel* m_label_logo;
		QLabel* m_label_info;
		QLabel* m_label_time;

		QTextEdit* m_text_edit_1;
		QTextEdit* m_text_edit_2;
		QTextEdit* m_text_edit_3;
		QDockWidget* m_dock_widget_1;
		QDockWidget* m_dock_widget_2;
		QDockWidget* m_dock_widget_3;
		std::vector<QDockWidget*> m_vec_dock_widget;

		AboutDialog* m_about_dialog;
		InfosDialog* m_infos_dialog;

	private:
		std::string m_log_cate;
		basicx::SysLog_S* m_syslog;
	};

} // namespace certify

#endif // CERTIFY_CERTIFY_MIAN_WINDOW_H
