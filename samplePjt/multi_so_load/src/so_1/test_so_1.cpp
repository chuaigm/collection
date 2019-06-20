/*!
 *		Copyright(C),2012-2012
 * \file:		testclass1.cpp
 * \brief:		
 * \author:		à¨¹úÃ÷
 * \version:		1.0
 * \Create:		2011-5-11
 * \modify£º	2018-5-23
 * \History:	
 * \Run:		
 * \Environment:
 *				
 */

////////////////////////////////////////////////////////////////////////////////////

#include "test_so_1.h"

test_so_1::test_so_1()
{
}

test_so_1::~test_so_1()
{
}

int test_so_1::handle_input(common_data* input_data)
{
	printf("input_data->m_var = %d\n",input_data->m_var);

	return 0;
}

int test_so_1::func1()
{
	printf("This is test so 1 !!!\n");
	return 0;
}


////////////////////////////////////////////////////////////////////////////////////
