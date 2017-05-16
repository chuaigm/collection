//**********************************************************************
// 7th ���ݽṹ������ʾ��
// cgm
// 2017-5-16
// ת������һЩ���ܻ��õ�������
// %s/\<UINT4\>/uint32_t/gc
// %s/\<INT4\>/int32_t/gc
// %s/\<BYTE\>/uint8_t/gc
// %s/\<INT1\>/int8_t/gc
// %s/\<REAL8\>/int64_t/gc
// %s/BYTEARRAY<\(.*\)>\s*\(.*\);/char                \2[\1];/gc
// %s/CFld\([A-Z][a-z]\+\)/fld_\l\1/gc
// %s/CFld\([A-Z][a-z]\+\)\([A-Z][a-z]\+\)/fld_\l\1_\l\2/gc
//
// ҵ������ԭ��
// 		1. ���ֹ���fld_xxx_xxx����󲻳����ĸ��ִ�
// 		2. �ִ�����Ŀǰֱ��ֱ������������ԭ���������
// 		3. 
//**********************************************************************
#ifndef __FLD_DEF_H__
#define __FLD_DEF_H__

#define DEF_FIELD_ID(id) enum{field_id = id}

/* ��Ӧ�� */
class fld_rsp_msg :public CFTCPField
{
public:
	fld_rsp_msg()
	{
		memset(&ErrCode,0,sizeof(fld_rsp_msg) - sizeof(CFTCPField));
	}
public:
	int32_t             ErrCode;                           /*������*/
	char                RspMsg[40];                        /*����������Ϣ*/
	uint64_t            TimeStamp[19];                     /*ʱ���*/
};

/* �ͻ���Ϣ�� */
class fld_client :public CFTCPField
{
public:
	fld_client()
	{
		memset(ClientID,0,sizeof(fld_client) - sizeof(CFTCPField));
	}
public:
	char                ClientID[16];                      /*�ͻ���*/
	char                MemberID[8];                       /*��Ա��*/
	char                ClientName[80];                    /*�ͻ�����*/
	uint8_t             ClientSort;                        /*�ͻ����*/
	uint8_t             ClientProperty;                    /*�ͻ�����*/
};

/* ������Ϣ�� */
class fld_order :public CFTCPField
{
public:
	fld_order()
	{
		memset(&SysOrderNo,0,sizeof(fld_order) - sizeof(CFTCPField));
	}
public:
	uint32_t               SysOrderNo;                    /*ϵͳί�к�*/
	uint32_t               LocalOrderNo;                  /*����ί�к�*/
	uint32_t               TID;                           /*������*/
	char                   TraderNo[16];                  /*ί��ϯλ��*/
	char                   MemberID[8];                   /*��Ա���*/
	char                   ClientID[16];                  /*�ͻ����*/
	char                   ContractID[80];                /*��Լ��*/
	uint8_t                BsFlag;                        /*������־*/
	uint8_t                EoFlag;                        /*��ƽ��־*/
	uint8_t                ShFlag;                        /*Ͷ����־*/
	int64_t                StopPrice;                     /*ֹ��(ӯ)��*/
	int64_t                Price;                         /*�۸�*/
	uint32_t               Qty;                           /*����*/
	uint8_t                OrderType;                     /*��������*/
	uint8_t                OrderAttr;                     /*��������*/
	int8_t                 MatchSession;                  /*�����ɽ�ʱ��(��)*/
	char                   ValidThrough[8];               /*��Чʱ��Լ��(��)*/
	uint32_t               MinQty;                        /*��С�ɽ���(��)*/
	int8_t                 AutoSuspend;                   /*�Զ������־(��)*/
	char                   OrderTime[12];                 /*ί��ʱ��*/
	char                   MsgRef[6];                     /*�û��Զ������ݡ��ɴ�ӡASCII�ַ�(��)*/
	uint8_t                OrderSort;                     /*�������*/
	uint8_t                OrderSrc;                      /*������Դ*/
	uint8_t                ForceOffsetReason;             /*ǿƽԭ��*/
	char                   ArbiContractID[80];            /*������Լ��*/
	uint32_t               OrderBatchNo;                  /*ί�����κ�*/
	int8_t                 IsMktMk;                       /*�Ƿ��������µ�*/
	char                   ClearAccountNo[14];            /*�����ʺ�*/
	uint8_t                TradeType;                     /*��������*/
};

/* ����״̬�� */
class fld_order_status :public CFTCPField
{
public:
	fld_order_status()
	{
		memset(&SysOrderNo,0,sizeof(fld_order_status) - sizeof(CFTCPField));
	}
public:
	uint32_t               SysOrderNo;                    /*ϵͳί�к�*/
	uint32_t               LocalOrderNo;                  /*����ί�к�*/
	char                   TraderNo[16];                  /*ί��ϯλ��*/
	char                   MemberID[8];                   /*��Ա���*/
	char                   ClientID[16];                  /*�ͻ����*/
	char                   ContractID[80];                /*��Լ��*/
	uint8_t                BsFlag;                        /*������־*/
	uint8_t                EoFlag;                        /*��ƽ��־*/
	uint8_t                ShFlag;                        /*Ͷ����־*/
	int64_t                StopPrice;                     /*ֹ��(ӯ)��*/
	int64_t                Price;                         /*�۸�*/
	uint8_t                OrderType;                     /*��������*/
	uint8_t                OrderAttr;                     /*��������*/
	int8_t                 MatchSession;                  /*�����ɽ�ʱ��(��)*/
	char                   ValidThrough[8];               /*��Чʱ��Լ�����գ�*/
	uint32_t               MinQty;                        /*��С�ɽ������գ�*/
	int8_t                 AutoSuspend;                   /*�Զ������־���գ�*/
	char                   OrderTime[12];                 /*ί��ʱ��*/
	char                   MsgRef[6];                     /*�û��Զ������ݡ��ɴ�ӡASCII�ַ����գ�*/
	uint8_t                OrderSort;                     /*�������*/
	uint8_t                OrderSrc;                      /*������Դ*/
	uint8_t                OrderStatus;                   /*����״̬*/
	uint32_t               MatchQty;                      /*�ɽ���*/
	char                   TrigTime[12];                  /*����ʱ��*/
	char                   CancelTime[12];                /*����ʱ��*/
	char                   CancelTraderNo[16];            /*����ϯλ��*/
	uint8_t                ForceOffsetReason;             /*ǿƽԭ��*/
	char                ArbiContractID[80];                /*������Լ��*/
	uint32_t               OrderBatchNo;                  /*ί�����κ�*/
	int8_t                IsMktMk;                       /*�Ƿ��������µ�*/
	char                ClearAccountNo[14];                /*�����ʺ�*/
	uint32_t               QtyTotOrginal;                 /*ԭʼ���걨����������Ϊ��λ��*/
	uint32_t               QtyTot;                        /*ʣ�����걨����������Ϊ��λ��*/
	char                SuspendTime[12];                   /*����ʱ�䣨�գ�*/
	char                UpdTime[12];                       /*����޸�ʱ��*/
	int64_t               Margin;                        /*��֤��*/
	uint32_t               ForceGroupID;                  /*ǿƽ��ţ��գ�*/
	int64_t               LastMatchPrice;                /*���³ɽ��۸�*/
	uint8_t                TradeType;                     /*��������*/
};

/* �ɽ�����Ϣ�� */
class fld_match :public CFTCPField
{
public:
	fld_match()
	{
		memset(ContractID,0,sizeof(fld_match) - sizeof(CFTCPField));
	}
public:
	char                ContractID[80];                    /*��Լ��*/
	int8_t                ContractVersion;               /*��Լ�汾�ţ��գ�*/
	uint32_t               TID;                           /*������*/
	int8_t                CancelFlag;                    /*�ɽ��Ƿ�ȡ�����գ�*/
	char                CancelDate[8];                    /*ȡ�����ڣ��գ�*/
	char                CancelTime[12];                    /*ȡ��ʱ�䣨�գ�*/
	uint32_t               MatchNo;                       /*�ɽ���*/
	char                MatchDate[8];                     /*�ɽ�����*/
	char                MatchTime[12];                     /*�ɽ�ʱ��*/
	char                ClearDate[8];                     /*��������*/
	int64_t               Price;                         /*�ɽ��۸�*/
	uint32_t               Qty;                           /*�ɽ�����*/
	uint32_t               ArbiQty;                       /*�����ɽ�����*/
	uint32_t               SysOrderNo;                    /*ϵͳί�к�*/
	char                TraderNo[16];                      /*ϯλ��*/
	uint8_t                BsFlag;                        /*������־*/
	uint8_t                EoFlag;                        /*��ƽ��־*/
	uint8_t                ShFlag;                        /*Ͷ����־*/
	char                MemberID[8];                      /*��Ա��*/
	char                ClientID[16];                      /*�ͻ���*/
	uint32_t               LocalID;                       /*�������ر���*/
	char                ArbiContractID[80];                /*������Լ��*/
	uint32_t               OrderBatchNo;                  /*ί�����κ�*/
	uint32_t               MatchBatchNo;                  /*�ɽ����κ�*/
	uint8_t                OrderSrc;                      /*������Դ*/
};

