# Bitcoin Transaction Lab – C Implementation

## Overview

This project implements the workflow required for the **CS216 Bitcoin Transaction Lab** using **C** and **Bitcoin Core RPC**.

The program interacts directly with a running `bitcoind` node in **regtest mode** to:

1. Generate addresses
2. Fund wallets
3. Create and broadcast transactions
4. Construct and sign raw transactions
5. Demonstrate the **UTXO model**
6. Compare **Legacy (P2PKH)** and **SegWit (P2SH-P2WPKH)** transactions

The implementation communicates with `bitcoind` using **JSON-RPC via libcurl** and parses responses using **cJSON**.

---

# Features

The program automatically performs the complete workflow for both **Legacy** and **SegWit** transactions.


# Project Structure

```
src/
│
├── main.c
│   Entry point. Executes the full transaction workflow.
│
├── bitcoin.c
├── bitcoin.h
│   High-level Bitcoin logic (address generation, transactions,
│   UTXO discovery, block generation).
│
├── rpc.c
├── rpc.h
│   JSON-RPC client that communicates with bitcoind using libcurl.
```

---

# Dependencies

The project requires:

* **Bitcoin Core**
* **libcurl**
* **cJSON**
* **CMake**

Install dependencies (Linux example):

```
sudo pacman -S bitcoin curl cjson cmake
```

or

```
sudo apt install bitcoind libcurl4-openssl-dev libcjson-dev cmake
```

---

# Bitcoin Core Setup

Create a `bitcoin.conf` file:

```
regtest=1
server=1
rpcuser=user
rpcpassword=password
txindex=1
```

Start the node:

```
bitcoind -regtest -daemon
```

---

# Building the Project

```
mkdir build
cd build
cmake ..
make
```

---

# Running the Program

```
./Just_Hash_It_Assignment_2
```


# Author

Saransh Halwai - 240001066 <br/>
Ankur Singh - 240001009 <br/>
Anurag Prasad - 240001011 <br/>
Vavadiya Rudra Bhaveshbhai - 240041038

CS216 

C implementation using Bitcoin Core RPC
