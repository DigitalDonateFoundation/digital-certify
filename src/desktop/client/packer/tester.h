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

#ifndef CERTIFY_PACKER_TESTER_H
#define CERTIFY_PACKER_TESTER_H

#include <common/common.h>

#include "packer.h"

namespace certify {

	void Test_Packer() {
		int32_t ret = 0;
		std::string error = "";
		std::string log_info = "";
		std::string log_cate = "<PACKER>";
		basicx::SysLog_S* syslog = basicx::SysLog_S::GetInstance();

		Packer packer;

		ret = packer.OpenDB( "I:\\test", "I:\\test\\test.db", 10 * 1024 * 1024, error );
		if( ret != 0 ) {
			FormatLibrary::StandardLibrary::FormatTo( log_info, "打开 数据库 失败！{0} {1}", ret, error );
			syslog->LogWrite( basicx::syslog_level::c_error, log_cate, log_info );
			return;
		}

		std::string string_test = "";
		std::string value_test = "String：啦啦啦0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
		ret = packer.PutString( "key_test_1", value_test.c_str(), error );
		if( ret != 0 ) {
			FormatLibrary::StandardLibrary::FormatTo( log_info, "存储 字符串 失败！{0} {1}", ret, error );
			syslog->LogWrite( basicx::syslog_level::c_error, log_cate, log_info );
		}
		else {
			ret = packer.GetString( "key_test_1", string_test, error );
			if( ret != 0 ) {
				FormatLibrary::StandardLibrary::FormatTo( log_info, "获取 字符串 失败！{0} {1}", ret, error );
				syslog->LogWrite( basicx::syslog_level::c_error, log_cate, log_info );
			}
			else {
				syslog->LogWrite( basicx::syslog_level::c_info, log_cate, string_test );
			}
		}

		int32_t length_test = 0;
		char* binary_test = nullptr;
		char buffer_test[256] = "Binary：abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789啦啦啦";
		ret = packer.PutBinary( "key_test_2", buffer_test, 256, error );
		if( ret != 0 ) {
			FormatLibrary::StandardLibrary::FormatTo( log_info, "存储 二进制 失败！{0} {1}", ret, error );
			syslog->LogWrite( basicx::syslog_level::c_error, log_cate, log_info );
		}
		else {
			ret = packer.GetBinary( "key_test_2", binary_test, length_test, error );
			if( ret != 0 ) {
				FormatLibrary::StandardLibrary::FormatTo( log_info, "获取 二进制 失败！{0} {1}", ret, error );
				syslog->LogWrite( basicx::syslog_level::c_error, log_cate, log_info );
			}
			else {
				std::string result_test( binary_test, length_test );
				syslog->LogWrite( basicx::syslog_level::c_info, log_cate, result_test );
			}
		}

		for( size_t i = 0; i < 5; i++ ) {
			std::string string_keys = "";
			FormatLibrary::StandardLibrary::FormatTo( string_keys, "keys_{0}", i );
			char buffer_test[64] = "abcdefghijklmnopqrstuvwxyz";
			ret = packer.PutBinary( string_keys.c_str(), buffer_test, 64, error );
			if( ret != 0 ) {
				FormatLibrary::StandardLibrary::FormatTo( log_info, "存储 测试集 失败！{0} {1}", ret, error );
				syslog->LogWrite( basicx::syslog_level::c_error, log_cate, log_info );
			}
		}
		packer.PrintAllRecord();

		ret = packer.DelRecord( "keys_1", error );
		if( ret != 0 ) {
			FormatLibrary::StandardLibrary::FormatTo( log_info, "删除 测试项 失败！{0} {1}", ret, error );
			syslog->LogWrite( basicx::syslog_level::c_error, log_cate, log_info );
		}
		ret = packer.DelRecord( "keys_3", error );
		if( ret != 0 ) {
			FormatLibrary::StandardLibrary::FormatTo( log_info, "删除 测试项 失败！{0} {1}", ret, error );
			syslog->LogWrite( basicx::syslog_level::c_error, log_cate, log_info );
		}
		packer.PrintAllRecord();

		ret = packer.DelAllRecord( error );
		if( ret != 0 ) {
			FormatLibrary::StandardLibrary::FormatTo( log_info, "删除 所有项 失败！{0} {1}", ret, error );
			syslog->LogWrite( basicx::syslog_level::c_error, log_cate, log_info );
		}
		packer.PrintAllRecord();

		ret = packer.CloseDB( error );
		if( ret != 0 ) {
			FormatLibrary::StandardLibrary::FormatTo( log_info, "关闭 数据库 失败！{0} {1}", ret, error );
			syslog->LogWrite( basicx::syslog_level::c_error, log_cate, log_info );
		}

		return;
	}

} // namespace certify

#endif // CERTIFY_PACKER_TESTER_H
