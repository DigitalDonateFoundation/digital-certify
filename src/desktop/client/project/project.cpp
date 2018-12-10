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

#include <sqlite/sqlite3.h>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include <common/sysdef.h>
#include <common/assist.h>
#include <common/common.h>

#include "project_.h"

#define DEF_GCID_LENGTH 64

namespace certify {

	Project_P::Project_P()
	: m_db_project( nullptr )
	, m_log_cate( "<PROJECT>" ) {
		m_syslog = basicx::SysLog_S::GetInstance();
	}

	Project_P::~Project_P() {
		if( m_db_project != nullptr ) {
			delete m_db_project;
			m_db_project = nullptr;
		}
	}

	std::string Project_P::GetGCID( bool upper_case/* = false*/ ) {
		boost::uuids::uuid gcid = boost::uuids::random_generator()();
		if( false == upper_case ) { // 小写
			return boost::uuids::to_string( gcid );
		}
		else { // 大写
			std::string result = boost::uuids::to_string( gcid );
			size_t length = result.length();
			for( size_t i = 0; i < length; ++i )
				result[i] = toupper( result[i] );
			return result;
		}
		return "";
	}

	std::vector<ProjectItem*> Project_P::GetAllProject() {
		return m_vec_project;
	}

	int32_t Project_P::LoadExistProject( std::string dbf_path ) {
		std::string log_info = "";
		
		m_vec_project.clear();
		m_map_project.clear();
		m_map_project_name.clear();
		m_map_project_path.clear();
		int32_t project_count = 0;

		try {
			//FormatLibrary::StandardLibrary::FormatTo( log_info, "{0} {1} {2}", SQLite::VERSION, SQLite::getLibVersion(), SQLITECPP_VERSION );
			//m_syslog->LogWrite( basicx::syslog_level::c_debug, m_log_cate, log_info );

			m_db_project = new SQLite::Database( dbf_path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE );
			log_info = "打开项目信息文件完成。";
			m_syslog->LogWrite( basicx::syslog_level::c_info, m_log_cate, log_info );

			if( false == m_db_project->tableExists( "project" ) ) {
				m_db_project->exec( "CREATE TABLE project ( \
					                     id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE NOT NULL DEFAULT ( 0 ), \
                                         gcid VARCHAR ( 64 ) UNIQUE NOT NULL DEFAULT ( '' ), \
					                     name VARCHAR ( 64 ) UNIQUE NOT NULL DEFAULT ( '' ), \
					                     path VARCHAR ( 256 ) UNIQUE NOT NULL DEFAULT ( '' ), \
                                         create_time DATETIME NOT NULL DEFAULT ( datetime( 'now', 'localtime' ) ) \
                                     )" );
				m_db_project->exec( "CREATE INDEX idx_gcid ON project ( gcid ASC )" );
				m_db_project->exec( "CREATE INDEX idx_name ON project ( name ASC )" );
				log_info = "创建项目信息库表完成。";
				m_syslog->LogWrite( basicx::syslog_level::c_info, m_log_cate, log_info );
			}

			int32_t ret_id = 0;
			std::string ret_gcid = "";
			std::string ret_name = "";
			std::string ret_path = "";
			std::string ret_create_time = "";
			SQLite::Statement query( *m_db_project, "SELECT id, gcid, name, path, create_time FROM project ORDER BY id ASC" );
			while( query.executeStep() ) {
				project_count++; //
				ret_id = query.getColumn( 0 ).getInt();
				ret_gcid = query.getColumn( 1 ).getText();
				ret_name = basicx::StringToGB2312( query.getColumn( 2 ).getText() ); // 中文 UTF8 还原
				ret_path = basicx::StringToGB2312( query.getColumn( 3 ).getText() ); // 中文 UTF8 还原
				ret_create_time = query.getColumn( 4 ).getText();
				ProjectItem* project_item = new ProjectItem( ret_gcid, ret_name, ret_path );
				// TODO：ret_create_time
				m_vec_project.push_back( project_item );
				m_map_project[ret_gcid] = project_item;
				m_map_project_name[ret_name] = ret_name;
				m_map_project_path[ret_path] = ret_path;
				FormatLibrary::StandardLibrary::FormatTo( log_info, "从项目信息库表获取：{0} {1} {2} {3} {4}", ret_id, ret_gcid, ret_name, ret_path, ret_create_time );
				m_syslog->LogWrite( basicx::syslog_level::c_info, m_log_cate, log_info );
			}
			query.reset();

			FormatLibrary::StandardLibrary::FormatTo( log_info, "共获取已有项目 {0} 个。", project_count );
			m_syslog->LogWrite( basicx::syslog_level::c_info, m_log_cate, log_info );
		}
		catch( std::exception& e ) {
			FormatLibrary::StandardLibrary::FormatTo( log_info, "打开项目信息文件异常！{0} {1}", e.what(), dbf_path );
			m_syslog->LogWrite( basicx::syslog_level::c_error, m_log_cate, log_info );
			return -1;
		}

		return project_count;
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

	ProjectItem* Project_P::CreateProject( std::string name, std::string path ) {
		std::string log_info = "";

		if( false == CanCreateProject( name, path ) ) {
			return nullptr;
		}

		std::string gcid = GetGCID( true );
		if( m_db_project != nullptr ) {
			try {
				ProjectItem* project_item = new ProjectItem( gcid, name, path );
				std::string create_time = project_item->GetCreateTime();

				SQLite::Statement query( *m_db_project, "INSERT INTO project ( gcid, name, path, create_time ) VALUES ( ?, ?, ?, ? )" );
				SQLite::bind( query, gcid, basicx::StringToUTF8( name ), basicx::StringToUTF8( path ), create_time ); // 中文 UTF8 转换
				if( query.exec() != 1 ) {
					FormatLibrary::StandardLibrary::FormatTo( log_info, "添加项目信息记录失败！{0} {1} {2}", gcid, name, path );
					m_syslog->LogWrite( basicx::syslog_level::c_error, m_log_cate, log_info );
					return nullptr;
				}
				
				m_vec_project.push_back( project_item );
				m_map_project[gcid] = project_item;
				m_map_project_name[name] = name;
				m_map_project_path[path] = path;

				return project_item;
			}
			catch( std::exception& e ) {
				FormatLibrary::StandardLibrary::FormatTo( log_info, "添加项目信息记录异常！{0} {1} {2} {3}", e.what(), gcid, name, path );
				m_syslog->LogWrite( basicx::syslog_level::c_error, m_log_cate, log_info );
				return nullptr;
			}
		}

		return nullptr;
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

	bool Project::CanCreateProject( std::string name, std::string path ) {
		return m_project_p->CanCreateProject( name, path );
	}

	ProjectItem* Project::CreateProject( std::string name, std::string path ) {
		return m_project_p->CreateProject( name, path );
	}

} // namespace certify
