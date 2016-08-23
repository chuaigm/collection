# ubuntu 清理
# From: http://www.jb51.net/os/Ubuntu/311761.html

# 1.删除所有软件安装包
# 本质上是删除了,  /var/cache/apt/archives 目录下所有以 .deb 结尾的文件

echo "----sudo apt-get clean----"
sudo apt-get clean
echo "----[done]----"

# 2. 删除不再可用到软件包

echo "----sudo apt-get autoclean----"
sudo apt-get autoclean
echo "----[done]----"

# 3. 删除不再序言到依赖软件包

echo "----sudo apt-get autoremove----"
sudo apt-get autoremove
echo "----[done]----"


# 删除指定软件
# sudo apt-get remove [软件名]
# sudo apt-get purge  [软件名]
# 前者删除软件
# 后者不仅删除软件，还删除相关配置，删除更彻底
#
#
#
#
#
