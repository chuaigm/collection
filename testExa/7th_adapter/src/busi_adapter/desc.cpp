#include "StdAfx.h"

/**************** Ӧ��ҵ��Field Id ********************/

/* ��Ӧ�� */
REGISTER_FIELD(0X00000001,CFldRspMsg);

/* �ͻ���Ϣ�� */
REGISTER_FIELD(0X00000002,CFldClient);

/* ������Ϣ�� */
REGISTER_FIELD(0X00000003,CFldOrder);

TFTCPPackageDesc g_FTCPPackageDesc[] = 
{
     {TID_REQTRADERSESSION,{&CFldTraderSessionReq::m_Describe},1,"����Ա�Ự����"},
	 {TID_RSPTRADERSESSION,{&CFldRspMsg::m_Describe,&CFldTraderSessionRsp::m_Describe,&CFldCompressMethodRsp::m_Describe},3,"����Ա�ỰӦ��"},

     {TID_REQTRADERCERT,{&CFldTraderCertReq::m_Describe},1,"����Ա��֤����"},
     {TID_RSPTRADERCERT,{&CFldRspMsg::m_Describe,&CFldTraderCertRsp::m_Describe},2,"����Ա��֤Ӧ��"},
	 {TID_REQQRYARBICONTRACT,{&CFldContractQryReq::m_Describe},1,"��ѯ������Լ"},
     {TID_RSPQRYARBICONTRACT,{&CFldRspMsg::m_Describe,&CFldArbiContract::m_Describe,&CFldArbiLeg::m_Describe},3,"����������ԼӦ��"},
     {TID_REQQRYARBIRULE,{&CFldArbiRuleReq::m_Describe},1,"��ѯ�������Թ�������"},
     {TID_RSPQRYARBIRULE,{&CFldRspMsg::m_Describe,&CFldArbiRule::m_Describe},2,"��ѯ�������Թ���Ӧ��"},
     {TID_REQTRADERQRYCLIENT,{&CFldClientQryReq::m_Describe},1,"��ѯ�ͻ�����"},
     {TID_RSPTRADERQRYCLIENT,{&CFldRspMsg::m_Describe,&CFldClient::m_Describe},2,"��ѯ�ͻ�Ӧ��"},
     {TID_REQREADY,{&CFldDissemination::m_Describe},1,"��������"},
     {TID_REQTRADERINSERTORDERS,{&CFldOrder::m_Describe},1,"����Ա��������"},
     {TID_RSPTRADERINSERTORDERS,{&CFldRspMsg::m_Describe,&CFldOrder::m_Describe},2,"����Ա����Ӧ��"},

     {TID_TRADERORDERSCONFIRMATION,{&CFldOrderStatus::m_Describe},1,"����Ա����״̬ȷ��"},
     {TID_NTYTRADERMATCH,{&CFldMatch::m_Describe},1,"����Ա�ɽ�֪ͨ"},
     {TID_REQTRADERCANCELORDER,{&CFldOrderAction::m_Describe},1,"����Ա��������"},
     {TID_RSPTRADERCANCELORDER,{&CFldRspMsg::m_Describe,&CFldOrderAction::m_Describe},2,"����Ա��������Ӧ��"},
     {TID_RPLTRADERCANCELORDER,{&CFldRspMsg::m_Describe,&CFldOrderAction::m_Describe},2,"����Ա��������ǰ��ͨѶӦ��"},

     {TID_REQTRADEROPTEXEC,{&CFldOrder::m_Describe},1,"��Ȩִ������"},
     {TID_RSPTRADEROPTEXEC,{&CFldRspMsg::m_Describe,&CFldOrder::m_Describe},2,"��Ȩִ������Ӧ��"},
     {TID_REQTRADERCANCELOPTEXEC,{&CFldOrder::m_Describe},1,"������Ȩִ������"},
     {TID_RSPTRADERCANCELOPTEXEC,{&CFldRspMsg::m_Describe,&CFldOrder::m_Describe},2,"������Ȩִ������Ӧ��"},
     {TID_REQTRADERQRYOPTEXEC,{&CFldOrderQryReq::m_Describe},1,"��ѯ��Ȩִ������"},
     {TID_RSPTRADERQRYOPTEXEC,{&CFldRspMsg::m_Describe,&CFldOrderStatus::m_Describe},2,"��ѯ��Ȩִ������Ӧ��"},
     {TID_REQTRADERQRYORDER, { &CFldOrderQryReq::m_Describe, &CFldBigQueryAuxReq::m_Describe}, 2, "��ѯί������" },
     {TID_RSPTRADERQRYORDER,{&CFldRspMsg::m_Describe,&CFldOrderStatus::m_Describe},2,"��ѯί��Ӧ��"},
     {TID_REQTRADERQRYMATCH, { &CFldMatchQryReq::m_Describe, &CFldBigQueryAuxReq::m_Describe}, 2, "��ѯ�ɽ�����" },
     {TID_RSPTRADERQRYMATCH,{&CFldRspMsg::m_Describe,&CFldMatch::m_Describe},2,"��ѯ�ɽ�Ӧ��"},
     {TID_REQTRADERQRYCLIENTPOSI,{&CFldClientPosiQryReq::m_Describe},1,"��ѯ�ͻ��ֲ�����"},
     {TID_RSPTRADERQRYCLIENTPOSI,{&CFldRspMsg::m_Describe,&CFldPosi::m_Describe},2,"��ѯ�ͻ��ֲ�Ӧ��"},
     {TID_REQTRADERQRYMEMBERPOSI,{&CFldMemberPosiQryReq::m_Describe},1,"��ѯ��Ա�ֲ�����"},
     {TID_RSPTRADERQRYMEMBERPOSI,{&CFldRspMsg::m_Describe,&CFldPosi::m_Describe},2,"��ѯ��Ա�ֲ�Ӧ��"},
     {TID_REQTRADERQRYMEMBERCAP,{&CFldMemberCapQryReq::m_Describe},1,"��ѯ��Ա�ʽ�����"},
     {TID_RSPTRADERQRYMEMBERCAP,{&CFldRspMsg::m_Describe,&CFldMemberCap::m_Describe},2,"��ѯ��Ա�ʽ�Ӧ��"},
     {TID_REQTRADERQRYPARAM,{&CFldTradeParamQryReq::m_Describe},1,"���ײ�����ѯ����"},
     {TID_RSPTRADERQRYPARAM,{&CFldRspMsg::m_Describe,&CFldTradeParam::m_Describe},2,"���ײ�����ѯӦ��"},
     {TID_BESTQUOT,{&CFldBestQuot::m_Describe},1,"ֱ���г���������֪ͨ"},
};

//����һ����̬��ע��FTCP������������
static CRegFTCPDesc  InRegFTCPDesc(sizeof(g_FTCPPackageDesc)/sizeof(TFTCPPackageDesc),&g_FTCPPackageDesc[0]);

void PrintFTCPDescCount()
{
	printf("Total FTCP Desc Count = %d\n",InRegFTCPDesc.m_count);
}

//�ܹ�162��field
