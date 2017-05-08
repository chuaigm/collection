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

	// a best field with 2 mbl
	uint32_t len = sizeof(best_quot_bd)+2*sizeof(mbl_bd);
	fld_1pn<best_quot_bd, mbl_bd>* quot_ptr = (fld_1pn<best_quot_bd, mbl_bd>*) new char[len];
	quot_ptr->num=2;
	quot_ptr->fld1.contract = fld_ptr->fld.contract;
	quot_ptr->fld1.seq      = 2;
	quot_ptr->fld1.time     = 3;
	quot_ptr->fld1.price    = 4;
	quot_ptr->fld1.vol      = 5;
	quot_ptr->fld1.uphigh   = 6;
	quot_ptr->fld1.downlow  = 7;

	quot_ptr->fld[0].contract = quot_ptr->fld1.contract;
	quot_ptr->fld[0].seq      = 2;
	quot_ptr->fld[0].time     = 3;
	quot_ptr->fld[0].price    = 1111;
	quot_ptr->fld[0].vol      = 11;

	quot_ptr->fld[1].contract = quot_ptr->fld1.contract;
	quot_ptr->fld[1].seq      = 2;
	quot_ptr->fld[1].time     = 3;
	quot_ptr->fld[1].price    = 2222;
	quot_ptr->fld[1].vol      = 2222;

	// send package
	pub_data(quot_ptr, len, 25, cid);

	// code just for test below, simulate when got the package
	handle_mbl_quot(quot_ptr, userdata, cid);

	return 0;
}

int Cplatform_code::handle_mbl_quot(fld_1pn<best_quot_bd, mbl_bd>* fld_ptr, char* userdata, int cid)
{
	printf("handle best mbl quot\n");

	best_quot_bd* best_ptr = &(fld_ptr->fld1);
	if(best_ptr->downlow == 7) {
		printf("\t->in plat best quot right!\n");
	}
	else {
		printf("\t->[xxx] in plat best quot wrong!\n");
	}

	mbl_bd* mbl_ptr = NULL;
	for(int i=0; i<fld_ptr->num; ++i)
	{
		mbl_ptr = &(fld_ptr->fld[i]);
		if (i==0) {
		if (mbl_ptr->price == 1111) {
			printf("\t->in plat mbl[%d] right!\n", i);
		}
		else {
			printf("\t->[xxx] in plat mbl[%d] wrong!\n", i);
		}
		}

		if (i==1) {
		if (mbl_ptr->price == 2222) {
			printf("\t->in plat mbl[%d] right!\n", i);
		}
		else {
			printf("\t->[xxx] in plat mbl[%d] wrong!\n", i);
		}
		}
	}

	return 0;
}

int Cplatform_code::pub_data(fld_1pn<best_quot_bd, mbl_bd>* fld, uint32_t len,uint32_t opcode, int cid)
{

}
////////////////////////////////////////////////////////////////////////////////////

