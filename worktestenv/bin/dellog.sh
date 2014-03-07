#!/bin/sh

#使用方法：1./dellog.sh  删除bin，log，store下的日志，但不删除交易初始化文件
#          2./dellog.sh date  删除bin，log，store下的日志，同时删除交易初始化文件
#______________________________________________________
#set -s 0

cd ..
TRADE_HOME_A=`pwd`
echo "TRADE_HOME_A="$TRADE_HOME_A

DIRS="f_arb/match_arb \
f_arb/front_arb \
f_arb/quot_arb \
f_arb/tran_arb \
f_switch/match_bus \
f_switch/front_bus \
f_switch/quot_bus \
f_switch/tran_bus \
f_initterm \
t_match \
t_ctrpro \
t_fund \
t_check \
t_query \
t_marketdata \
t_tradefront \
t_quotfront \
t_synwrite \
t_tradegate \
 "

for X in $DIRS
do
     cd $TRADE_HOME_A/$X
     rm  ./bin/*.con ./bin/*.id ./bin/*.date ./bin/*.log  ./bin/s*.dat ./bin/*.txt >/dev/null 2>&1
     rm ./log/* >/dev/null 2>&1
     #rm ./store/*.con ./store/*.id ./store/*.date ./store/*.txt ./store/*.log ./store/s*.dat ./store/bus*.dat ./store/CL*.dat ./store/dev*.dat    >/dev/null 2>&1
     #避免删除初始化文件
     rm `ls ./store/*  2>/dev/null | grep -v trade `>/dev/null 2>&1
     if  [  "$1"  =  "data" ]   ;then
        rm ./store/trade*.dat  >/dev/null 2>&1
        echo  "删除日志和初始化数据 ----> $X" 
     else
        echo  "删除日志 ----> $X" 
     fi
 done


