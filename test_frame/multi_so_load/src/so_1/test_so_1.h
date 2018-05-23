/*!
 *		Copyright(C),2012-2012
 * \file:		testclass1.h
 * \brief:		
 * \author:		
 * \version:		1.0
 * \Create:		2011-5-11
 * \modify£º	2018-5-23
 * \History:	
 * \Run:		
 * \Environment:
 *				
 */
#ifndef TEST_SO_1
#define TEST_SO_1
 
#include <cstdio>
#include "common.h"

////////////////////////////////////////////////////////////////////////////////////

class test_so_1
{
public:
	test_so_1();
	~test_so_1();
public:
	int handle_input(common_data* input_data);
	int func1();
public:
	int var_c1;
private:
};

////////////////////////////////////////////////////////////////////////////////////
#endif
