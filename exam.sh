#!/bin/bash

if [ $UID != 0 ];then
	echo "You must be a root"
	exit
fi

function getrequestfile()
{
	rm data/requestList_$1.log
	rm data/file*.log
	for((z=1;z<=$1;z++))
	do
	      fileseg=`sed -n '1,1p' data/requestFile$z.log`;
	      echo $fileseg $z >> data/requestList_$1.log
	done
}

umount data
mount tmpramdisk data
ulimit -n 65535

sed 's/\(BufferStrategy=\).*/\1LRU/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
sed 's/\(ServerBand=\).*/\12000/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
sed 's/\(SourceNums=\).*/\1100/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
sed 's/\(ClientNums=\).*/\1150/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
sed 's/\(PerSendSize=\).*/\1200/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
sed 's/\(Special=\).*/\1false/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
sed 's/\(Prefetch=\).*/\11/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
sed 's/\(IsStartTogether=\).*/\1false/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
sed 's/\(Lambda=\).*/\1100/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;

RUNTIMES=$1
if [ ! $RUNTIMES ];then
	RUNTIMES=3
fi

CLIENTNUMS=(115 110)
CLIENTNUMSLENGTH=${#CLIENTNUMS[@]}
MULTI=(2)
MULTILENGTH=${#MULTI[@]}
ISP2P=(false)
ISP2PLENGTH=${#ISP2P[@]}
THELTA=(729)
THELTALENGTH=${#THELTA[@]}
PLAYTOPLAY=(600)
PLAYTOBACK=(400)
PLAYTOFOR=(0)
MMLENGTH=${#PLAYTOFOR[@]}

echo "****Test****"
echo "1.repeat isp2popen true or false test"
echo "2.repeat multiple 2 or 4 test"
echo "3.p2p effect test"
echo "4.limit value"
echo "5.all above(not recommended)"
echo -n "enter option:";read option

#repeat test
if [ ${option} == 1 ] || [ ${option} == 5 ];then
    sed 's/\(Multiple=\).*/\1'"${MULTI[0]}"'/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
    sed 's/\(Thelta=\).*/\1729/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
    sed 's/\(ClientNums=\).*/\1150/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;

    rm -rf data/requestFile*.log
    ./listgenerator
    getrequestfile ${CLIENTNUMS[0]}
    mv data/requestList_${CLIENTNUMS[0]}.log result/A_${CLIENTNUMS[0]}_${THELTA[0]}_R_${PLAYTOPLAY[0]}_${PLAYTOBACK[0]}_${PLAYTOFOR[0]}_${j}_requestList.log;

    for((i=0;i<$ISP2PLENGTH;i++))
    do
    	sed 's/\(isP2POpen=\).*/\1'"${ISP2P[$i]}"'/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
    	for((j=0;j<${RUNTIMES};j++))
    	do
    		killall server_exe;
    		killall client_exe;
    		./server_exe > server.log &
   	         sleep 1
    		./client_exe > client.log &
		sleeptimes=`expr ${MULTI[0]} \* 8000`
    		sleep $sleeptimes;
    		killall server_exe;
    		killall client_exe;
    		if [ ${ISP2P[$i]} == true ];then
#	    		mv server.log result/A_${CLIENTNUMS[0]}_${THELTA[0]}_Y_R_${PLAYTOPLAY[0]}_${PLAYTOBACK[0]}_${PLAYTOFOR[0]}_${j}_server.log;
	    		mv data/result.log result/A_${CLIENTNUMS[0]}_${THELTA[0]}_Y_R_${PLAYTOPLAY[0]}_${PLAYTOBACK[0]}_${PLAYTOFOR[0]}_${j}_result.log;
    		else
#	    		mv server.log result/A_${CLIENTNUMS[0]}_${THELTA[0]}_N_R_${PLAYTOPLAY[0]}_${PLAYTOBACK[0]}_${PLAYTOFOR[0]}_${j}_server.log;
	    		mv data/result.log result/A_${CLIENTNUMS[0]}_${THELTA[0]}_N_R_${PLAYTOPLAY[0]}_${PLAYTOBACK[0]}_${PLAYTOFOR[0]}_${j}_result.log;
    		fi
    	done
    done    
fi
    
if [ ${option} == 2 ] || [ ${option} == 5 ];then
    sed 's/\(ClientNums=\).*/\1'"${CLIENTNUMS[0]}"'/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
    sed 's/\(isP2POpen=\).*/\1'"${ISP2P[0]}"'/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
    sed 's/\(Thelta=\).*/\1'"${THELTA[0]}"'/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;

    rm -rf data/requestFile*.log
    ./listgenerator   
    getrequestfile ${CLIENTNUMS[0]}
    mv data/requestList_${CLIENTNUMS[0]}.log result/B_${CLIENTNUMS[0]}_${THELTA[0]}_R_${PLAYTOPLAY[0]}_${PLAYTOBACK[0]}_${PLAYTOFOR[0]}_requestList.log
    for((i=0;i<$MULTILENGTH;i++))
    do
    	sed 's/\(Multiple=\).*/\1'"${MULTI[$i]}"'/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
    	for((j=0;j<1;j++))
    	do
    		killall server_exe;
    		killall client_exe;
    		./server_exe > server.log &
        	sleep 1
    		./client_exe > client.log &
	        if [ ${MULTI[$i]} == 2 ];then
        	        sleeptimes=`expr ${MULTI[$i]} \* 8000`
	        else
                	sleeptimes=`expr ${MULTI[$i]} \* 8000`
		fi
		sleep $sleeptimes
    		killall server_exe;
	    	killall client_exe;
		mv data/result.log result/B_${CLIENTNUMS[0]}_${THELTA[0]}_Y_R_${PLAYTOPLAY[0]}_${PLAYTOBACK[0]}_${PLAYTOFOR[0]}_${MULTI[$i]}_result.log;
    	done
    done
fi

#p2p effect
if [ ${option} == 3 ] || [ ${option} == 5 ];then
	sed 's/\(Multiple=\).*/\1'"${MULTI[0]}"'/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;

	for((l=0;l<$THELTALENGTH;l++))
	do
		sed 's/\(Thelta=\).*/\1'"${THELTA[$l]}"'/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
		for((j=0;j<$CLIENTNUMSLENGTH;j++))
		do
			sed 's/\(ClientNums=\).*/\1'"${CLIENTNUMS[$j]}"'/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
			for((i=0;i<$ISP2PLENGTH;i++))
			do
				echo ${ISP2P[$i]}
				pause
				sed 's/\(isP2POpen=\).*/\1'"${ISP2P[$i]}"'/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
				for((n=0;n<$MMLENGTH;n++))
				do
					sed 's/\(PlayToPlay=\).*/\1'"${PLAYTOPLAY[$n]}"'/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
					sed 's/\(PlayToForward=\).*/\1'"${PLAYTOFOR[$n]}"'/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
					sed 's/\(PlayToBackward=\).*/\1'"${PLAYTOBACK[$n]}"'/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
					rm -rf data/requestFile*.log
					./listgenerator
					getrequestfile ${CLIENTNUMS[$j]}
					mv data/requestList_${CLIENTNUMS[$j]}.log result/C_${CLIENTNUMS[$j]}_${THELTA[$l]}_R_${PLAYTOPLAY[$n]}_${PLAYTOBACK[$n]}_${PLAYTOFOR[$n]}_requestList.log
					for((m=0;m<1;m++))
					do
						killall server_exe;
						killall client_exe;
						./server_exe > server.log &
						sleep 1
						./client_exe > client.log &
						sleeptimes=`expr ${MULTI[0]} \* 8000`
						sleep $sleeptimes
						killall server_exe;
						killall client_exe;
						if [ ${ISP2P[$i]} == true ];then
							mv data/result.log result/C_${CLIENTNUMS[$j]}_${THELTA[$l]}_Y_R_${PLAYTOPLAY[$n]}_${PLAYTOBACK[$n]}_${PLAYTOFOR[$n]}_${m}_result.log;
						else   
							mv data/result.log result/C_${CLIENTNUMS[$j]}_${THELTA[$l]}_N_R_${PLAYTOPLAY[$n]}_${PLAYTOBACK[$n]}_${PLAYTOFOR[$n]}_${m}_result.log;
						fi
					done
				done
			done
		done
	done
fi

if [ $option == 4 ] || [ $option == 5 ];then

	sed 's/\(ClientNums=\).*/\1'"${CLIENTNUMS[0]}"'/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
	sed 's/\(Multiple=\).*/\1'"${MULTI[0]}"'/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
	sed 's/\(Thelta=\).*/\1'"${THELTA[0]}"'/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;

	for((i=0;i<$RUNTIMES;i++))
	do
		rm -rf data/requestFile*.log
		./listgenerator

		sed 's/\(isP2POpen=\).*/\1true/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
		./server_exe > server.log &
		sleep 1
		./client_exe > client.log &
		while true
		do
			grep "reach" server.log > temp
			if [ -s temp ];then
				break
			else
				sleep 100
			fi
		done

		killall server_exe
		killall client_exe

		mv temp result/E_${THELTA[0]}_${i}_limitvalue.log
		mv data/result.log result/E_Y_${THELTA[0]}_${i}_result.log
	

		sed 's/\(isP2POpen=\).*/\1false/g' config/simulator.cfg > temp;cat temp > config/simulator.cfg;
		./server_exe > server.log &
		sleep 1
		./client_exe > client.log &

		while true
		do
			grep "reach" server.log > temp
			if [ -s temp ];then
				break
			else
				sleep 100
			fi
		done
		killall server_exe
		killall client_exe
		cat temp >> result/E_${THELTA[0]}_${i}_limitvalue.log
		mv data/result.log result/E_N_${THELTA[0]}_${i}_result.log
	done
fi
