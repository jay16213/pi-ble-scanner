use [beacontools](https://github.com/citruz/beacontools.git) to scan ibeacon message


## RUN
first run C_socketServer, than run scanner.py as a client

```
gcc C_socketServer.c -o C_socketServer
./C_socketServer <port>
```

```
python3 scanner.py <id> <server ip address> <port>
```

id: a nonnegative integer that help server recognize different raspi client
