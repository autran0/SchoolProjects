#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from errno import ECONNREFUSED
from functools import partial
from multiprocessing import Pool
from datetime import datetime
import socket, subprocess, sys, argparse, os

NUM_CORES = 4
flag = 0
host=None

line = "+" * 80 # Just a fancy line consisting '+'
desc = line+'''\nPort Scanner
Example usage: python port_scanner.py www.hackthissite.org 1 1000\n'''+line+"\n"

parser = argparse.ArgumentParser(description = desc, formatter_class=argparse.RawTextHelpFormatter)
parser.add_argument('host', metavar='H', help='Host name you want to scan')
parser.add_argument('startport', metavar='P1', nargs='?', help='Start scanning from this port')
parser.add_argument('endport', metavar='P2', nargs='?',help='Scan until this port')
args = parser.parse_args()

if (args.startport) and args.endport :
	# If this condition is true, the script will scan over this port range
	host = args.host
	start_port = int(args.startport)
	end_port = int(args.endport)
else:
	host=args.host
	flag = 1

subprocess.call('clear', shell=True)
open_ports = []

closed_port_count = 0
common_ports = {
	21, 22, 23, 25, 53, 69, 80, 109, 110,
	123, 137, 138, 139, 143, 156, 389, 
	443, 546, 547, 995, 993, 2086, 2087, 
	2082, 2083, 3306, 8443, 10000
}

def ping(host, port, result=1):
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    	sock.settimeout(0.5)
	r = sock.connect_ex((host, port))
	if r == 0:
	    result = r 
	    serv = socket.getservbyport(port)

	    if sock.proto == 0:
		pro = 'tcp'
	    else:
		pro = 'udp'
	    if serv:
	        print '{0:<8} {1:<8} {2:<8} {3}'.format(port, pro, 'Open', serv)
	    else:
		print '{0:<8} {1:<8} {2:<8} {3}'.format(port, pro, 'Open', 'Unknown')
	sock.close()
    except Exception, e:
        pass
        
    return result


def scan_ports(host):
    if flag:
	closed_port_count = len(common_ports)
	for p in sorted(common_ports):
	    sys.stdout.flush() # flush the stdout buffer.
    	    response = ping(host, p) # call the function to connect to the port
    	    if response == 0:
	        open_ports.append(p)
	    	closed_port_count = closed_port_count - 1
    else:
	closed_port_count = end_port-start_port
	for p in range(start_port, end_port+1):
	    sys.stdout.flush() # flush the stdout buffer.
    	    response = ping(host, p) # call the function to connect to the port
    	    if response == 0:
	        open_ports.append(p)
	    	closed_port_count = closed_port_count - 1
    return closed_port_count

def main():
    t1 = datetime.now()

    try:
    	print("Scanning ports on " + host + " ...\n")
	print 'Port     Type     Status   Service'
    	ports = scan_ports(host)

    except KeyboardInterrupt:
    	print "You pressed Ctrl+C"
    	sys.exit()

    except socket.gaierror:
    	print 'Hostname could not be resolved. Exiting'
    	sys.exit()

    except socket.error:
    	print "Couldn't connect to server"
    	sys.exit()

    print ports, 'scanned closed ports'

    t2 = datetime.now()
    total = t2 - t1
    print '\nScanning Completed in ', total

if __name__ == "__main__":
    main()

