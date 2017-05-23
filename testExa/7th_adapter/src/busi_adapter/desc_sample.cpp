#include "StdAfx.h"

/**************** 应用业务Field Id ********************/

/* 响应域 */
REGISTER_FIELD(0X00000001,CFldRspMsg);

/* 客户信息域 */
REGISTER_FIELD(0X00000002,CFldClient);

/* 定单信息域 */
REGISTER_FIELD(0X00000003,CFldOrder);

TFTCPPackageDesc g_FTCPPackageDesc[] = 
{
     {TID_REQTRADERSESSION,{&CFldTraderSessionReq::m_Describe},1,"交易员会话请求"},
	 {TID_RSPTRADERSESSION,{&CFldRspMsg::m_Describe,&CFldTraderSessionRsp::m_Describe,&CFldCompressMethodRsp::m_Describe},3,"交易员会话应答"},

     {TID_REQTRADERCERT,{&CFldTraderCertReq::m_Describe},1,"交易员认证请求"},
     {TID_RSPTRADERCERT,{&CFldRspMsg::m_Describe,&CFldTraderCertRsp::m_Describe},2,"交易员认证应答"},
	 {TID_REQQRYARBICONTRACT,{&CFldContractQryReq::m_Describe},1,"查询套利合约"},
     {TID_RSPQRYARBICONTRACT,{&CFldRspMsg::m_Describe,&CFldArbiContract::m_Describe,&CFldArbiLeg::m_Describe},3,"查期套利合约应答"},
     {TID_REQQRYARBIRULE,{&CFldArbiRuleReq::m_Describe},1,"查询套利策略规则请求"},
     {TID_RSPQRYARBIRULE,{&CFldRspMsg::m_Describe,&CFldArbiRule::m_Describe},2,"查询套利策略规则应答"},
     {TID_REQTRADERQRYCLIENT,{&CFldClientQryReq::m_Describe},1,"查询客户请求"},
     {TID_RSPTRADERQRYCLIENT,{&CFldRspMsg::m_Describe,&CFldClient::m_Describe},2,"查询客户应答"},
     {TID_REQREADY,{&CFldDissemination::m_Describe},1,"就绪请求"},
     {TID_REQTRADERINSERTORDERS,{&CFldOrder::m_Describe},1,"交易员定单请求"},
     {TID_RSPTRADERINSERTORDERS,{&CFldRspMsg::m_Describe,&CFldOrder::m_Describe},2,"交易员定单应答"},

     {TID_TRADERORDERSCONFIRMATION,{&CFldOrderStatus::m_Describe},1,"交易员定单状态确认"},
     {TID_NTYTRADERMATCH,{&CFldMatch::m_Describe},1,"交易员成交通知"},
     {TID_REQTRADERCANCELORDER,{&CFldOrderAction::m_Describe},1,"交易员撤销定单"},
     {TID_RSPTRADERCANCELORDER,{&CFldRspMsg::m_Describe,&CFldOrderAction::m_Describe},2,"交易员撤销定单应答"},
     {TID_RPLTRADERCANCELORDER,{&CFldRspMsg::m_Describe,&CFldOrderAction::m_Describe},2,"交易员撤销定单前置通讯应答"},

     {TID_REQTRADEROPTEXEC,{&CFldOrder::m_Describe},1,"期权执行申请"},
     {TID_RSPTRADEROPTEXEC,{&CFldRspMsg::m_Describe,&CFldOrder::m_Describe},2,"期权执行申请应答"},
     {TID_REQTRADERCANCELOPTEXEC,{&CFldOrder::m_Describe},1,"撤销期权执行申请"},
     {TID_RSPTRADERCANCELOPTEXEC,{&CFldRspMsg::m_Describe,&CFldOrder::m_Describe},2,"撤销期权执行申请应答"},
     {TID_REQTRADERQRYOPTEXEC,{&CFldOrderQryReq::m_Describe},1,"查询期权执行申请"},
     {TID_RSPTRADERQRYOPTEXEC,{&CFldRspMsg::m_Describe,&CFldOrderStatus::m_Describe},2,"查询期权执行申请应答"},
     {TID_REQTRADERQRYORDER, { &CFldOrderQryReq::m_Describe, &CFldBigQueryAuxReq::m_Describe}, 2, "查询委托请求" },
     {TID_RSPTRADERQRYORDER,{&CFldRspMsg::m_Describe,&CFldOrderStatus::m_Describe},2,"查询委托应答"},
     {TID_REQTRADERQRYMATCH, { &CFldMatchQryReq::m_Describe, &CFldBigQueryAuxReq::m_Describe}, 2, "查询成交请求" },
     {TID_RSPTRADERQRYMATCH,{&CFldRspMsg::m_Describe,&CFldMatch::m_Describe},2,"查询成交应答"},
     {TID_REQTRADERQRYCLIENTPOSI,{&CFldClientPosiQryReq::m_Describe},1,"查询客户持仓请求"},
     {TID_RSPTRADERQRYCLIENTPOSI,{&CFldRspMsg::m_Describe,&CFldPosi::m_Describe},2,"查询客户持仓应答"},
     {TID_REQTRADERQRYMEMBERPOSI,{&CFldMemberPosiQryReq::m_Describe},1,"查询会员持仓请求"},
     {TID_RSPTRADERQRYMEMBERPOSI,{&CFldRspMsg::m_Describe,&CFldPosi::m_Describe},2,"查询会员持仓应答"},
     {TID_REQTRADERQRYMEMBERCAP,{&CFldMemberCapQryReq::m_Describe},1,"查询会员资金请求"},
     {TID_RSPTRADERQRYMEMBERCAP,{&CFldRspMsg::m_Describe,&CFldMemberCap::m_Describe},2,"查询会员资金应答"},
     {TID_REQTRADERQRYPARAM,{&CFldTradeParamQryReq::m_Describe},1,"交易参数查询请求"},
     {TID_RSPTRADERQRYPARAM,{&CFldRspMsg::m_Describe,&CFldTradeParam::m_Describe},2,"交易参数查询应答"},
     {TID_BESTQUOT,{&CFldBestQuot::m_Describe},1,"直接市场最优行情通知"},
};

//定义一个静态的注册FTCP包的描述对象
static CRegFTCPDesc  InRegFTCPDesc(sizeof(g_FTCPPackageDesc)/sizeof(TFTCPPackageDesc),&g_FTCPPackageDesc[0]);

void PrintFTCPDescCount()
{
	printf("Total FTCP Desc Count = %d\n",InRegFTCPDesc.m_count);
}

//总共162个field
