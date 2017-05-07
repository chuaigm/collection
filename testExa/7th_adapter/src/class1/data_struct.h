/*!
 *		Copyright(C),2017-2017
 * \file:		data_struct.h
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
#ifndef _DATA_STRUCT_
#define _DATA_STRUCT_
 
////////////////////////////////////////////////////////////////////////////////////

// define template
template <typename T>
class fld_1
{
public:
//	fld_1(){};
//	~fld_1(){};
	T fld;
};

/*
template <typename T1, typename T2>
class fld_2
{
	T1 fld1;
	T2 fld2;
};

template <typename T>
class fld_n
{
	int32_t num;
	T fld[];
};

template <typename T1, typename T2>
class fld_1pn
{
	T1 fld1;
	int32_t num;
	T2 fld[];
};
*/

// define base struct

struct err_bd
{
	int32_t err_code;
	char err_msg[64];
};

struct order_bd
{
	int32_t contract;
	int64_t seq;
	int64_t time;
	int8_t bs_flag;
	int8_t eo_flag;
	int64_t price;
};

struct best_quot_bd
{
	int32_t contract;
	int64_t seq;
	int64_t time;
	int64_t price;
	int64_t vol;
	int64_t uphigh;
	int64_t downlow;
};

struct mbl_bd
{
	int32_t contract;
	int64_t seq;
	int64_t time;
	int64_t price;
	int64_t vol;
};


// define package struct

//fld_1<order_bd>               order_req;
//fld_2<order_bd, err_bd>       order_rsp;
//fld_n<order_bd>               batch_order_req;
//fld_1pn<best_quot_bd, mbl_bd> depth_quot;


////////////////////////////////////////////////////////////////////////////////////
#endif
