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

#ifndef CERTIFY_GLOBAL_COMPILE_H
#define CERTIFY_GLOBAL_COMPILE_H

//---------- ������� ----------//

#define CERTIFY_GLOBAL_EXP
//#define CERTIFY_GLOBAL_IMP

#define CERTIFY_SHARES_EXP
//#define CERTIFY_SHARES_IMP

//---------- ���ý��� ----------//

#ifdef CERTIFY_GLOBAL_EXP
    #define CERTIFY_GLOBAL_EXPIMP __declspec(dllexport)
#endif

#ifdef CERTIFY_GLOBAL_IMP
    #define CERTIFY_GLOBAL_EXPIMP __declspec(dllimport)
#endif

//------------------------------//

#ifdef CERTIFY_SHARES_EXP
    #define CERTIFY_SHARES_EXPIMP __declspec(dllexport)
#endif

#ifdef CERTIFY_SHARES_IMP
    #define CERTIFY_SHARES_EXPIMP __declspec(dllimport)
#endif

//------------------------------//

#endif // CERTIFY_GLOBAL_COMPILE_H
