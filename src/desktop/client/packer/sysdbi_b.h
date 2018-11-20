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

#ifndef CERTIFY_PACKER_SYSDBI_B_H
#define CERTIFY_PACKER_SYSDBI_B_H

#include <vector>
#include <iostream>
#include <stdint.h>

#include <common/common.h>
#include <syslog/syslog.h>

#include "global/compile.h"

namespace certify {

	class SysDBI_B_P;

	class CERTIFY_PACKER_EXPIMP SysDBI_B {
	public:
		SysDBI_B();
		~SysDBI_B();

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
		SysDBI_B_P* m_sysdbi_b_p;
	};

} // namespace certify

#endif // CERTIFY_PACKER_SYSDBI_B_H
