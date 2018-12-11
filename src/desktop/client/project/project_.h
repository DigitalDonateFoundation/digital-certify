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

#include <map>
#include <vector>

#include <sqlitecpp/SQLiteCpp.h>
#include <sqlitecpp/VariadicBind.h>

#include <syslog/syslog.h>

#include "project.h"

#ifndef CERTIFY_PROJECT_PROJECT_P_H
#define CERTIFY_PROJECT_PROJECT_P_H

namespace certify {

	class ProjectItem_P {
	public:
		ProjectItem_P( std::string gcid, std::string name, std::string path );
		~ProjectItem_P();

	public:
		std::string GetGCID();
		std::string GetName();
		std::string GetPath();
		std::string GetHome();
		std::string GetCreateTime();
		void SetCreateTime( std::string create_time );

	private:
		std::string m_gcid;
		std::string m_name;
		std::string m_path;
		std::string m_home;
		std::chrono::system_clock::time_point m_create_time;
	};

	class Project_P {
	public:
		Project_P();
		~Project_P();

	public:
		std::string GetGCID( bool upper_case = false );
		std::vector<ProjectItem*> GetAllProject();
		int32_t LoadExistProject( std::string dbf_path );
		bool CanCreateProject( std::string name, std::string path );
		ProjectItem* CreateProject( std::string name, std::string path );

	private:
		SQLite::Database* m_db_project;

		std::vector<ProjectItem*> m_vec_project;
		std::map<std::string, ProjectItem*> m_map_project;
		std::map<std::string, std::string> m_map_project_name;
		std::map<std::string, std::string> m_map_project_path;

	private:
		std::string m_log_cate;
		basicx::SysLog_S* m_syslog;
	};

} // namespace certify

#endif // CERTIFY_PROJECT_PROJECT_P_H
