
# Sample:
# 

CPP_FILE="
BusinessQueryHandle.cpp
"

for F in $CPP_FILE
do

sed "
s/ClientID.buf/ClientID/g;
s/BYTES_TO_BYTES(\(.*\),\s*\(.*\)ClientID.*);/\1 = \2ClientID;/g;
s/BYTES_TO_BYTES(\(.*\)ClientID.*,\s*\(.*\));/\1ClientID = \2;/g;
s/0\s*!=\s*strlen(.*ClientID.*&&\s*//g;
s/0\s*!=\s*strcmp(\(.*\),\s*\(.*\)ClientID[^)]*)\(.*\)/\1 != \2ClientID\3/g;
s/0\s*!=\s*strcmp(\(.*\)ClientID.*,\s*\([^)]*\))\(.*\)/\1ClientID != \2\3/g;
s/0\s*==\s*strcmp(\(.*\),\s*\(.*\)ClientID[^)]*)\(.*\)/\1 == \2ClientID\3/g;
s/0\s*==\s*strcmp(\(.*\)ClientID.*,\s*\([^)]*\))\(.*\)/\1ClientID == \2\3/g;
s/atoi(\(.*\)ClientID[^)]*)\(.*\)/\1ClientID\2/g;
s/!(\(.*\)\s*==\s*\(.*\)ClientID)/\1 != \2ClientID/g;
;
s/MemberID.buf/MemberID/g;
s/BYTES_TO_BYTES(\(.*\),\s*\(.*\)MemberID.*);/\1 = \2MemberID;/g;
s/BYTES_TO_BYTES(\(.*\)MemberID.*,\s*\(.*\));/\1MemberID = \2;/g;
s/0\s*!=\s*strlen(.*MemberID.*&&\s*//g;
s/0\s*!=\s*strcmp(\(.*\),\s*\(.*\)MemberID[^)]*)\(.*\)/\1 != \2MemberID\3/g;
s/0\s*!=\s*strcmp(\(.*\)MemberID.*,\s*\([^)]*\))\(.*\)/\1MemberID != \2\3/g;
s/0\s*==\s*strcmp(\(.*\),\s*\(.*\)MemberID[^)]*)\(.*\)/\1 == \2MemberID\3/g;
s/0\s*==\s*strcmp(\(.*\)MemberID.*,\s*\([^)]*\))\(.*\)/\1MemberID == \2\3/g;
s/atoi(\(.*\)MemberID[^)]*)\(.*\)/\1MemberID\2/g;
s/!(\(.*\)\s*==\s*\(.*\)MemberID)/\1 != \2MemberID/g;
;
s/TraderNo.buf/TraderNo/g;
s/BYTES_TO_BYTES(\(.*\),\s*\(.*\)TraderNo.*);/\1 = \2TraderNo;/g;
s/BYTES_TO_BYTES(\(.*\)TraderNo.*,\s*\(.*\));/\1TraderNo = \2;/g;
s/0\s*!=\s*strlen(.*TraderNo.*&&\s*//g;
s/0\s*!=\s*strcmp(\(.*\),\s*\(.*\)TraderNo[^)]*)\(.*\)/\1 != \2TraderNo\3/g;
s/0\s*!=\s*strcmp(\(.*\)TraderNo.*,\s*\([^)]*\))\(.*\)/\1TraderNo != \2\3/g;
s/0\s*==\s*strcmp(\(.*\),\s*\(.*\)TraderNo[^)]*)\(.*\)/\1 == \2TraderNo\3/g;
s/0\s*==\s*strcmp(\(.*\)TraderNo.*,\s*\([^)]*\))\(.*\)/\1TraderNo == \2\3/g;
s/atoi(\(.*\)TraderNo[^)]*)\(.*\)/\1TraderNo\2/g;
s/!(\(.*\)\s*==\s*\(.*\)TraderNo)/\1 != \2TraderNo/g;
;
s/ContractID.buf/ContractID/g;
s/BYTES_TO_BYTES(\(.*\),\s*\(.*\)ContractID.*);/\1 = \2ContractID;/g;
s/BYTES_TO_BYTES(\(.*\)ContractID.*,\s*\(.*\));/\1ContractID = \2;/g;
s/0\s*!=\s*strlen(.*ContractID.*&&\s*//g;
s/0\s*!=\s*strcmp(\(.*\),\s*\(.*\)ContractID[^)]*)\(.*\)/\1 != \2ContractID\3/g;
s/0\s*!=\s*strcmp(\(.*\)ContractID.*,\s*\([^)]*\))\(.*\)/\1ContractID != \2\3/g;
s/0\s*==\s*strcmp(\(.*\),\s*\(.*\)ContractID[^)]*)\(.*\)/\1 == \2ContractID\3/g;
s/0\s*==\s*strcmp(\(.*\)ContractID.*,\s*\([^)]*\))\(.*\)/\1ContractID == \2\3/g;
s/atoi(\(.*\)ContractID[^)]*)\(.*\)/\1ContractID\2/g;
s/!(\(.*\)\s*==\s*\(.*\)ContractID)/\1 != \2ContractID/g;
;
s/ArbiContractID.buf/ArbiContractID/g;
s/BYTES_TO_BYTES(\(.*\),\s*\(.*\)ArbiContractID.*);/\1 = \2ArbiContractID;/g;
s/BYTES_TO_BYTES(\(.*\)ArbiContractID.*,\s*\(.*\));/\1ArbiContractID = \2;/g;
s/0\s*!=\s*strlen(.*ArbiContractID.*&&\s*//g;
s/0\s*!=\s*strcmp(\(.*\),\s*\(.*\)ArbiContractID[^)]*)\(.*\)/\1 != \2ArbiContractID\3/g;
s/0\s*!=\s*strcmp(\(.*\)ArbiContractID.*,\s*\([^)]*\))\(.*\)/\1ArbiContractID != \2\3/g;
s/0\s*==\s*strcmp(\(.*\),\s*\(.*\)ArbiContractID[^)]*)\(.*\)/\1 == \2ArbiContractID\3/g;
s/0\s*==\s*strcmp(\(.*\)ArbiContractID.*,\s*\([^)]*\))\(.*\)/\1ArbiContractID == \2\3/g;
s/atoi(\(.*\)ArbiContractID[^)]*)\(.*\)/\1ArbiContractID\2/g;
s/!(\(.*\)\s*==\s*\(.*\)ArbiContractID)/\1 != \2ArbiContractID/g;
;
s/ClearAccountNo.buf/ClearAccountNo/g;
s/BYTES_TO_BYTES(\(.*\),\s*\(.*\)ClearAccountNo.*);/\1 = \2ClearAccountNo;/g;
s/BYTES_TO_BYTES(\(.*\)ClearAccountNo.*,\s*\(.*\));/\1ClearAccountNo = \2;/g;
s/0\s*!=\s*strlen(.*ClearAccountNo.*&&\s*//g;
s/0\s*!=\s*strcmp(\(.*\),\s*\(.*\)ClearAccountNo[^)]*)\(.*\)/\1 != \2ClearAccountNo\3/g;
s/0\s*!=\s*strcmp(\(.*\)ClearAccountNo.*,\s*\([^)]*\))\(.*\)/\1ClearAccountNo != \2\3/g;
s/0\s*==\s*strcmp(\(.*\),\s*\(.*\)ClearAccountNo[^)]*)\(.*\)/\1 == \2ClearAccountNo\3/g;
s/0\s*==\s*strcmp(\(.*\)ClearAccountNo.*,\s*\([^)]*\))\(.*\)/\1ClearAccountNo == \2\3/g;
s/atoi(\(.*\)ClearAccountNo[^)]*)\(.*\)/\1ClearAccountNo\2/g;
s/!(\(.*\)\s*==\s*\(.*\)ClearAccountNo)/\1 != \2ClearAccountNo/g;
;
s/ExchCode.buf/ExchCode/g;
s/BYTES_TO_BYTES(\(.*\),\s*\(.*\)ExchCode.*);/\1 = \2ExchCode;/g;
s/BYTES_TO_BYTES(\(.*\)ExchCode.*,\s*\(.*\));/\1ExchCode = \2;/g;
s/0\s*!=\s*strlen(.*ExchCode.*&&\s*//g;
s/0\s*!=\s*strcmp(\(.*\),\s*\(.*\)ExchCode[^)]*)\(.*\)/\1 != \2ExchCode\3/g;
s/0\s*!=\s*strcmp(\(.*\)ExchCode.*,\s*\([^)]*\))\(.*\)/\1ExchCode != \2\3/g;
s/0\s*==\s*strcmp(\(.*\),\s*\(.*\)ExchCode[^)]*)\(.*\)/\1 == \2ExchCode\3/g;
s/0\s*==\s*strcmp(\(.*\)ExchCode.*,\s*\([^)]*\))\(.*\)/\1ExchCode == \2\3/g;
s/atoi(\(.*\)ExchCode[^)]*)\(.*\)/\1ExchCode\2/g;
s/!(\(.*\)\s*==\s*\(.*\)ExchCode)/\1 != \2ExchCode/g;
" ./111/$F > ./222/$F

done

