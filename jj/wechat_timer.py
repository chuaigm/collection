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

itchat.auto_login(hotReload=True, enableCmdQR=True)
i=1
while True:
    itchat.send('Hello, just a test' + i, toUserName='filehelper')
    time.sleep(5)



#itchat.run()
