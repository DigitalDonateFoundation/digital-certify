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

#include <berkeleydb/db_cxx.h>

#include "sysdbi_b.h"

#ifndef CERTIFY_PACKER_SYSDBI_B_P_H
#define CERTIFY_PACKER_SYSDBI_B_P_H

namespace certify {

	class SysDBI_B_P {
	public:
		SysDBI_B_P();
		~SysDBI_B_P();

	public:
		int32_t OpenDB( std::string db_path, std::string db_file, uint32_t cache_size, std::string& error );
		int32_t CloseDB( std::string& error );
		int32_t PutString( const char* key, const char* value, std::string& error );
		int32_t GetString( const char* key, std::string& value, std::string& error );
		int32_t PutBinary( const char* key, const char* value, int32_t length, std::string& error );
		int32_t GetBinary( const char* key, char*& value, int32_t& length, std::string& error );
		int32_t DelRecord( const char* key, std::string& error );
		int32_t DelAllRecord( std::string& error );
		void PrintAllRecord();

	private:
		Db* m_db;
		DbEnv* m_db_env;
		std::string m_log_cate;
		basicx::SysLog_S* m_syslog;
	};

} // namespace certify

#endif // CERTIFY_PACKER_SYSDBI_B_P_H
