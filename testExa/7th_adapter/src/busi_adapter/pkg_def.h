//**********************************************************************
// 7th 数据包定义
// powered by cgm
// date: 2017-5-16
//
// 业务包定义原则：
// 		1. 名字构成pkg_xxx_xxx，最大不超过四个分词
// 		2. 分词内容目前直接来自于TID名字的定义
// 		3. (未实现)分词方法，解析原TID名字，将一些关键字进行剥离，如ORDER,MATCH,TRADER
// 		4. (未实现)由3中所述的分词方法加下划线大写字母加TID前缀，得到TID名字定义
// 		5. (未实现)由3中所述的分词方法加下划线小写字母加pkg前缀，得到包结构定义
// 		6. (未实现)由3中所述的分词方法加下划线小写字母加handle前缀，得到处理函数定义
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
