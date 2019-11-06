import itchat
import time
import logging

logging.basicConfig(level=logging.INFO,
                    filename='log.log',
                    format=
                    '%(asctime)s-%(pathname)s[line:%(lineno)d]-%(levelname)s: %(message)s'
                    )
"""
@itchat.msg_register('Text', isGroupChat=False)
def text_reply(msg):
    content = msg["Content"]
    from_user = msg["FromUserName"]
    print(msg)
"""

#itchat.auto_login(hotReload=True)

# 注意，这里二维码在不同系统字符宽度不同，需要调整设置
#itchat.auto_login(hotReload=True, enableCmdQR=True)
itchat.auto_login(hotReload=True, enableCmdQR=2)

#account=itchat.get_friends('小明')
#print(account)

i=1
while True:
    time_str = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()) 
    output = "test: [" + str(i) + "] " + time_str
    logging.info(output)
    # 文件助手
    itchat.send(output, toUserName='filehelper')
    # 小明  (不好用)
    #itchat.send('你在嘎哈?  ' + str(i), toUserName='@08626a7d0b81b942a8c94dab62bd2760813327f3073b72d4a6ecdba7d528398b')
    # may  (test OK)
    #itchat.send('你在嘎哈?  ' + str(i), toUserName='@f69787c0b46316cc09cd2ea4837f5c10c05af21d4edcc6523424fa16a16fae2f')
    # 李文平 (test OK)
    #itchat.send('你在嘎哈?  ' + str(i), toUserName='@f49ca4ca138576edc7402e0df60754e8')

    #target = itchat.search_friends(nickName='小明')[0]   不好用
    #target = itchat.search_friends(nickName='人想瘦别吃肉')[0]
    #print(target)
    #target.send('test')

    time.sleep(7200)
    i=i+1



#itchat.run()
