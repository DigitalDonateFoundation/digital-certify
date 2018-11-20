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

#include <string>

#include "global/compile.h"

// ��Ŀ״̬����
#define DEF_PROJECT_STATUS_CREATE 0 //
#define DEF_PROJECT_STATUS_MODIFY 1 //
#define DEF_PROJECT_STATUS_COMMIT 2 //
#define DEF_PROJECT_STATUS_ERRORS 3 //

namespace certify {

	class Project_P;

	class CERTIFY_PROJECT_EXPIMP Project {
	public:
		Project();
		~Project();

	public:
		static Project* GetInstance();

	public:
		bool CreateProject( std::string name, std::string path );
		bool CanCreateProject( std::string name, std::string path );

	private:
		Project_P* m_project_p;
		static Project* m_instance;
	};

} // namespace certify

#endif // CERTIFY_PROJECT_PROJECT_H