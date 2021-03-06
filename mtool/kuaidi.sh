# 快递查询API申请 2013-11-15
# http://api.ickd.cn/users/
# usr: chuai1609@163.com
# passwd : 666666
# 本文件编码: GB2312,GBK

# if UTF8, assign =1
disp_code=0
# config file name
ini_file="./expressage.ini"
# seperator
sp="----------"

if [ ! -f $ini_file ]; then
	echo "Error: need [expressage.ini] file at local directory!"
	exit 0
fi

bool_utf=`sed -n 1p $ini_file | cut -d '=' -f 2`
if [ $bool_utf = 1 ]; then
	disp_code=utf8
else
	disp_code=gbk
fi

let i=2
while true
do
	#echo $i
	line=`sed -n "$i p" $ini_file`
	if [[ $line = $sp ]]; then
echo "--------------------------------------------------"
		let annot=$i+1
		let exprs=$i+2
		let wbill=$i+3
		line=`sed -n "$annot p" $ini_file | cut -d '=' -f 1`
		if [[ $line = "annotation" ]]; then
			annotation=`sed -n "$annot p" $ini_file | cut -d '=' -f 2`
echo [ $annotation ]
			line=`sed -n "$exprs p" $ini_file | cut -d '=' -f 1`
			if [[ $line = "exprs_corp" ]]; then
				exprs_corp=`sed -n "$exprs p" $ini_file | cut -d '=' -f 2`
echo [ $exprs_corp ]
				line=`sed -n "$wbill p" $ini_file | cut -d '=' -f 1`
				if [[ $line = "way_bill" ]]; then
					way_bill=`sed -n "$wbill p" $ini_file | cut -d '=' -f 2`
echo [ $way_bill ]
curl -s 'http://api.ickd.cn/?id=102634&secret=14de636ea94ad80b3ec93ee821049fac&com='$exprs_corp'&nu='$way_bill'&type=text&encode='$disp_code'&ord=asc'
				else
					exit 0
				fi
			else
				exit 0
			fi
		else
			exit 0
		fi
	else
		exit 0
	fi
	let i=$i+4
done

#old_method
<<"XXX"
excompany=shentong
waybill=668819650676
echo "--------------------------------------------------"
#echo "ex_company=[$excompany] waybill=[$waybill]"
echo "[理发器, 88y]                [申通]"
if [ $iu8code -eq 1 ]; then
curl -s 'http://api.ickd.cn/?id=102634&secret=14de636ea94ad80b3ec93ee821049fac&com='$excompany'&nu='$waybill'&type=text&encode=utf8&ord=asc'
else
curl -s 'http://api.ickd.cn/?id=102634&secret=14de636ea94ad80b3ec93ee821049fac&com='$excompany'&nu='$waybill'&type=text&ord=asc'
fi
echo " "
XXX


# http://api.ickd.cn/?id=102634 &
#                 secret=14de636ea94ad80b3ec93ee821049fac &
#                    com=快递公司英文代码 &
#                     nu=快递单号 &
#                   type=html|text|json &
#                 encode=gbk|utf8 &
#                    ord=asc|desc &
#                   lang=en


# 快递公司列表
#序号 	英文代码 	快递公司中文名
#1	    aae	AAE快递
#2	    anjie	安捷快递
#3	    anxinda	安信达快递
#4	    aramex	Aramex国际快递
#5	    balunzhi	巴伦支
#6	    baotongda	宝通达
#7	    benteng	成都奔腾国际快递
#8	    cces	CCES快递
#9	    changtong	长通物流
#10	    chengguang	程光快递
#11	    chengji	城际快递
#12	    chengshi100	城市100
#13	    chuanxi	传喜快递
#14	    chuanzhi	传志快递
#15	    chukouyi	出口易物流
#16	    citylink	CityLinkExpress
#17	    coe	东方快递
#18	    cszx	城市之星
#19	    datian	大田物流
#20	    dayang	大洋物流快递
#21	    debang	德邦物流
#22	    dechuang	德创物流
#23	    dhl	DHL快递
#24	    diantong	店通快递
#25	    dida	递达快递
#26	    dingdong	叮咚快递
#27	    disifang	递四方速递
#28	    dpex	DPEX快递
#29	    dsu	D速快递
#30	    ees	百福东方物流
#31	    ems	EMS快递
#32	    fanyu	凡宇快递
#33	    fardar	Fardar
#34	    fedex	国际Fedex
#35	    fedexcn	Fedex国内
#36	    feibang	飞邦物流
#37	    feibao	飞豹快递
#38	    feihang	原飞航物流
#39	    feihu	飞狐快递
#40	    feite	飞特物流
#41	    feiyuan	飞远物流
#42	    fengda	丰达快递
#43	    fkd	飞康达快递
#44	    gdyz	广东邮政物流
#45	    gnxb	邮政国内小包
#46	    gongsuda	共速达物流|快递
#47	    guotong	国通快递
#48	    haihong	山东海红快递
#49	    haimeng	海盟速递
#50	    haosheng	昊盛物流
#51	    hebeijianhua	河北建华快递
#52	    henglu	恒路物流
#53	    huacheng	华诚物流
#54	    huahan	华翰物流
#55	    huaqi	华企快递
#56	    huaxialong	华夏龙物流
#57	    huayu	天地华宇物流
#58	    huiqiang	汇强快递
#59	    huitong	汇通快递
#60	    hwhq	海外环球快递
#61	    jiaji	佳吉快运
#62	    jiayi	佳怡物流
#63	    jiayunmei	加运美快递
#64	    jinda	金大物流
#65	    jingdong	京东快递
#66	    jingguang	京广快递
#67	    jinyue	晋越快递
#68	    jixianda	急先达物流
#69	    jldt	嘉里大通物流
#70	    kangli	康力物流
#71	    kcs	顺鑫(KCS)快递
#72	    kuaijie	快捷快递
#73	    kuanrong	宽容物流
#74	    kuayue	跨越快递
#75	    lejiedi	乐捷递快递
#76	    lianhaotong	联昊通快递
#77	    lijisong	成都立即送快递
#78	    longbang	龙邦快递
#79	    minbang	民邦快递
#80	    mingliang	明亮物流
#81	    minsheng	闽盛快递
#82	    nell	尼尔快递
#83	    nengda	港中能达快递
#84	    ocs	OCS快递
#85	    pinganda	平安达
#86	    pingyou	中国邮政平邮
#87	    pinsu	品速心达快递
#88	    quanchen	全晨快递
#89	    quanfeng	全峰快递
#90	    quanjitong	全际通快递
#91	    quanritong	全日通快递
#92	    quanyi	全一快递
#93	    rpx	RPX保时达
#94	    rufeng	如风达快递
#95	    saiaodi	赛澳递
#96	    santai	三态速递
#97	    scs	伟邦(SCS)快递
#98	    shengan	圣安物流
#99	    shengfeng	盛丰物流
#100	shenghui	盛辉物流
#101	shentong	申通快递（可能存在延迟）
#102	shunfeng	顺丰快递
#103	suchengzhaipei	速呈宅配
#104	suijia	穗佳物流
#105	sure	速尔快递
#106	tiantian	天天快递
#107	tnt	TNT快递
#108	tongcheng	通成物流
#109	tonghe	通和天下物流
#110	ups	UPS快递
#111	usps	USPS快递
#112	wanbo	万博快递
#113	wanjia	万家物流
#114	weitepai	微特派
#115	xianglong	祥龙运通快递
#116	xinbang	新邦物流
#117	xinfeng	信丰快递
#118	xingchengzhaipei	星程宅配快递
#119	xiyoute	希优特快递
#120	yad	源安达快递
#121	yafeng	亚风快递
#122	yibang	一邦快递
#123	yinjie	银捷快递
#124	yinsu	音素快运
#125	yishunhang	亿顺航快递
#126	yousu	优速快递
#127	ytfh	北京一统飞鸿快递
#128	yuancheng	远成物流
#129	yuantong	圆通快递
#130	yuanzhi	元智捷诚
#131	yuefeng	越丰快递
#132	yumeijie	誉美捷快递
#133	yunda	韵达快递
#134	yuntong	运通中港快递
#135	yuxin	宇鑫物流
#136	ywfex	源伟丰
#137	zhaijisong	宅急送快递
#138	zhengzhoujianhua	郑州建华快递
#139	zhima	芝麻开门快递
#140	zhongtian	济南中天万运
#141	zhongtie	中铁快运
#142	zhongtong	中通快递
#143	zhongxinda	忠信达快递
#144	zhongyou	中邮物流
#
#*部分带有验证码的快递（物流）公司还不被支持。

