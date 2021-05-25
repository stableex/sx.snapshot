#include <eosio.token/eosio.token.hpp>

#include "snapshot.sx.hpp"

namespace sx {

[[eosio::action]]
void code::snapshot( const symbol_code symcode, const set<name> accounts )
{
    require_auth( get_self() );

    code::vaults_table _vaults( get_self(), symcode.raw() );
    const int64_t rate = (symcode == symbol_code{"SXEOS"}) ? 92717849 : 91611901;
    check( symcode == symbol_code{"SXEOS"} || symcode == symbol_code{"SXUSDT"}, "must be SXEOS or SXUSDT" );

    // validate accounts
    for ( const name account : accounts ) {
        check( is_account( account ), get_self().to_string() + "::snapshot: " + account.to_string() + " account does not exists");

        auto itr = _vaults.find( account.value );
        const symbol sym = symcode == symbol_code{"SXEOS"} ? symbol{"EOS", 4} : symbol{"USDT", 4};
        const asset balance = eosio::token::get_balance( "token.sx"_n, account, symcode );
        if ( balance.amount <= 10000 ) {
            if ( itr != _vaults.end() ) _vaults.erase( itr );
            continue;
        }

        // initialize quantities
        auto insert = [&]( auto & row ) {
            row.account = account;
            row.snapshot = current_time_point();
            row.reclaimed.symbol = symbol{ symcode, 4 };
            row.balance = balance;
            row.claimable = asset{ row.balance.amount * 10000 / rate, sym };
            row.claimed.symbol = sym;

        };

        // create/modify order
        if ( itr == _vaults.end() ) _vaults.emplace( get_self(), insert );
        else _vaults.modify( itr, get_self(), insert );
    }
}

[[eosio::action]]
void code::stats( const symbol_code symcode )
{
    code::vaults_table _vaults( get_self(), symcode.raw() );
    check( symcode == symbol_code{"SXEOS"} || symcode == symbol_code{"SXUSDT"}, "must be SXEOS or SXUSDT" );
    const symbol sym = symcode == symbol_code{"SXEOS"} ? symbol{"EOS", 4} : symbol{"USDT", 4};

    asset balance = asset{0, symbol{ symcode, 4 }};
    asset reclaimed = asset{0, symbol{ symcode, 4 }};
    asset claimable = asset{0, sym};
    asset claimed = asset{0, sym};

    for ( const auto row : _vaults ) {
        balance += row.balance;
        reclaimed += row.reclaimed;
        claimable += row.claimable;
        claimed += row.claimed;
    }
    check(false, "balance: " + balance.to_string() + " | reclaimed: " + reclaimed.to_string() + " | claimable: " + claimable.to_string() + " | claimed: " + claimed.to_string());
}

[[eosio::action]]
void code::claim( const name account, const asset balance, const asset claimable )
{
    require_auth( get_self() );

    code::vaults_table _vaults( get_self(), balance.symbol.code().raw() );
    auto itr = _vaults.find( account.value );

    check( itr != _vaults.end(), account.to_string() + " account does not exists" );
    _vaults.modify( itr, get_self(), [&]( auto & row ) {
        row.reclaimed += balance;
        row.claimed += claimable;
        check( row.reclaimed == row.balance, account.to_string() + " reclaimed must equal to balance");
        check( row.claimed == row.claimable, account.to_string() + " claimed must equal to claimable");
    });
}

}
