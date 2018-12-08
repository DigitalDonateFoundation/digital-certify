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

#include <common/common.h>

#include "project_.h"

namespace certify {

	Project_P::Project_P()
	: m_log_cate( "<PROJECT>" ) {
		m_syslog = basicx::SysLog_S::GetInstance();
	}

	Project_P::~Project_P() {
	}

	std::vector<ProjectItem*> Project_P::GetAllProject() {
		return m_vec_project;
	}

	int32_t Project_P::LoadExistProject( std::string dbf_path ) {
		std::string log_info = "";
		int32_t project_count = 0;

		m_vec_project.clear();
		m_map_project.clear();
		m_map_project_name.clear();
		m_map_project_path.clear();

		try {
			//FormatLibrary::StandardLibrary::FormatTo( log_info, "{0} {1} {2}", SQLite::VERSION, SQLite::getLibVersion(), SQLITECPP_VERSION );
			//m_syslog->LogWrite( basicx::syslog_level::c_debug, m_log_cate, log_info );

			SQLite::Database db_project( dbf_path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE );
			log_info = "打开项目信息文件完成。";
			m_syslog->LogWrite( basicx::syslog_level::c_info, m_log_cate, log_info );

			if( false == db_project.tableExists( "project" ) ) {
				db_project.exec( "CREATE TABLE project ( \
					                  id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE NOT NULL DEFAULT ( 0 ), \
					                  name VARCHAR ( 64 ) UNIQUE NOT NULL DEFAULT ( '' ), \
					                  path VARCHAR ( 256 ) UNIQUE NOT NULL DEFAULT ( '' ) \
                                  )" );
				db_project.exec( "CREATE INDEX idx_name ON project ( name ASC )" );
				log_info = "创建项目信息库表完成。";
				m_syslog->LogWrite( basicx::syslog_level::c_info, m_log_cate, log_info );
			}

			int32_t ret_id = 0;
			std::string ret_name = "";
			std::string ret_path = "";
			SQLite::Statement query( db_project, "SELECT id, name, path FROM project ORDER BY id ASC" );
			while( query.executeStep() ) {
				project_count++; //
				ret_id = query.getColumn( 0 ).getInt();
				ret_name = query.getColumn( 1 ).getText();
				ret_path = query.getColumn( 2 ).getText();
				ProjectItem* project_item = new ProjectItem( ret_name, ret_path );
				m_vec_project.push_back( project_item );
				m_map_project[ret_name] = project_item;
				m_map_project_name[ret_name] = ret_name;
				m_map_project_path[ret_path] = ret_path;
				FormatLibrary::StandardLibrary::FormatTo( log_info, "从项目信息库表获取：{0} {1} {2}", ret_id, ret_name, ret_path );
				m_syslog->LogWrite( basicx::syslog_level::c_info, m_log_cate, log_info );
			}

			FormatLibrary::StandardLibrary::FormatTo( log_info, "共获取已有项目 {0} 个。", project_count );
			m_syslog->LogWrite( basicx::syslog_level::c_info, m_log_cate, log_info );
		}
		catch( std::exception& e ) {
			FormatLibrary::StandardLibrary::FormatTo( log_info, "打开项目信息文件失败！{0} {1}", e.what(), dbf_path );
			m_syslog->LogWrite( basicx::syslog_level::c_error, m_log_cate, log_info );
			return -1;
		}

		return project_count;
	}

	bool Project_P::CreateProject( std::string name, std::string path ) {
		if( false == CanCreateProject( name, path ) ) {
			return false;
		}

		// TODO：添加项目信息数据库记录

		ProjectItem* project_item = new ProjectItem( name, path );
		m_vec_project.push_back( project_item );
		m_map_project[name] = project_item;
		m_map_project_name[name] = name;
		m_map_project_path[path] = path;
		return true;
	}

	bool Project_P::CanCreateProject( std::string name, std::string path ) {
		bool is_exist = false;
		auto it_pn = m_map_project_name.find( name );
		if( it_pn != m_map_project_name.end() ) {
			is_exist = true;
		}
		auto it_pp = m_map_project_path.find( path );
		if( it_pp != m_map_project_path.end() ) {
			is_exist = true;
		}
		return !is_exist;
	}

	Project* Project::m_instance = nullptr;

	Project::Project()
	: m_project_p( nullptr ) {
		try {
			m_project_p = new Project_P();
		}
		catch( ... ) {}
		m_instance = this;
	}

	Project::~Project() {
		if( m_project_p != nullptr ) {
			delete m_project_p;
			m_project_p = nullptr;
		}
	}

	Project* Project::GetInstance() {
		return m_instance;
	}

	std::vector<ProjectItem*> Project::GetAllProject() {
		return m_project_p->GetAllProject();
	}

	int32_t Project::LoadExistProject( std::string dbf_path ) {
		return m_project_p->LoadExistProject( dbf_path );
	}

	bool Project::CreateProject( std::string name, std::string path ) {
		return m_project_p->CreateProject( name, path );
	}

	bool Project::CanCreateProject( std::string name, std::string path ) {
		return m_project_p->CanCreateProject( name, path );
	}

} // namespace certify
