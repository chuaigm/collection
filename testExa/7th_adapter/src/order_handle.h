/*!
 * \file:		
 * \Create:		2017-5-8
 */
#ifndef _ORDER_HANDLE_
#define _ORDER_HANDLE_
 
#include "data_struct.h"
#include "biz_filter.h"
 
////////////////////////////////////////////////////////////////////////////////////

class t1_match : public biz_match
{
public:
	t1_match (std::string who) : who_(who)
	{}

	bool handle_input(biz_data* data);
private:
	std::string who_;
};

class t1_target : public biz_target
{
public:
	t1_target (std::string who) : who_(who)
	{}

	biz_ret handle_input(biz_data* data);
private:
	std::string who_;
};


class Corder_handle
{
public:
	Corder_handle();
	~Corder_handle();
public:
	int testfunc();
	int rules_init();
	int handle_order_biz(biz_data* biz_ptr, char* udata, int cid);
public:
	int m_var;
private:
};

////////////////////////////////////////////////////////////////////////////////////
#endif

