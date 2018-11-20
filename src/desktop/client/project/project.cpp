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

#include "project_.h"

namespace certify {

	Project_P::Project_P() {
	}

	Project_P::~Project_P() {
	}

	bool Project_P::CreateProject( std::string name, std::string path ) {
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

	bool Project::CreateProject( std::string name, std::string path ) {
		return m_project_p->CreateProject( name, path );
	}

	bool Project::CanCreateProject( std::string name, std::string path ) {
		return m_project_p->CanCreateProject( name, path );
	}

} // namespace certify
