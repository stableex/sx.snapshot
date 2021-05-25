#!/bin/bash

# unlock wallet
cleos wallet unlock --password $(cat ~/eosio-wallet/.pass)

# create account
cleos create account eosio snapshot.sx EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio token.sx EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio myaccount EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio toaccount EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV

# contract
cleos set contract snapshot.sx . snapshot.sx.wasm snapshot.sx.abi
cleos set contract token.sx ./include/eosio.token eosio.token.wasm eosio.token.abi

# create A tokens
cleos push action token.sx create '["eosio", "100000000.0000 SXEOS"]' -p token.sx
cleos push action token.sx issue '["eosio", "5000000.0000 SXEOS", "init"]' -p eosio
cleos push action token.sx create '["eosio", "100000000.0000 SXUSDT"]' -p token.sx
cleos push action token.sx issue '["eosio", "5000000.0000 SXUSDT", "init"]' -p eosio

cleos transfer eosio myaccount "5000.0000 SXEOS" --contract token.sx
cleos transfer eosio myaccount "10000.0000 SXUSDT" --contract token.sx
cleos transfer eosio toaccount "0.0000 SXEOS" --contract token.sx
cleos transfer eosio toaccount "90000.0000 SXUSDT" --contract token.sx
