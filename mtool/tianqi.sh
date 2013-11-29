<<"XXX"
red='\e[0;31m'
RED='\e[1;31m'
green='\e[0;32m'
GREEN='\e[1;32m'
blue='\e[0;34m'
BLUE='\e[1;34m'
cyan='\e[0;36m'
CYAN='\e[1;36m'
NC='\e[0m' 
echo -e "${green} Please input a number:${NC}(time format ${RED} s|m|h|d${NC})"
XXX

curl -s 'http://m.weather.com.cn/data/101070201.html' > wtmp

city=`sed 's/.*city":"\(.*\)","city_en.*/\1/g' wtmp`
echo $city
