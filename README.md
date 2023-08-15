# tlns-key-gen

Generate keys for ABP LoRaWAN devices.

Clone [Github repository](https://github.com/commandus/tlns-key-gen):
```
git clone git@github.com:commandus/tlns-key-gen.git
```
or
```
git clone https://github.com/commandus/tlns-key-gen.git
```

First install dependencies (see below) and then configure and make project using Autotools:

```
cd tlns-key-gen
./autogen.sh
./configure
make
```

or using CMake:
```
cd tlns-key-gen
mkdir build
cd build
cmake ..
make
```

You can use CMake with care, it may be inconsistent. You need check missed sources in the CMake script.

## Usage

Pass address:
```
./tlns-key-gen 12345
12345 c97aa23095081239 6ad8eabe16c9a45a0117f89a9eb4b114 159bf2f1390dbe285e27fda02c90cb2b
```

Set header on with -v option:
```
./tlns-key-gen 12345 -v
| Addr  | EUI            | NWK                            | APP                            |
+-------+----------------+--------------------------------+--------------------------------+
   12345 3c1b76edd435a9d9 840d9c22909812b3d8545d7b139b7829 109be47226c36ed0d6e58c6cb303d572 
```

Set predefined "master key":
```
./tlns-key-gen 12345 -v -k MySecret
key: MySecret
| Addr  | EUI            | NWK                            | APP                            |
+-------+----------------+--------------------------------+--------------------------------+
12345 a51368a37521de88 5621cd9ee2282ff8e0b0b35890989fa8 06c67db2dd9c5084e46a6194b4176f8b 
```
