## System prepataion
### Fedora
- Environment
```shell
sudo dnf install git
sudo dnf install gcc
sudo dnf install g++
sudo dnf -y install cmake
```

- jsoncpp
```shell
sudo dnf -y install jsoncpp-devel
```

- uuid
```shell
sudo dnf -y install uuid-devel
```

- zlib
```shell
sudo dnf -y install zlib-devel
```

- OpenSSL (Optional, if you want to support HTTPS)
```shell
sudo dnf -y install openssl-devel
```

- Sqlite3
```shell
sudo dnf -y install sqlite3
sudo dnf -y install sqlite-devel
```


## drogon install
### Linux
```shell
git clone https://github.com/drogonframework/drogon
cd drogon
git submodule update --init
mkdir build
cd build
# debug version
cmake ..
# release version
cmake -DCMAKE_BUILD_TYPE=Release ..
make && sudo make install
```
