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
#include "order_handle.h"

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
	fld_1<order_bd>* order_ptr = new fld_1<order_bd>;
	order_ptr->fld.contract = 1;
	order_ptr->fld.seq      = 2;
	order_ptr->fld.time     = 3;
	order_ptr->fld.bs_flag  = 4;
	order_ptr->fld.eo_flag  = 5;
	order_ptr->fld.price    = 6;
	//order_bd _order;

	char usr_data[64] ={0};

	//2. do handle order, entrance
	handle_order_req(order_ptr, usr_data, 10);

	return 0;
}

//int Cplatform_code::handle_order_req(order_req* fld, char* userdata, int cid)
int Cplatform_code::handle_order_req(fld_1<order_bd>* fld_ptr, char* userdata, int cid)
{
	printf("handle order request\n");
	
	biz_data data;
	CFTCPField* ftcp_ptr = &(fld_ptr->fld);
	data.field = ftcp_ptr;
		
	Corder_handle ord_handle;
	ord_handle.handle_order_biz(&data, userdata, cid);

	return 0;
}

int Cplatform_code::handle_mbl_quot(fld_1pn<best_quot_bd, mbl_bd>* fld_ptr, char* userdata, int cid)
{
	printf("handle best mbl quot\n");

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////

