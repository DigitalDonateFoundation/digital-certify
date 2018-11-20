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

#include "project.h"

#ifndef CERTIFY_PROJECT_PROJECT_P_H
#define CERTIFY_PROJECT_PROJECT_P_H

namespace certify {

	class Project_P {
	public:
		Project_P();
		~Project_P();

	public:
		bool CreateProject( std::string name, std::string path );
		bool CanCreateProject( std::string name, std::string path );

	private:
		std::map<std::string, std::string> m_map_project_name;
		std::map<std::string, std::string> m_map_project_path;
	};

} // namespace certify

#endif // CERTIFY_PROJECT_PROJECT_P_H
