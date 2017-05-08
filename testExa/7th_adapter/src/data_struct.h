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

#include "stdint.h"
#include "FTCPField.h"
 
////////////////////////////////////////////////////////////////////////////////////
//class CFTCPField
//{
//};

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

template <typename T1, typename T2>
class fld_2
{
	T1 fld1;
	T2 fld2;
};

template <typename T1, typename T2, typename T3>
class fld_3
{
	T1 fld1;
	T2 fld2;
	T3 fld3;
};

template <typename T>
class fld_n
{
	int32_t num;
	T fld[];
};

template <typename T1, typename Tn>
class fld_1pn
{
	T1 fld1;
	int32_t num;
	Tn fld[];
};

template <typename T1, typename T2, typename Tn>
class fld_2pn
{
	T1 fld1;
	T2 fld2;
	int32_t num;
	Tn fld[];
};

////////////////////////////////////////////////////////////////////////////////////

// define base struct

//struct rsp_msg
class rsp_msg : public CFTCPField
{
public:
	int32_t err_code;
	int64_t time_stamp;
	char err_msg[64];
};

class order_bd : public CFTCPField
{
public:
	int32_t contract;
	int64_t seq;
	int64_t time;
	int8_t bs_flag;
	int8_t eo_flag;
	int64_t price;
};

class best_quot_bd : public CFTCPField
{
public:
	int32_t contract;
	int64_t seq;
	int64_t time;
	int64_t price;
	int64_t vol;
	int64_t uphigh;
	int64_t downlow;
};

class mbl_bd : public CFTCPField
{
public:
	int32_t contract;
	int64_t seq;
	int64_t time;
	int64_t price;
	int64_t vol;
};


// define package struct

//fld_1<order_bd>               order_req;
//fld_2<order_bd, rsp_msg>       order_rsp;
//fld_n<order_bd>               batch_order_req;
//fld_1pn<best_quot_bd, mbl_bd> depth_quot;


////////////////////////////////////////////////////////////////////////////////////
#endif
