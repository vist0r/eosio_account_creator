#include "create.hpp"		
void create::crt(name newact,std::string owner,std::string active){	
	std::array<char, 33> owner_pubkey_char;
	std::array<char, 33> active_pubkey_char;

    const auto cpu = asset(1500,EOS_SYMBOL);
    const auto net = asset(500,EOS_SYMBOL);

    const abieos::public_key owner_pubkey = abieos::string_to_public_key(owner);
    const abieos::public_key active_pubkey = abieos::string_to_public_key(active);

 	std::copy(owner_pubkey.data.begin(), owner_pubkey.data.end(),owner_pubkey_char.begin());
 	std::copy(active_pubkey.data.begin(), active_pubkey.data.end(),active_pubkey_char.begin());

 	const auto owner_auth = authority{
        1, {{{(uint8_t)abieos::key_type::k1, owner_pubkey_char}, 1}}, {}, {}};
    const auto active_auth = authority{
        1, {{{(uint8_t)abieos::key_type::k1, active_pubkey_char}, 1}}, {}, {}};
 	
    action(
    	permission_level{get_self(),"active"_n},
    	"eosio"_n,"newaccount"_n,
    	std::make_tuple(get_self(),newact,owner_auth,active_auth)
    ).send();

    action(
    	permission_level{get_self(),"active"_n},
    	"eosio"_n,"buyram"_n,
    	std::make_tuple(get_self(),newact,asset(1500,EOS_SYMBOL))
    ).send();
    
    action(
    	permission_level{get_self(),"active"_n},
    	"eosio"_n,"delegatebw"_n,
    	std::make_tuple(get_self(),newact,net,cpu,1)
    ).send();
}

