
ps -ef | grep -w arb | grep -v 'grep' | awk '{printf("kill -9 %s\n",$2)}' | sh
