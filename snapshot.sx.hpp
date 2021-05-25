#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>

using namespace eosio;
using namespace std;

namespace sx {

class [[eosio::contract("snapshot.sx")]] code : public eosio::contract {
public:
    using contract::contract;

    /**
     * ## TABLE `state`
     *
     * - `{set<name>} whitelist` - whitelist accounts authorized to withdraw
     * - `{time_point_sec} lock_until` - withdrawns unavailable until lock period is past
     *
     * ### example
     *
     * ```json
     * {
     *     "account": "sx",
     *     "snapshot": "2021-03-06T00:00:00",
     *     "balance": "10000.0000 SXEOS",
     *     "reclaimed": "0.0000 SXEOS",
     *     "claimable": "1.0000 EOS",
     *     "claimed": "0.0000 EOS"
     * }
     * ```
     */
    struct [[eosio::table("vaults")]] vaults_row {
        name                account;
        time_point_sec      snapshot;
        asset               balance;
        asset               reclaimed;
        asset               claimable;
        asset               claimed;

        uint64_t primary_key() const { return account.value; }
    };
    typedef eosio::multi_index< "vaults"_n, vaults_row > vaults_table;

    [[eosio::action]]
    void snapshot( const symbol_code symcode, const set<name> accounts );

    [[eosio::action]]
    void stats( const symbol_code symcode );

    [[eosio::action]]
    void claim( const name account, const asset balance, const asset claimable );

    // action wrappers
    using snapshot_action = eosio::action_wrapper<"snapshot"_n, &sx::code::snapshot>;
};
}