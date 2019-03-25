#!/bin/bash
# UDP broadcast for Lap Counter host
# This script needs to start on boot (e.g. via SYSTEM D)
#
# Waits until a packet is received from the client
# then sends config info to client(s) (via broadcast)

# network device
DEV=eth0
# id to broadcast
HOSTID="LC1"
# hostname
HOSTNAME=`hostname`
# broadcast port
BC_PORT=2000
# telemetry port (where the server listens for telemetry packets)
TELEMETRY_PORT=2006
# server URL
SERVERURL="galston500.com"
SERVERPAGE="lapcount.php?"
SERVERPORT=80
#broadcast interval in seconds
INTERVAL=10


# wait for system to settle down
sleep 20

#get the broadcast address
bc_address=`/bin/ip a s dev $DEV | awk '/inet / {print $4}'`
echo "Broadcasting on $bc_address"

# broadcast loop
while true
do
	# wait for a packet to arrive at broadcast port
	nc_output="$(/bin/nc -u -l -p $BC_PORT -w 0)"
	# echo "packet received"
	# split client string into array
	read -r -a client_data <<< "$nc_output"
	# check array size
	# echo "array size: " ${#client_data[@]}
	if [ ${#client_data[@]} -eq 1 ]; then
		# send client config data packet
	        echo -e "$HOSTID\t$TELEMETRY_PORT\t$HOSTNAME\t$SERVERURL\t$SERVERPORT\t$SERVERPAGE\t" | /bin/nc -ub -w0 $bc_address $BC_PORT
        	echo "answered request from " ${client_data[0]}
	fi
        # sleep $INTERVAL
done

echo "Broadcast exited"
