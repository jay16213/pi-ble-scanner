import time, sys
import socket
import argparse
from beacontools import BeaconScanner, IBeaconFilter

parser = argparse.ArgumentParser()
parser.add_argument("id")
parser.add_argument("addr")
parser.add_argument("port", type=int)

def callback(bt_addr, rssi, packet, additional_info):
    # print("<%s, %d> %s %s" % (bt_addr, rssi, packet, additional_info))
    uuid = additional_info['uuid']

    global numOfPeople

    if uuid in validUUID:
        print("legal uuid: {}".format(uuid))
        # if this uuid is already scanned, ignore it
        if uuid not in scanList:
            scanList.append(uuid)
            numOfPeople += 1
    else:
        print("illegal uuid: {}".format(uuid))

    # fout.write("uuid: {}\n".format(uuid))

def getValidUUID():
    print("loading legal uuid")
    v = []
    with open('uuid.txt') as f:
        for line in f:
            uuid = line.split("\n")[0] # cut the newline character
            print(uuid)
            v.append(uuid)
    return v

if __name__=='__main__':
    args = parser.parse_args()

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((args.addr, args.port))

    numOfPeople = 0
    global validUUID
    validUUID = getValidUUID()
    scanner = BeaconScanner(callback)
    print("start scan")
    scanner.start()
    while True:
        try:
                fout = open("scan.txt", "w")
                print("----------")
                numOfPeople = 0
                scanList = []

                time.sleep(1)
                print("scanned: {}".format(numOfPeople))
                fout.write("{}\n".format(numOfPeople))
                fout.close()
                # fix msg to 1024 bytes and encode in ascii
                msg = str("{} {}".format(args.id, numOfPeople)).ljust(1024, '\x00').encode("ascii")
                sock.send(msg)

        except KeyboardInterrupt: # if user press ctrl-C => exit the program
            scanner.stop()
            sock.close()
            sys.exit(0)
