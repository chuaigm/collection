/*!
 *		Copyright(C),2017-2017
 * \file:		platform_code.cpp
 * \brief:		
 * \author:		à¨¹úÃ÷
 * \version:		1.0
 * \Create:		2011-5-11
 * \modify£º	2012-6-14
 * \History:	
 * \Run:		
 * \Environment:
 *				
 */

////////////////////////////////////////////////////////////////////////////////////

#include "platform_code.h"

Cplatform_code::Cplatform_code()
{
}

Cplatform_code::~Cplatform_code()
{
}

int Cplatform_code::func1()
{
	printf("This is test class 1 !!!\n");
	return 0;
}

int Cplatform_code::run()
{
	printf("Platform Code -> Run!\n");

	//1. pretend a input
	//order_req* _order = new order_req();
	fld_1<order_bd>* _order;
	//order_bd _order;
	char usr_data[64] ={0};

	//2. do handle order, entrance
	handle_order_req(_order, usr_data, 10);

	return 0;
}

//int Cplatform_code::handle_order_req(order_req* fld, char* userdata, int cid)
int Cplatform_code::handle_order_req(fld_1<order_bd>* fld, char* userdata, int cid)
{
	printf("handle order request\n");

	return 0;
}
////////////////////////////////////////////////////////////////////////////////////
