import itchat

@itchat.msg_register('Text', isGroupChat=False)
def text_reply(msg):
    content = msg["Content"]
    from_user = msg["FromUserName"]
    print(msg)

#itchat.auto_login(hotReload=True)
itchat.auto_login(enableCmdQR=True)
itchat.send('Hello, just a test', toUserName='filehelper')


#itchat.run()
