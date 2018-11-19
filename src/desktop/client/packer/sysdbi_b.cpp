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

#include "sysdbi_b_.h"

namespace certify {

	SysDBI_B_P::SysDBI_B_P()
		: m_db( nullptr )
		, m_db_env( nullptr )
		, m_log_cate( "<SYSDBI_B>" ) {
		m_syslog = basicx::SysLog_S::GetInstance();
	}

	SysDBI_B_P::~SysDBI_B_P() {
	}

	int32_t SysDBI_B_P::OpenDB( std::string db_path, std::string db_file, uint32_t cache_size, std::string& error ) {
		int32_t ret = CloseDB( error );
		if( ret != 0 ) {
			return ret;
		}
		try {
			u_int32_t flags_db_env = DB_CREATE | DB_INIT_LOG | DB_INIT_MPOOL;
			m_db_env = new DbEnv( nullptr );
			m_db_env->set_cachesize( 0, cache_size, 1 );
			m_db_env->open( db_path.c_str(), flags_db_env, 0 );

			u_int32_t flags_db = DB_CREATE;
			m_db = new Db( m_db_env, 0 );
			ret = m_db->open( nullptr, db_file.c_str(), nullptr, DB_HASH, flags_db, 0 ); // DB_BTREE、DB_HASH
			if( ret != 0 ) {
				error = DbEnv::strerror( ret );
				return ret;
			}
		}
		catch( DbException& e ) {
			error = e.what();
			return -1;
		}
		catch( std::exception& e ) {
			error = e.what();
			return -2;
		}
		catch( ... ) {
			error = "未知异常！";
			return -3;
		}
		return 0;
	}

	int32_t SysDBI_B_P::CloseDB( std::string& error ) {
		try {
			if( m_db != nullptr ) {
				m_db->close( 0 );
				delete m_db;
				m_db = nullptr;
			}
			if( m_db_env != nullptr ) {
				m_db_env->close( 0 );
				delete m_db_env;
				m_db_env = nullptr;
			}
		}
		catch( DbException& e ) {
			error = e.what();
			return -1;
		}
		catch( std::exception& e ) {
			error = e.what();
			return -2;
		}
		catch( ... ) {
			error = "未知异常！";
			return -3;
		}
		return 0;
	}

	int32_t SysDBI_B_P::PutString( const char* key, const char* value, std::string& error ) {
		try {
			Dbt db_key( (char*)key, (u_int32_t)strlen( key ) );
			Dbt db_value( (char*)value, (u_int32_t)strlen( value ) );
			int32_t ret = m_db->put( nullptr, &db_key, &db_value, DB_NOOVERWRITE );
			if( ret == DB_KEYEXIST ) {
				return -1;
			}
			m_db->sync( 0 );
		}
		catch( DbException& e ) {
			error = e.what();
			return -2;
		}
		catch( std::exception& e ) {
			error = e.what();
			return -3;
		}
		catch( ... ) {
			error = "未知异常！";
			return -4;
		}
		return 0;
	}

	int32_t SysDBI_B_P::GetString( const char* key, std::string& value, std::string& error ) {
		try {
			Dbt db_key( (char*)key, (u_int32_t)strlen( key ) );
			Dbt db_value;
			int32_t ret = m_db->get( nullptr, &db_key, &db_value, 0 );
			if( ret == 0 ) {
				//std::string key_string( (char*)db_key.get_data(), db_key.get_size() );
				value = std::string( (char*)db_value.get_data(), db_value.get_size() );
			}
			else {
				error = DbEnv::strerror( ret );
				return -1;
			}
		}
		catch( DbException& e ) {
			error = e.what();
			return -2;
		}
		catch( std::exception& e ) {
			error = e.what();
			return -3;
		}
		catch( ... ) {
			error = "未知异常！";
			return -4;
		}
		return 0;
	}

	int32_t SysDBI_B_P::PutBinary( const char* key, const char* value, int32_t length, std::string& error ) {
		try {
			Dbt db_key( (char*)key, (u_int32_t)strlen( key ) );
			Dbt db_value( (char*)value, length );
			int32_t ret = m_db->put( nullptr, &db_key, &db_value, DB_NOOVERWRITE );
			if( ret == DB_KEYEXIST ) {
				return -1;
			}
			m_db->sync( 0 );
		}
		catch( DbException& e ) {
			error = e.what();
			return -2;
		}
		catch( std::exception& e ) {
			error = e.what();
			return -3;
		}
		catch( ... ) {
			error = "未知异常！";
			return -4;
		}
		return 0;
	}

	int32_t SysDBI_B_P::GetBinary( const char* key, char*& value, int32_t& length, std::string& error ) {
		try {
			Dbt db_key( (char*)key, (u_int32_t)strlen( key ) );
			Dbt db_value;
			int32_t ret = m_db->get( nullptr, &db_key, &db_value, 0 );
			if( ret == 0 ) {
				//std::string key_string( (char*)db_key.get_data(), db_key.get_size() );
				length = db_value.get_size();
				value = (char*)db_value.get_data();
			}
			else {
				error = DbEnv::strerror( ret );
				return -1;
			}
		}
		catch( DbException& e ) {
			error = e.what();
			return -2;
		}
		catch( std::exception& e ) {
			error = e.what();
			return -3;
		}
		catch( ... ) {
			error = "未知异常！";
			return -4;
		}
		return 0;
	}

	int32_t SysDBI_B_P::DelRecord( const char* key, std::string& error ) {
		try {
			Dbt db_key( (char*)key, (u_int32_t)strlen( key ) );
			int32_t ret = m_db->del( nullptr, &db_key, 0 );
			if( ret != 0 ) {
				error = DbEnv::strerror( ret );
				return -1;
			}
			m_db->sync( 0 );
		}
		catch( DbException& e ) {
			error = e.what();
			return -2;
		}
		catch( std::exception& e ) {
			error = e.what();
			return -3;
		}
		catch( ... ) {
			error = "未知异常！";
			return -4;
		}
		return 0;
	}

	int32_t SysDBI_B_P::DelAllRecord( std::string& error ) {
		Dbc* cursor = nullptr;
		std::string log_info = "";
		if( m_db->cursor( nullptr, &cursor, 0 ) != 0 ) {
			error = "获取游标失败！";
			return -1;
		}
		else {
			int32_t ret = 0;
			Dbt db_key, db_value;
			while( cursor->get( &db_key, &db_value, DB_NEXT ) == 0 ) {
				ret = cursor->del( 0 );
				if( ret != 0 ) {
					error = DbEnv::strerror( ret );
					return -2;
				}
			}
			m_db->sync( 0 );
		}
		return 0;
	}

	void SysDBI_B_P::PrintAllRecord() {
		Dbc* cursor = nullptr;
		std::string log_info = "";
		if( m_db->cursor( nullptr, &cursor, 0 ) != 0 ) {
			log_info = "获取游标失败！";
			m_syslog->LogWrite( basicx::syslog_level::c_error, m_log_cate, log_info );
		}
		else {
			Dbt db_key, db_value;
			while( cursor->get( &db_key, &db_value, DB_NEXT ) == 0 ) {
				std::string key_string( (char*)db_key.get_data(), db_key.get_size() );
				std::string value_string( (char*)db_value.get_data(), db_value.get_size() );
				FormatLibrary::StandardLibrary::FormatTo( log_info, "{0}:{1}", key_string, value_string );
				m_syslog->LogWrite( basicx::syslog_level::c_debug, m_log_cate, log_info );
			}
		}
	}

	SysDBI_B::SysDBI_B()
		: m_sysdbi_b_p( nullptr ) {
		try {
			m_sysdbi_b_p = new SysDBI_B_P();
		}
		catch( ... ) {}
	}

	SysDBI_B::~SysDBI_B() {
		if( m_sysdbi_b_p != nullptr ) {
			delete m_sysdbi_b_p;
			m_sysdbi_b_p = nullptr;
		}
	}

	int32_t SysDBI_B::OpenDB( std::string db_path, std::string db_file, uint32_t cache_size, std::string& error ) {
		return m_sysdbi_b_p->OpenDB( db_path, db_file, cache_size, error );
	}

	int32_t SysDBI_B::CloseDB( std::string& error ) {
		return m_sysdbi_b_p->CloseDB( error );
	}

	int32_t SysDBI_B::PutString( const char* key, const char* value, std::string& error ) {
		return m_sysdbi_b_p->PutString( key, value, error );
	}

	int32_t SysDBI_B::GetString( const char* key, std::string& value, std::string& error ) {
		return m_sysdbi_b_p->GetString( key, value, error );
	}

	int32_t SysDBI_B::PutBinary( const char* key, const char* value, int32_t length, std::string& error ) {
		return m_sysdbi_b_p->PutBinary( key, value, length, error );
	}

	int32_t SysDBI_B::GetBinary( const char* key, char*& value, int32_t& length, std::string& error ) {
		return m_sysdbi_b_p->GetBinary( key, value, length, error );
	}

	int32_t SysDBI_B::DelRecord( const char* key, std::string& error ) {
		return m_sysdbi_b_p->DelRecord( key, error );
	}

	int32_t SysDBI_B::DelAllRecord( std::string& error ) {
		return m_sysdbi_b_p->DelAllRecord( error );
	}

	void SysDBI_B::PrintAllRecord() {
		m_sysdbi_b_p->PrintAllRecord();
	}

} // namespace certify
