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

#include <stdlib.h>
#include <stdint.h>

#include <QApplication>
#include <QMainWindow>
#include <QPushButton>

#include "certify.h"

int32_t main( int32_t argc, char* argv[] ) {
	QApplication app( argc, argv );

	QMainWindow main_window;
	main_window.show();

	QPushButton btn( "hello world" );
	btn.resize( 200, 100 );
	btn.show();

	return app.exec();
}

namespace certify {

} // namespace certify
