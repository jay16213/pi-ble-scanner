# Raspberry Pi Ble Scanner System

> use [beacontools](https://github.com/citruz/beacontools.git) to scan ibeacon message

## Environment
- You must have python3 on your Pi

- install [beacontools[scan]](https://github.com/citruz/beacontools.git) on Pi
```sh
# install libbluetooth headers and libpcap2
sudo apt-get install python-dev libbluetooth-dev libcap2-bin

# grant the python executable permission to access raw socket data
sudo setcap 'cap_net_raw,cap_net_admin+eip' $(readlink -f $(which python3))

# install beacontools with scanning support
pip3 install beacontools[scan]
```

## Build
```
make
```
this will help you compile a socket server `C_socketServer`

## RUN
1. first, run `C_socketServer` to create a tcp server
```
./C_socketServer <port>
```
- 參數說明
    - port: tcp server 要 listen 的 port number

2. run scanner on pi to scan beacon message
```
sudo python3 scanner.py <id> <ip> <port>
```
- 參數說明
    - id: 一個大於 0 的整數，表示樹梅派的編號
    - ip: tcp server 的 ip address
    - port: tcp server 的 port number

## 功能說明
- Pi 會檢查收到的 beacon 的 uuid 是否合法，每隔 2 秒將收到的合法卡號回傳給 server

- server 根據 2 台不同的 Pi 回傳的合法卡號，計算位於兩台Pi beacon 掃描交集範圍的合法卡號數量，並將結果存在 `scan_result.log` 中

#### scan_result.log 範例

```
2
```

表示目前有兩個合法卡號 (兩個人) 位於兩台 Pi 的交集範圍內

#### 合法卡號清單 範例 (uuid.txt)

```bash
15345164-67ab-3e49-f9d6-e29000000012
15345164-67ab-3e49-f9d6-e29000000034
```

各個合法卡號分別獨立一行，在跑 scanner.py 時 Pi 即可載入合法卡號的清單以進行判斷
