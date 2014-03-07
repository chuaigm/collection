
cd ../arbitration/1_arb/bin
nohup ./fr_arb > ./fr_arb_run.log &
cd - > /dev/null

cd ../arbitration/2_arb/bin
nohup ./ma_arb > ./ma_arb_run.log &
cd - > /dev/null

cd ../arbitration/9_arb/bin
nohup ./R_arb > ./R_arb_run.log &
cd - > /dev/null
