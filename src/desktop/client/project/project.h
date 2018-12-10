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

#ifndef CERTIFY_PROJECT_PROJECT_H
#define CERTIFY_PROJECT_PROJECT_H

#include <ctime>
#include <string>
#include <chrono>

#include "global/compile.h"

// 项目状态定义
#define DEF_PROJECT_STATUS_CREATE 0 //
#define DEF_PROJECT_STATUS_MODIFY 1 //
#define DEF_PROJECT_STATUS_COMMIT 2 //
#define DEF_PROJECT_STATUS_ERRORS 3 //

namespace certify {

	class CERTIFY_PROJECT_EXPIMP ProjectItem {
	public:
		ProjectItem( std::string gcid, std::string name, std::string path )
		: m_gcid( gcid )
		, m_name( name )
		, m_path( path )
		, m_home( m_path + "/.certify" )
		, m_create_time( std::chrono::system_clock::now() ) {
		}

		~ProjectItem() {
		}

		std::string GetCreateTime() {
			std::time_t create_time_t = std::chrono::system_clock::to_time_t( m_create_time );
			tm create_time_tm;
			localtime_s( &create_time_tm, &create_time_t );
			char create_time_buffer[64] = { 0 };
			strftime( create_time_buffer, 64, "%Y-%m-%d %H:%M:%S", &create_time_tm );
			return std::string( create_time_buffer );
		}

	public:
		std::string m_gcid;
		std::string m_name;
		std::string m_path;
		std::string m_home;
		std::chrono::system_clock::time_point m_create_time;
	};

	class Project_P;

	class CERTIFY_PROJECT_EXPIMP Project {
	public:
		Project();
		~Project();

	public:
		static Project* GetInstance();

	public:
		std::vector<ProjectItem*> GetAllProject();
		int32_t LoadExistProject( std::string dbf_path );
		bool CanCreateProject( std::string name, std::string path );
		ProjectItem* CreateProject( std::string name, std::string path );
		
	private:
		Project_P* m_project_p;
		static Project* m_instance;
	};

} // namespace certify

#endif // CERTIFY_PROJECT_PROJECT_H
