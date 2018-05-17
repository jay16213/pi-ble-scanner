# Raspberry Pi Ble Scanner System

> use [beacontools](https://github.com/citruz/beacontools.git) to scan ibeacon message

## Build
```
make
```
this will help you compile a socket server `C_socketServer`

## RUN
- first, run `C_socketServer` to create a tcp server
```
./C_socketServer <port>
```

- then run scanner on pi to scan beacon message
```
sudo python3 scanner.py <id> <server ip address> <port>
```

argument `id` is a nonnegative integer that help server recognize different raspi client

Server 會輸出3個 log 檔，兩個是紀錄2邊 pi 傳過來的beacon數量，1個是統整之後計算出在交集範圍內的合法 beacon 數，檔名為 scan_result.log。讀 scan_result.log 即可得知目前位在2個 pi beacon掃描交集範圍內的合法 beacon 數量
