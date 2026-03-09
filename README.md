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

### Legacy (P2PKH)

1. Generate addresses **A, B, C**
2. Mine blocks to fund address **A**
3. Send transaction **A → B**
4. Extract **UTXO for B**
5. Create raw transaction **B → C**
6. Fund transaction (adds fee + change)
7. Sign transaction
8. Broadcast transaction

### SegWit (P2SH-P2WPKH)

The same workflow is repeated using SegWit addresses:

1. Generate addresses **A', B', C'**
2. Send **A' → B'**
3. Extract **UTXO**
4. Create raw transaction **B' → C'**
5. Sign and broadcast

This demonstrates the structural differences between **legacy scriptSig unlocking** and **SegWit witness-based unlocking**.

---

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

Example output:

```
===== LEGACY TEST =====
A: mzmYKucSPMtdmxbikBS5GMbxgZRiTo1cEe
B: mhUNyfa6prEx8pn87voYUkH7QotwAEXxgH
C: mnabvj5q7PzAUw7xw2JPnaAJL5qUYHhruN
A → B txid: ...
UTXO for B:
txid: ...
vout: ...
B → C txid: ...

===== SEGWIT TEST =====
A': 2MwwXR2gF9y4AfdG7Hhtax9zXAxMZLFMSqe
B': 2N8C7Wse1kXHeKFNj4ec5s6NiRYTxqTifEQ
C': 2NC3Jd3KxPf8Nu8SCbtidsVM2C614BzSz2i
A' → B' txid: ...
UTXO for B':
txid: ...
vout: ...
B' → C' txid: ...
```

---

# Key Concepts Demonstrated

## UTXO Model

Bitcoin transactions spend **unspent outputs** instead of account balances.

```
A → B creates a UTXO
B → C spends that UTXO
```

The program automatically extracts the UTXO from `listunspent` and uses it as input to build the next transaction.

---

## Legacy Transactions (P2PKH)

Legacy inputs contain unlocking data inside `scriptSig`.

```
scriptSig:
<signature> <public key>

scriptPubKey:
OP_DUP OP_HASH160 <pubKeyHash>
OP_EQUALVERIFY
OP_CHECKSIG
```

---

## SegWit Transactions (P2SH-P2WPKH)

SegWit separates signatures into **witness data**.

```
scriptSig: (mostly empty)

txinwitness:
<signature>
<public key>
```

This reduces transaction size and improves scalability.

---

## Size Comparison

Example difference:

| Transaction Type   | Size       | vsize       |
| ------------------ | ---------- | ----------- |
| Legacy P2PKH       | ~225 bytes | ~225 vbytes |
| SegWit P2SH-P2WPKH | ~224 bytes | ~143 vbytes |

SegWit transactions are cheaper because witness data receives a **weight discount**.

---

# RPC Methods Used

The implementation uses the following Bitcoin RPC calls:

```
getnewaddress
sendtoaddress
listunspent
createrawtransaction
fundrawtransaction
signrawtransactionwithwallet
sendrawtransaction
generatetoaddress
```

---

# Educational Goals

This project demonstrates:

* Bitcoin transaction construction
* The UTXO spending model
* Script locking and unlocking
* SegWit transaction structure
* Interaction with Bitcoin Core via RPC

---

# Notes

This program is designed for **regtest mode** only.
Regtest allows instant block generation and is intended for development and testing.

---

# Author

CS216 Bitcoin Transaction Lab
C implementation using Bitcoin Core RPC
