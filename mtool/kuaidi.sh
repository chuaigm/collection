# ��ݲ�ѯAPI���� 2013-11-15
# http://api.ickd.cn/users/
# usr: chuai1609@163.com
# passwd : 666666
# ���ļ�����: GB2312,GBK

# if UTF8, assign =1
iu8code=0
<<"XXX"
XXX
# change your mail company and waybill here!!!
excompany=shentong
waybill=668819650676
echo "--------------------------------------------------"
#echo "ex_company=[$excompany] waybill=[$waybill]"
echo "[����, 88y]                [��ͨ]"
if [ $iu8code -eq 1 ]; then
curl -s 'http://api.ickd.cn/?id=102634&secret=14de636ea94ad80b3ec93ee821049fac&com='$excompany'&nu='$waybill'&type=text&encode=utf8&ord=asc'
else
curl -s 'http://api.ickd.cn/?id=102634&secret=14de636ea94ad80b3ec93ee821049fac&com='$excompany'&nu='$waybill'&type=text&ord=asc'
fi
echo " "
echo "--------------------------------------------------"
excompany=yunda
waybill=1201152029178
echo "[�����˻�, 14.8y]              [�ϴ�]"
curl -s 'http://api.ickd.cn/?id=102634&secret=14de636ea94ad80b3ec93ee821049fac&com='$excompany'&nu='$waybill'&type=text&ord=asc'
echo " "
echo "--------------------------------------------------"
<<"XXX"
excompany=yunda
waybill=1201130565354
echo "[��Բ��500g��18y]           [�ϴ�]"
curl -s 'http://api.ickd.cn/?id=102634&secret=14de636ea94ad80b3ec93ee821049fac&com='$excompany'&nu='$waybill'&type=text&ord=asc'
echo " "
echo "--------------------------------------------------"
excompany=zhongtong
waybill=761239806637
echo "[ñ�ӣ�29y]                 [��ͨ]"
curl -s 'http://api.ickd.cn/?id=102634&secret=14de636ea94ad80b3ec93ee821049fac&com='$excompany'&nu='$waybill'&type=text&ord=asc'
echo " "
echo "--------------------------------------------------"
XXX


# http://api.ickd.cn/?id=102634 &
#                 secret=14de636ea94ad80b3ec93ee821049fac &
#                    com=��ݹ�˾Ӣ�Ĵ��� &
#                     nu=��ݵ��� &
#                   type=html|text|json &
#                 encode=gbk|utf8 &
#                    ord=asc|desc &
#                   lang=en


# ��ݹ�˾�б�
#��� 	Ӣ�Ĵ��� 	��ݹ�˾������
#1	    aae	AAE���
#2	    anjie	���ݿ��
#3	    anxinda	���Ŵ���
#4	    aramex	Aramex���ʿ��
#5	    balunzhi	����֧
#6	    baotongda	��ͨ��
#7	    benteng	�ɶ����ڹ��ʿ��
#8	    cces	CCES���
#9	    changtong	��ͨ����
#10	    chengguang	�̹���
#11	    chengji	�Ǽʿ��
#12	    chengshi100	����100
#13	    chuanxi	��ϲ���
#14	    chuanzhi	��־���
#15	    chukouyi	����������
#16	    citylink	CityLinkExpress
#17	    coe	�������
#18	    cszx	����֮��
#19	    datian	��������
#20	    dayang	�����������
#21	    debang	�°�����
#22	    dechuang	�´�����
#23	    dhl	DHL���
#24	    diantong	��ͨ���
#25	    dida	�ݴ���
#26	    dingdong	���˿��
#27	    disifang	���ķ��ٵ�
#28	    dpex	DPEX���
#29	    dsu	D�ٿ��
#30	    ees	�ٸ���������
#31	    ems	EMS���
#32	    fanyu	������
#33	    fardar	Fardar
#34	    fedex	����Fedex
#35	    fedexcn	Fedex����
#36	    feibang	�ɰ�����
#37	    feibao	�ɱ����
#38	    feihang	ԭ�ɺ�����
#39	    feihu	�ɺ����
#40	    feite	��������
#41	    feiyuan	��Զ����
#42	    fengda	�����
#43	    fkd	�ɿ�����
#44	    gdyz	�㶫��������
#45	    gnxb	��������С��
#46	    gongsuda	���ٴ�����|���
#47	    guotong	��ͨ���
#48	    haihong	ɽ��������
#49	    haimeng	�����ٵ�
#50	    haosheng	�ʢ����
#51	    hebeijianhua	�ӱ��������
#52	    henglu	��·����
#53	    huacheng	��������
#54	    huahan	��������
#55	    huaqi	������
#56	    huaxialong	����������
#57	    huayu	��ػ�������
#58	    huiqiang	��ǿ���
#59	    huitong	��ͨ���
#60	    hwhq	���⻷����
#61	    jiaji	�Ѽ�����
#62	    jiayi	��������
#63	    jiayunmei	���������
#64	    jinda	�������
#65	    jingdong	�������
#66	    jingguang	������
#67	    jinyue	��Խ���
#68	    jixianda	���ȴ�����
#69	    jldt	�����ͨ����
#70	    kangli	��������
#71	    kcs	˳��(KCS)���
#72	    kuaijie	��ݿ��
#73	    kuanrong	��������
#74	    kuayue	��Խ���
#75	    lejiedi	�ֽݵݿ��
#76	    lianhaotong	���ͨ���
#77	    lijisong	�ɶ������Ϳ��
#78	    longbang	������
#79	    minbang	�����
#80	    mingliang	��������
#81	    minsheng	��ʢ���
#82	    nell	������
#83	    nengda	�����ܴ���
#84	    ocs	OCS���
#85	    pinganda	ƽ����
#86	    pingyou	�й�����ƽ��
#87	    pinsu	Ʒ���Ĵ���
#88	    quanchen	ȫ�����
#89	    quanfeng	ȫ����
#90	    quanjitong	ȫ��ͨ���
#91	    quanritong	ȫ��ͨ���
#92	    quanyi	ȫһ���
#93	    rpx	RPX��ʱ��
#94	    rufeng	������
#95	    saiaodi	���ĵ�
#96	    santai	��̬�ٵ�
#97	    scs	ΰ��(SCS)���
#98	    shengan	ʥ������
#99	    shengfeng	ʢ������
#100	shenghui	ʢ������
#101	shentong	��ͨ��ݣ����ܴ����ӳ٣�
#102	shunfeng	˳����
#103	suchengzhaipei	�ٳ�լ��
#104	suijia	�������
#105	sure	�ٶ����
#106	tiantian	������
#107	tnt	TNT���
#108	tongcheng	ͨ������
#109	tonghe	ͨ����������
#110	ups	UPS���
#111	usps	USPS���
#112	wanbo	�򲩿��
#113	wanjia	�������
#114	weitepai	΢����
#115	xianglong	������ͨ���
#116	xinbang	�°�����
#117	xinfeng	�ŷ���
#118	xingchengzhaipei	�ǳ�լ����
#119	xiyoute	ϣ���ؿ��
#120	yad	Դ������
#121	yafeng	�Ƿ���
#122	yibang	һ����
#123	yinjie	���ݿ��
#124	yinsu	���ؿ���
#125	yishunhang	��˳�����
#126	yousu	���ٿ��
#127	ytfh	����һͳ�ɺ���
#128	yuancheng	Զ������
#129	yuantong	Բͨ���
#130	yuanzhi	Ԫ�ǽݳ�
#131	yuefeng	Խ����
#132	yumeijie	�����ݿ��
#133	yunda	�ϴ���
#134	yuntong	��ͨ�иۿ��
#135	yuxin	��������
#136	ywfex	Դΰ��
#137	zhaijisong	լ���Ϳ��
#138	zhengzhoujianhua	֣�ݽ������
#139	zhima	֥�鿪�ſ��
#140	zhongtian	������������
#141	zhongtie	��������
#142	zhongtong	��ͨ���
#143	zhongxinda	���Ŵ���
#144	zhongyou	��������
#
#*���ִ�����֤��Ŀ�ݣ���������˾������֧�֡�

