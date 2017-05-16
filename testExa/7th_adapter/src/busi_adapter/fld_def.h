//**********************************************************************
// 7th 数据结构，域定义示例
// cgm
// 2017-5-16
// 转换类型一些可能会用到的命令
// %s/\<UINT4\>/uint32_t/gc
// %s/\<INT4\>/int32_t/gc
// %s/\<BYTE\>/uint8_t/gc
// %s/\<INT1\>/int8_t/gc
// %s/\<REAL8\>/int64_t/gc
// %s/BYTEARRAY<\(.*\)>\s*\(.*\);/char                \2[\1];/gc
// %s/CFld\([A-Z][a-z]\+\)/fld_\l\1/gc
// %s/CFld\([A-Z][a-z]\+\)\([A-Z][a-z]\+\)/fld_\l\1_\l\2/gc
//
// 业务域定义原则：
// 		1. 名字构成fld_xxx_xxx，最大不超过四个分词
// 		2. 分词内容目前直接直接来自于六期原定义的名字
// 		3. 
//**********************************************************************
#ifndef __FLD_DEF_H__
#define __FLD_DEF_H__

#define DEF_FIELD_ID(id) enum{field_id = id}

/* 响应域 */
class fld_rsp_msg :public CFTCPField
{
public:
	fld_rsp_msg()
	{
		memset(&ErrCode,0,sizeof(fld_rsp_msg) - sizeof(CFTCPField));
	}
public:
	int32_t             ErrCode;                           /*错误码*/
	char                RspMsg[40];                        /*错误描述信息*/
	uint64_t            TimeStamp[19];                     /*时间戳*/
};

/* 客户信息域 */
class fld_client :public CFTCPField
{
public:
	fld_client()
	{
		memset(ClientID,0,sizeof(fld_client) - sizeof(CFTCPField));
	}
public:
	char                ClientID[16];                      /*客户号*/
	char                MemberID[8];                       /*会员号*/
	char                ClientName[80];                    /*客户名称*/
	uint8_t             ClientSort;                        /*客户类别*/
	uint8_t             ClientProperty;                    /*客户性质*/
};

/* 定单信息域 */
class fld_order :public CFTCPField
{
public:
	fld_order()
	{
		memset(&SysOrderNo,0,sizeof(fld_order) - sizeof(CFTCPField));
	}
public:
	uint32_t               SysOrderNo;                    /*系统委托号*/
	uint32_t               LocalOrderNo;                  /*本地委托号*/
	uint32_t               TID;                           /*事务编号*/
	char                   TraderNo[16];                  /*委托席位号*/
	char                   MemberID[8];                   /*会员编号*/
	char                   ClientID[16];                  /*客户编号*/
	char                   ContractID[80];                /*合约号*/
	uint8_t                BsFlag;                        /*买卖标志*/
	uint8_t                EoFlag;                        /*开平标志*/
	uint8_t                ShFlag;                        /*投保标志*/
	int64_t                StopPrice;                     /*止损(盈)价*/
	int64_t                Price;                         /*价格*/
	uint32_t               Qty;                           /*数量*/
	uint8_t                OrderType;                     /*定单类型*/
	uint8_t                OrderAttr;                     /*定单属性*/
	int8_t                 MatchSession;                  /*报单成交时间(空)*/
	char                   ValidThrough[8];               /*有效时间约束(空)*/
	uint32_t               MinQty;                        /*最小成交量(空)*/
	int8_t                 AutoSuspend;                   /*自动挂起标志(空)*/
	char                   OrderTime[12];                 /*委托时间*/
	char                   MsgRef[6];                     /*用户自定义数据。可打印ASCII字符(空)*/
	uint8_t                OrderSort;                     /*定单类别*/
	uint8_t                OrderSrc;                      /*定单来源*/
	uint8_t                ForceOffsetReason;             /*强平原因*/
	char                   ArbiContractID[80];            /*套利合约号*/
	uint32_t               OrderBatchNo;                  /*委托批次号*/
	int8_t                 IsMktMk;                       /*是否做市商下单*/
	char                   ClearAccountNo[14];            /*结算帐号*/
	uint8_t                TradeType;                     /*交易类型*/
};

/* 定单状态域 */
class fld_order_status :public CFTCPField
{
public:
	fld_order_status()
	{
		memset(&SysOrderNo,0,sizeof(fld_order_status) - sizeof(CFTCPField));
	}
public:
	uint32_t               SysOrderNo;                    /*系统委托号*/
	uint32_t               LocalOrderNo;                  /*本地委托号*/
	char                   TraderNo[16];                  /*委托席位号*/
	char                   MemberID[8];                   /*会员编号*/
	char                   ClientID[16];                  /*客户编号*/
	char                   ContractID[80];                /*合约号*/
	uint8_t                BsFlag;                        /*买卖标志*/
	uint8_t                EoFlag;                        /*开平标志*/
	uint8_t                ShFlag;                        /*投保标志*/
	int64_t                StopPrice;                     /*止损(盈)价*/
	int64_t                Price;                         /*价格*/
	uint8_t                OrderType;                     /*定单类型*/
	uint8_t                OrderAttr;                     /*定单属性*/
	int8_t                 MatchSession;                  /*报单成交时间(空)*/
	char                   ValidThrough[8];               /*有效时间约束（空）*/
	uint32_t               MinQty;                        /*最小成交量（空）*/
	int8_t                 AutoSuspend;                   /*自动挂起标志（空）*/
	char                   OrderTime[12];                 /*委托时间*/
	char                   MsgRef[6];                     /*用户自定义数据。可打印ASCII字符（空）*/
	uint8_t                OrderSort;                     /*定单类别*/
	uint8_t                OrderSrc;                      /*定单来源*/
	uint8_t                OrderStatus;                   /*定单状态*/
	uint32_t               MatchQty;                      /*成交量*/
	char                   TrigTime[12];                  /*触发时间*/
	char                   CancelTime[12];                /*撤单时间*/
	char                   CancelTraderNo[16];            /*撤单席位号*/
	uint8_t                ForceOffsetReason;             /*强平原因*/
	char                ArbiContractID[80];                /*套利合约号*/
	uint32_t               OrderBatchNo;                  /*委托批次号*/
	int8_t                IsMktMk;                       /*是否做市商下单*/
	char                ClearAccountNo[14];                /*结算帐号*/
	uint32_t               QtyTotOrginal;                 /*原始总申报数量（以手为单位）*/
	uint32_t               QtyTot;                        /*剩余总申报数量（以手为单位）*/
	char                SuspendTime[12];                   /*挂起时间（空）*/
	char                UpdTime[12];                       /*最后修改时间*/
	int64_t               Margin;                        /*保证金*/
	uint32_t               ForceGroupID;                  /*强平组号（空）*/
	int64_t               LastMatchPrice;                /*最新成交价格*/
	uint8_t                TradeType;                     /*交易类型*/
};

/* 成交单信息域 */
class fld_match :public CFTCPField
{
public:
	fld_match()
	{
		memset(ContractID,0,sizeof(fld_match) - sizeof(CFTCPField));
	}
public:
	char                ContractID[80];                    /*合约号*/
	int8_t                ContractVersion;               /*合约版本号（空）*/
	uint32_t               TID;                           /*事务编号*/
	int8_t                CancelFlag;                    /*成交是否被取消（空）*/
	char                CancelDate[8];                    /*取消日期（空）*/
	char                CancelTime[12];                    /*取消时间（空）*/
	uint32_t               MatchNo;                       /*成交号*/
	char                MatchDate[8];                     /*成交日期*/
	char                MatchTime[12];                     /*成交时间*/
	char                ClearDate[8];                     /*清算日期*/
	int64_t               Price;                         /*成交价格*/
	uint32_t               Qty;                           /*成交数量*/
	uint32_t               ArbiQty;                       /*套利成交手数*/
	uint32_t               SysOrderNo;                    /*系统委托号*/
	char                TraderNo[16];                      /*席位号*/
	uint8_t                BsFlag;                        /*买卖标志*/
	uint8_t                EoFlag;                        /*开平标志*/
	uint8_t                ShFlag;                        /*投保标志*/
	char                MemberID[8];                      /*会员号*/
	char                ClientID[16];                      /*客户号*/
	uint32_t               LocalID;                       /*报单本地编码*/
	char                ArbiContractID[80];                /*套利合约号*/
	uint32_t               OrderBatchNo;                  /*委托批次号*/
	uint32_t               MatchBatchNo;                  /*成交批次号*/
	uint8_t                OrderSrc;                      /*定单来源*/
};

