import itchat
import time

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
    # 文件助手
    #itchat.send('你在嘎哈?  ' + str(i), toUserName='filehelper')
    # 小明  (不好用)
    itchat.send('你在嘎哈?  ' + str(i), toUserName='@0783779934b26f28d97c899ae810575534d046026d1e5f6dd82f6b614c24b4a8')
    # may  (test OK)
    #itchat.send('你在嘎哈?  ' + str(i), toUserName='@f69787c0b46316cc09cd2ea4837f5c10c05af21d4edcc6523424fa16a16fae2f')
    # 李文平 (test OK)
    #itchat.send('你在嘎哈?  ' + str(i), toUserName='@f49ca4ca138576edc7402e0df60754e8')

    time.sleep(10)
    i=i+1



#itchat.run()
