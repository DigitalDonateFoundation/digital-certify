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
#include <sstream>
#include <iomanip>

#include "global/compile.h"

// 项目状态定义
#define DEF_PROJECT_STATUS_CREATE 0 //
#define DEF_PROJECT_STATUS_MODIFY 1 //
#define DEF_PROJECT_STATUS_COMMIT 2 //
#define DEF_PROJECT_STATUS_ERRORS 3 //

namespace certify {

	class ProjectItem_P;

	class CERTIFY_PROJECT_EXPIMP ProjectItem {
	public:
		ProjectItem( std::string gcid, std::string name, std::string path );
		~ProjectItem();

	public:
		std::string GetGCID();
		std::string GetName();
		std::string GetPath();
		std::string GetHome();
		std::string GetCreateTime();
		void SetCreateTime( std::string create_time );

	private:
		ProjectItem_P* m_project_item_p;
	};

	class Project_P;

	class CERTIFY_PROJECT_EXPIMP Project {
	public:
		Project();
		~Project();

	public:
		static Project* GetInstance();

	public:
		ProjectItem* GetProject( std::string gcid );
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
