from bs4 import BeautifulSoup
from lxml import html
import xml
import time
import requests

# 博时黄金基金的信息页面
url = "https://www.bosera.com/fund/002611.html"
f = requests.get(url)                 #Get该网页从而获取该html内容
soup = BeautifulSoup(f.content, "lxml")  #用lxml解析器解析该网页的内容, 好像f.text也是返回的html

#print(f.content.decode())
#content = soup.find_all('div',class_="fund-essential_return" )   #因为calss和关键字冲突，所以改名class_
#content = soup.find_all('span',class_="num" )   #因为calss和关键字冲突，所以改名class_

# 通过网页调试，找到净值的值在这个地方
content = soup.find('span',class_="num" )   #因为calss和关键字冲突，所以改名class_
net_worth = content.text.strip().split("\n")[0].strip() #由于输入比较奇怪，所以先strip弄掉空白字符，
#print(content.text)
#print(net_worth)

#由于博时的黄金是净值乘 285
gold_price = float(net_worth) * 285
#print(gold_price)

#time_str = time.strftime("%Y-%m-%d", time.localtime())

# 日期我们也取网页上的时间
date_line = soup.find('span',class_="text" )
date_str = date_line.text.split("(")[1].split(")")[0]
#print(date_line.text)
#print(date_str)

print(date_str + "  " + net_worth + "  " + "%.2f" %(gold_price))


#a = content.find_all('span', class_="num")
#print(a)

"""
for k in soup.find_all('div',class_='pl2'):#,找到div并且class为pl2的标签
   a = k.find_all('span')       #在每个对应div标签下找span标签，会发现，一个a里面有四组span
   print(a[0].string)            #取第一组的span中的字符串
"""
