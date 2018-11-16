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

#ifndef CERTIFY_CERTIFY_PROJECT_LIST_ITEM_H
#define CERTIFY_CERTIFY_PROJECT_LIST_ITEM_H

#include <QtCore/QString>
#include <QtCore/QMetaType>

namespace certify {

	typedef enum {
		S_R,
		S_G,
		S_B,
	} ItemStatus;

	struct ItemData {
		QString m_name;
		QString m_phone;
	};

	Q_DECLARE_METATYPE( ItemData )

} // namespace certify

#endif // CERTIFY_CERTIFY_PROJECT_LIST_ITEM_H
