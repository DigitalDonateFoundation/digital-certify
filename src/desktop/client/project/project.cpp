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

	ProjectItem_P::ProjectItem_P( std::string gcid, std::string name, std::string path )
	: m_gcid( gcid )
	, m_name( name )
	, m_path( path )
	, m_home( m_path + "/.certify" )
	, m_create_time( std::chrono::system_clock::now() ) {
	}

	ProjectItem_P::~ProjectItem_P() {
	}

	std::string ProjectItem_P::GetGCID() {
		return m_gcid;
	}

	std::string ProjectItem_P::GetName() {
		return m_name;
	}

	std::string ProjectItem_P::GetPath() {
		return m_path;
	}

	std::string ProjectItem_P::GetHome() {
		return m_home;
	}

	std::string ProjectItem_P::GetCreateTime() {
		std::time_t create_time_t = std::chrono::system_clock::to_time_t( m_create_time );
		tm create_time_tm;
		localtime_s( &create_time_tm, &create_time_t );
		char create_time_buffer[64] = { 0 };
		strftime( create_time_buffer, 64, "%Y-%m-%d %H:%M:%S", &create_time_tm );
		return std::string( create_time_buffer );
	}

	void ProjectItem_P::SetCreateTime( std::string create_time ) {
		tm create_time_tm;
		std::istringstream iss( create_time );
		iss >> std::get_time( &create_time_tm, "%Y-%m-%d %H:%M:%S" );
		m_create_time = std::chrono::system_clock::from_time_t( std::mktime( &create_time_tm ) );
	}

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
		if( false == upper_case ) { // Сд
			return boost::uuids::to_string( gcid );
		}
		else { // ��д
			std::string result = boost::uuids::to_string( gcid );
			size_t length = result.length();
			for( size_t i = 0; i < length; ++i )
				result[i] = toupper( result[i] );
			return result;
		}
		return "";
	}

	ProjectItem* Project_P::GetProject( std::string gcid ) {
		auto it_pi = m_map_project.find( gcid );
		if( it_pi != m_map_project.end() ) {
			return it_pi->second;
		}
		return nullptr;
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
			log_info = "����Ŀ��Ϣ�ļ���ɡ�";
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
				log_info = "������Ŀ��Ϣ�����ɡ�";
				m_syslog->LogWrite( basicx::syslog_level::c_info, m_log_cate, log_info );
			}

			int32_t ret_id = 0;
			std::string ret_gcid = "";
			std::string ret_name = "";
			std::string ret_path = "";
			std::string ret_create_time = "";
			SQLite::Statement query( *m_db_project, "SELECT id, gcid, name, path, create_time FROM project ORDER BY id ASC" );
			while( query.executeStep() ) {
				ret_id = query.getColumn( 0 ).getInt();
				ret_gcid = query.getColumn( 1 ).getText();
				ret_name = basicx::StringToGB2312( query.getColumn( 2 ).getText() ); // ���� UTF8 ��ԭ
				ret_path = basicx::StringToGB2312( query.getColumn( 3 ).getText() ); // ���� UTF8 ��ԭ
				ret_create_time = query.getColumn( 4 ).getText();

				ProjectItem* project_item = new ProjectItem( ret_gcid, ret_name, ret_path );
				project_item->SetCreateTime( ret_create_time );

				m_vec_project.push_back( project_item );
				m_map_project[ret_gcid] = project_item;
				m_map_project_name[ret_name] = ret_name;
				m_map_project_path[ret_path] = ret_path;

				project_count++; //
				FormatLibrary::StandardLibrary::FormatTo( log_info, "����Ŀ��Ϣ����ȡ��{0} {1} {2} {3}", ret_id, ret_gcid, ret_name, ret_path );
				m_syslog->LogWrite( basicx::syslog_level::c_info, m_log_cate, log_info );
			}
			query.reset();

			FormatLibrary::StandardLibrary::FormatTo( log_info, "����ȡ������Ŀ {0} ����", project_count );
			m_syslog->LogWrite( basicx::syslog_level::c_info, m_log_cate, log_info );
		}
		catch( std::exception& e ) {
			FormatLibrary::StandardLibrary::FormatTo( log_info, "����Ŀ��Ϣ�ļ��쳣��{0} {1}", e.what(), dbf_path );
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

				SQLite::Statement query( *m_db_project, "INSERT INTO project ( gcid, name, path, create_time ) VALUES ( ?, ?, ?, ? )" );
				SQLite::bind( query, gcid, basicx::StringToUTF8( name ), basicx::StringToUTF8( path ), project_item->GetCreateTime() ); // ���� UTF8 ת��
				if( query.exec() != 1 ) {
					FormatLibrary::StandardLibrary::FormatTo( log_info, "�����Ŀ��Ϣ��¼ʧ�ܣ�{0} {1} {2}", gcid, name, path );
					m_syslog->LogWrite( basicx::syslog_level::c_error, m_log_cate, log_info );
					return nullptr;
				}
				
				m_vec_project.push_back( project_item );
				m_map_project[gcid] = project_item;
				m_map_project_name[name] = name;
				m_map_project_path[path] = path;

				FormatLibrary::StandardLibrary::FormatTo( log_info, "�½���Ŀ�����ɣ�{0} {1} {2}", gcid, name, path );
				m_syslog->LogWrite( basicx::syslog_level::c_info, m_log_cate, log_info );

				return project_item;
			}
			catch( std::exception& e ) {
				FormatLibrary::StandardLibrary::FormatTo( log_info, "�����Ŀ��Ϣ��¼�쳣��{0} {1} {2} {3}", e.what(), gcid, name, path );
				m_syslog->LogWrite( basicx::syslog_level::c_error, m_log_cate, log_info );
				return nullptr;
			}
		}

		return nullptr;
	}

	ProjectItem::ProjectItem( std::string gcid, std::string name, std::string path )
	: m_project_item_p( nullptr ) {
		try {
			m_project_item_p = new ProjectItem_P( gcid, name, path );
		}
		catch( ... ) {}
	}

	ProjectItem::~ProjectItem() {
		if( m_project_item_p != nullptr ) {
			delete m_project_item_p;
			m_project_item_p = nullptr;
		}
	}

	std::string ProjectItem::GetGCID() {
		return m_project_item_p->GetGCID();
	}

	std::string ProjectItem::GetName() {
		return m_project_item_p->GetName();
	}

	std::string ProjectItem::GetPath() {
		return m_project_item_p->GetPath();
	}

	std::string ProjectItem::GetHome() {
		return m_project_item_p->GetHome();
	}

	std::string ProjectItem::GetCreateTime() {
		return m_project_item_p->GetCreateTime();
	}

	void ProjectItem::SetCreateTime( std::string create_time ) {
		m_project_item_p->SetCreateTime( create_time );
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

	ProjectItem* Project::GetProject( std::string gcid ) {
		return m_project_p->GetProject( gcid );
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
