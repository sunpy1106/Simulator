#!/bin/bash

if [ $UID != 0 ];then
	echo "You must be a root"
	exit
fi

ulimit -n 65535


sed 's/\(ServerBand=\).*/\14000/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
sed 's/\(SourceNums=\).*/\1100/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
sed 's/\(ClientNums=\).*/\1150/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
sed 's/\(Multiple=\).*/\11/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
sed 's/\(Thelta=\).*/\1729/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
sed 's/\(isP2POpen=\).*/\1true/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
sed 's/\(PerSendSize=\).*/\11000/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
sed 's/\(Special=\).*/\1true/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
sed 's/\(isRepeat=\).*/\1true/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;

BUFFER=(DW)
STARTNUMS=1
ENDNUMS=3

sed 's/\(BufferStrategy=\).*/\1'"DWS"'/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
sed 's/\(ClientBufStrategy=\).*/\1'"${BUFFER[$i]}"'/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
for((i=$STARTNUMS;i<=$ENDNUMS;i++))
do
	rm -rf data/requestFiel*.log
	rm -rf data/client*.log
	rm -rf data/sequence.log
	cp -rf test_$i/requestFile*.log data/
	if [ $i -ge 0 ] && [ $i -lt 4 ];then
		sed 's/\(BlockSize=\).*/\110000/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
		sed 's/\(BlockNums=\).*/\120/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
		sed 's/\(ServerBlockNums=\).*/\11000/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
		sed 's/\(isP2POpen=\).*/\1true/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
		sed 's/\(IsUseRealDevice=\).*/\1true/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
	fi
	if [ $i = 4 ];then
		sed 's/\(BlockSize=\).*/\15000/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
		sed 's/\(BlockNums=\).*/\140/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
	fi
	if [ $i = 5 ];then
		sed 's/\(BlockSize=\).*/\120000/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
		sed 's/\(BlockNums=\).*/\1100/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
	fi
	if [ $i -ge 6 ];then
		sed 's/\(BlockSize=\).*/\110000/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
		sed 's/\(BlockNums=\).*/\140/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
	fi

        ./server_exe > server.log &
	sleep 1
       ./client_exe > client.log &
	
	sleep 5000

	while true
	do
		sed -n '$,$p' server.log > temp
		nowtimes=`sed 's/At:\(.*\) server .*/\1/g' temp`
		if [ $(echo "${nowtimes} > 5001" | bc)x = 1x ];then
			break
		fi
		sleep 1
	done

	killall server_exe
	killall client_exe
	rm -rf test_$i/client*.log
	rm -rf test_$i/sequence.log
#./optResult data/sequence.log >> data/sequence.log 
	mv data/sequence.log test_$i/
	mv data/client*.log test_$i/
	mv data/result.log test_$i/
done
