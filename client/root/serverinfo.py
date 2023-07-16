import app

app.ServerName = None
Windows_IP = '127.0.0.1'
FreeBSD_IP = '51.254.23.238'

STATE_NONE = '...'
		
STATE_DICT = {
	0 : '....',
	1 : 'NORM',
	2 : 'BUSY',
	3 : 'FULL'	}

SERVER1_CHANNEL_DICT = {
	1:{'key':11,'name':'Channel1','ip':Windows_IP,'tcp_port':13001,'udp_port':13001,'state':STATE_NONE,},
	2:{'key':12,'name':'Channel2','ip':Windows_IP,'tcp_port':13010,'udp_port':13010,'state':STATE_NONE,},
}

SERVER2_CHANNEL_DICT = {
	1:{'key':21,'name':'Channel1','ip':FreeBSD_IP,'tcp_port':13001,'udp_port':13001,'state':STATE_NONE,},
}

REGION_NAME_DICT = {
	0 : 'GERMANY',
}

REGION_AUTH_SERVER_DICT = {
	0 : {
		1 : { 'ip':Windows_IP, 'port':999, }, 
		# NOTE: You can adjust your ports here from auth_1 ~ auth_4
		2 : { 'ip':FreeBSD_IP, 'port':999, },
		}	
}

REGION_DICT = {
	0 : {
		1 : { 'name' : 'Windows', 'channel' : SERVER1_CHANNEL_DICT, },
		2 : { 'name' : 'FreeBSD', 'channel' : SERVER2_CHANNEL_DICT, },
		},
}

MARKADDR_DICT = {
	10 : { 'ip' : Windows_IP, 'tcp_port' : 13001, 'mark' : '10.tga', 'symbol_path' : '10', },
	20 : { 'ip' : FreeBSD_IP, 'tcp_port' : 13001, 'mark' : '10.tga', 'symbol_path' : '10', },
	}

TESTADDR = { 'ip' : Windows_IP, 'tcp_port' : 50000, 'udp_port' : 50000, }

#DONE
