import itchat
import time
import json
import requests
import logging
#import urllib.request
#import urllib.parse

logging.basicConfig(level=logging.INFO,
                   # filename='log.log',
                    format=
                    '%(asctime)s-%(pathname)s[line:%(lineno)d]-%(levelname)s: %(message)s'
                    )

#logging.debug('debug')
#logging.warning('warning')

def auto_chat(input_data, user_id):
    api_url = "http://openapi.tuling123.com/openapi/api/v2"
    post_data = {
        "reqType":0,
        "perception": {
            "inputText": {
                "text": input_data
            },
            "selfInfo": {
                "location": {
                    "city": "Dalian",
                    "province": "LiaoNing",
                }
            }
        },
        "userInfo": {
            "apiKey": "7156ec1180234b8783980166cf348e13",
            "userId": user_id
        }
    }
    post_data = json.dumps(post_data)
    r = requests.post(api_url, data=post_data).json()
    #print(r)
    rsp = r['results'][0]['values']['text']
    logging.info("--> tuling robot reply : " + rsp)
    return rsp


@itchat.msg_register('Text', isGroupChat=False)
def text_reply(msg):
    #logging.info(msg)
    from_user = msg["FromUserName"]
    #nick_name = msg["NickName"]   xxxxxxx
    content = msg["Content"]
    logging.info(from_user + ": " +content)
    #print(msg.user["NickName"])
    #if (msg.user["NickName"] == "人想瘦别吃肉"):
    if (msg.user["NickName"] == "may"):
        message = auto_chat(content, 123456)
        msg.user.send(message)
    #message = auto_chat(content, from_user)
    #itchat.send(message, from_user)

#
itchat.auto_login(hotReload=True, enableCmdQR=2)
itchat.run()

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
    # 小明  (不好用)   @08626a7d0b81b942a8c94dab62bd2760813327f3073b72d4a6ecdba7d528398b
    itchat.send('你在嘎哈?  ' + str(i), toUserName='@0783779934b26f28d97c899ae810575534d046026d1e5f6dd82f6b614c24b4a8')
    # may  (test OK)
    #itchat.send('你在嘎哈?  ' + str(i), toUserName='@f69787c0b46316cc09cd2ea4837f5c10c05af21d4edcc6523424fa16a16fae2f')
    # 李文平 (test OK)
    #itchat.send('你在嘎哈?  ' + str(i), toUserName='@f49ca4ca138576edc7402e0df60754e8')

    time.sleep(10)
    i=i+1

"""

