//**********************************************************************
// 7th ���ݰ�����
// powered by cgm
// date: 2017-5-16
//
// ҵ�������ԭ��
// 		1. ���ֹ���pkg_xxx_xxx����󲻳����ĸ��ִ�
// 		2. �ִ�����Ŀǰֱ��������TID���ֵĶ���
// 		3. (δʵ��)�ִʷ���������ԭTID���֣���һЩ�ؼ��ֽ��а��룬��ORDER,MATCH,TRADER
// 		4. (δʵ��)��3�������ķִʷ������»��ߴ�д��ĸ��TIDǰ׺���õ�TID���ֶ���
// 		5. (δʵ��)��3�������ķִʷ������»���Сд��ĸ��pkgǰ׺���õ����ṹ����
// 		6. (δʵ��)��3�������ķִʷ������»���Сд��ĸ��handleǰ׺���õ�����������
// 		7. 
//**********************************************************************
#ifndef __PKG_DEF_H__
#define __PKG_DEF_H__

#include "fld_def.h"

#define DECL_TID(id) enum{TID = id}
#define DECL_LABEL() \
public: \
    uint32_t label_{0}; \
	uint32_t get_label() \
	{ \
		return label_; \
	}
#define DECL_LABEL_MOCK() \
public: \
	uint32_t get_label() \
	{ \
		return 0; \
	}

#endif
