#!/bin/bash

# build
eosio-cpp snapshot.sx.cpp -I include
cleos set contract snapshot.sx . snapshot.sx.wasm snapshot.sx.abi
