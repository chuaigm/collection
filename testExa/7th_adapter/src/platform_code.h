/*!
 *		Copyright(C),2017-2017
 * \file:		platform_code.h
 * \brief:		
 * \author:		à¨¹úÃ÷
 * \version:		1.0
 * \Create:		2017-5-06
 * \modify£º	
 * \History:	
 * \Run:		
 * \Environment:
 *				
 */
#ifndef _PLATFORM_CODE_
#define _PLATFORM_CODE_
 
#include <cstdio>
#include "data_struct.h"

////////////////////////////////////////////////////////////////////////////////////

class Cplatform_code
{
public:
	Cplatform_code();
	~Cplatform_code();
public:
	int func1();
	//int handle_order_req(order_req* fld, char* userdata, int cid);
	int handle_order_req(fld_1<order_bd>* fld, char* userdata, int cid);
	int handle_mbl_quot(fld_1pn<best_quot_bd, mbl_bd>* fld, char* userdata, int cid);
	int pub_data(fld_1pn<best_quot_bd, mbl_bd>* fld, uint32_t len,uint32_t opcode, int cid);
	int run();
public:
	int var_c1;
private:
};

////////////////////////////////////////////////////////////////////////////////////
#endif
