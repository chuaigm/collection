from bs4 import BeautifulSoup
import time
import requests
import sys
import json

post_url = "https://passport.jd.com/uc/loginService"
login_url = "https://passport.jd.com/uc/login"
auth_url = "https://passport.jd.com/uc/showAuthCode"
vip_url = "https://vip.jd.com/sign/index"
bean_url = "https://bean.jd.com/myJingBean/list"

headers = {
            'User-Agent': 'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36'
            ' (KHTML, like Gecko) Chrome/62.0.3202.89 Safari/537.36',
            'Referer': 'https://www.jd.com/'
        }

s = requests.Session()

def get_login_info(username, password, login_url):
    response = s.get(url=login_url, headers=headers)
    soup = BeautifulSoup(response.text, 'lxml')

    # get target info
    sa_token = soup.find(id='sa_token')['value']
    uuid = soup.find(id='uuid')['value']
    loginType = soup.find(id='loginType')['value']
    pubKey = soup.find(id='pubKey')['value']
    _t = soup.find(id='token')['value']
    fp = soup.find(id='sessionId')['value']
    eid = soup.find(id='eid')['value']

    # judge virefy
    auth_response = s.post(auth_url, data={'loginName': username, 'nloginpwd': password}).text
    if 'true' in auth_response:
        # get pic
        auth_code_url = soup.find(id='JD_Verification1').get('src2')
        auth_code = str(get_auth_img(auth_code_url))
    else:
        auth_code = ''

    data = {
        'uuid': uuid,
        'eid': eid,
        'fp': fp,
        '_t': _t,
        'loginType': loginType,
        'loginname': username,
        'nloginpwd': password,
        'chkRememberMe': True,
        'authcode': auth_code,
        'pubKey': pubKey,
        'sa_token': sa_token
        # 'authCode': auth_code
    }


    print('1. get login info success')
    return data

def get_auth_img(url):
    auth_code_url = 'http:{}&yys={}'.format(url, str(int(time.time()*1000)))
    auth_img = s.get(auth_code_url, headers=headers)
    with open('authcode.jpg', 'wb') as f:
        f.write(auth_img.content)
    code_typein = input('please according to authcode.jpg input:')
    return code_typein

def login(data):
    headers['Referer'] = 'https://passport.jd.com/uc/login?ltype=logout'
    response = s.post(post_url, data=data, headers=headers)
    # if 
    if "success" in response.text:
        print("2. login success")
    else:
        print("2. login fail")
        print((response.text).encode('utf-8'))
        sys.exit(1)

def get_shops():
    headers['Referer'] = 'https://home.jd.com/'
    response = s.get(bean_url, headers=headers)
    soup = BeautifulSoup(response.text, 'lxml')
    bean_shop_list = soup.find_all('a', class_='s-btn')
    return bean_shop_list

def sign_shop(shop_url):
    try:
        headers['Referer'] = str(shop_url)
        response = s.get(shop_url, headers=headers)
        soup = BeautifulSoup(response.text, 'lxml')
        # 
        shop_id = soup.find(id='shop_id')['value']
        # 
        sign_url = 'https://mall.jd.com/shopSign-{}.html'.format(shop_id)
        s.get(sign_url, headers=headers)
        print('sign success: {}'.format(shop_url))
    except Exception as error:
        print('sign fail: {}'.format(shop_url))
        print(error)


def main():
    #username = input('user_name')
    #password = input('pwd')
    # 
    username = 'chuai1609'
    password = '5625106jingdong'
    # 
    data = get_login_info(username, password, login_url)
    # 
    login(data)
    # 
    bean_shop_list = get_shops()
    if len(bean_shop_list) == 0:
        print('all sign')
        sys.exit(0)
    for shop in bean_shop_list:
        sign_shop(shop['href'])
    

if __name__ == "__main__":
    main()
