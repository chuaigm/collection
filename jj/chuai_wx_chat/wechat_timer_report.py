import itchat
import time
import logging
import os

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

unsent_file = "../message_log/unsent.log"
i=1
while True:
    time_str = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()) 
    output = "test: [" + str(i) + "] " + time_str
    logging.info(output)

    #with open(unsent_file,'r') as f:
    if(os.path.exists(unsent_file)):
        f = open(unsent_file,'r')
        unsent_ctn = f.read()
        logging.info(unsent_ctn)
        # 文件助手
        itchat.send(unsent_ctn, toUserName='filehelper')
        os.remove(unsent_file)

    time.sleep(3600)
    i=i+1


#itchat.run()
