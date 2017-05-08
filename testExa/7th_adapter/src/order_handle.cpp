/*!
 *		Copyright(C),2011-2011
 * \file:		order_handle.cpp
 * \brief:		
 * \author:		chuai
 * \version:	1.0
 * \Create:		2017-5-8
 * \modify£º	
 * \History:	
 * \Run:		
 * \Environment:
 *				
 */

////////////////////////////////////////////////////////////////////////////////////

#include "order_handle.h"

Corder_handle::Corder_handle()
{
	m_var = 0;
}

Corder_handle::~Corder_handle()
{
}

int Corder_handle::testfunc()
{
	printf("This is test class in trunk!!!\n");
	return 0;
}

int Corder_handle::rules_init()
{
}

int Corder_handle::handle_order_biz(biz_data* biz_ptr, char* udata, int cid)
{
	// 
	// table -> chain -> rule -> (match, target)
	//
	
	std::vector<biz_param> param_vec;

	biz_table table("biz_test_handle_order", 1);
	biz_chain* chain_ptr = new biz_chain;
	//biz_rule*  rule_ptr  = new biz_rule("rule_1");
	biz_rule*  rule_ptr  = new biz_rule();
	t1_match*  mth1_ptr = new t1_match("match_1");
	t1_target* tgt1_ptr = new t1_target("target_1");

	rule_ptr->register_match(mth1_ptr, param_vec);
	rule_ptr->register_target(tgt1_ptr, param_vec);
	chain_ptr->register_rule(rule_ptr, 0);
	int32_t chain_id = table.register_chain(chain_ptr);
	if(chain_id == -1) {
		printf("register error!\n");
		abort();
	}

	table.handle_input(biz_ptr, chain_id);
}

bool t1_match::handle_input(biz_data* data)
{
	printf("|%s| match handle_input\n", who_.c_str());

	order_bd* order_ptr = (order_bd*)(data->field);
	if(order_ptr->contract == 1) {
		if(order_ptr->seq == 2 && order_ptr->price == 6) {
			printf("\t->order data right!\n");
		}
		else {
			printf("\t->order data wrong! 2\n");
			return false;
		}
	}
	else {
		printf("\t->order data wrong! 1\n");
		return false;
	}

	return true;
}

biz_ret t1_target::handle_input(biz_data* data)
{
	printf("|%s| target handle_input\n", who_.c_str());

	order_bd* order_ptr = (order_bd*)(data->field);
	if(order_ptr->contract == 1) {
		if(order_ptr->seq == 2 && order_ptr->price == 6) {
			printf("\t->in target order data right!\n");
		}
		else {
			printf("\t->in target order data wrong! 2\n");
			return RET_FAILED;
		}
	}
	else {
		printf("\t->in target order data wrong! 1\n");
		return RET_FAILED;
	}


	return RET_ACCEPT;
}

////////////////////////////////////////////////////////////////////////////////////

