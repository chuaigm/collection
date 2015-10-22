
#cat these, and execute in shell
while true;do echo -ne "\e[s\e[0;$((COLUMNS-27))H$(date)\e[u";sleep 1;done &

