# tlns-key-gen

Generate keys for ABP LoRaWAN devices.

## Build

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

## Preamble

There are 8 types of LoRaWAN networks numerated from 0 to 7 with different address capacity.

Each network has an identifier. Identifiers 0 and 1 are reserved for free usage without registration.

When you plan your LoRaWAN network first you will decide 

- network identifier
- address number

Key generator produce devEUI and ABP network and application keys by the address.

## Usage

Key generator has 2 generation mode:

- use pseudo-random generator (default mode). devEUI and all keys are pseudo-random.
- use "master" passphrase to generate series of devEUI and keys. devEUI and all keys calculated by the "master" passphrase.

In second mode "master" passphrase must keep in secret. This mode you can use when LoRaWAN identity service
does not have device credentials database but generate credentials on the fly using "master" passphrase.

Key generator produce devEUI and ABP keys by LoRaWAN network address. There are 3 types of address argument:

- address (hexadecimal 32-bit number)
- address range (two hexadecimal 32-bit numbers separated by "-" (minus) sign)
- address range (network type number, network identifier and two addresses)

Example of address:

f1234

Example of address range:

f1234-f4567

Example of address range (network type number, network identifier and two addresses):

1:22:ff00-ffff

### Address agrument

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

### Address list agruments

Pass two address:
```
./tlns-key-gen 12345 45678
   12345 6129908ce92413a7 0a4b1f7a87cd00dfcf840fc7de12fd35 3b803d44a34835ffc691feb3311d1c8a
   45678 ed661e3ac3a3bf0d 714c418fa1eb9be05d8bc566c4fc3580 9c1bf255f4f819a7800fda27bbb3d63d
```

Pass two address with two master passphrases:
```
./tlns-key-gen -k magenta -k yellow -v 12345 45678
key: magenta
| Addr  | EUI            | NWK                            | APP                            |
+-------+----------------+--------------------------------+--------------------------------+
   12345 b69c2b67fae9e08c caef7760e8d9088b1bfc70890d350368 d35f76172cf83c5d4622338d164fdde4
| Addr  | EUI            | NWK                            | APP                            |
+-------+----------------+--------------------------------+--------------------------------+
   45678 db13e87fb8445e4c 9c52c9736edc7f418c08a3bbd20088d2 cfb72bf695dde45ad428f024c665f4a4
key: yellow
| Addr  | EUI            | NWK                            | APP                            |
+-------+----------------+--------------------------------+--------------------------------+
   12345 0c90b5c2155695d8 c96d7ce3dfc47d15c8760829e421d676 e5ef9bf74eb2e362b37f51bbe1e4bd44
| Addr  | EUI            | NWK                            | APP                            |
+-------+----------------+--------------------------------+--------------------------------+
   45678 e536109bb8657171 fc92a6a64b65a0e83a51b59e110fbc1e 07671459d5d2a824c7ed5ce2727065f0
```

### Address range agruments

