#!/bin/bash

# unlock wallet
cleos wallet unlock --password $(cat ~/eosio-wallet/.pass)

# init
cleos push action snapshot.sx snapshot '["SXEOS", ["myaccount", "toaccount"]]' -p snapshot.sx
cleos push action snapshot.sx snapshot '["SXUSDT", ["myaccount", "toaccount"]]' -p snapshot.sx

cleos push action snapshot.sx claim '["myaccount", "10000.0000 SXEOS", "1.0785 EOS"]' -p snapshot.sx
cleos push action snapshot.sx claim '["myaccount", "20000.0000 SXUSDT", "19.6481 USDT"]' -p snapshot.sx
