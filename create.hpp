#include <string>
#include <algorithm>
#include <eosiolib/asset.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/eosio.hpp>
#include <eosiolib/public_key.hpp>
#include <eosiolib/crypto.h>
#include "abieos_numeric.hpp"

using namespace eosio;

typedef uint16_t weight_type;
const auto EOS_SYMBOL = eosio::symbol("EOS", 4);
CONTRACT create : public contract{
		public :
			create( name receiver, name code, datastream<const char*> ds):contract(receiver,code,ds){}
	
			void apply(uint64_t receiver, uint64_t code, uint64_t action); 
			struct wait_weight {
				uint32_t wait_sec;
				weight_type weight;
			};
			struct key_weight {
				public_key   key;
				weight_type  weight;

				// explicit serialization macro is not necessary, used here only to improve compilation time
				EOSLIB_SERIALIZE( key_weight, (key)(weight) )
			};
			struct permission_level_weight {
				permission_level  permission;
				weight_type       weight;

				// explicit serialization macro is not necessary, used here only to improve compilation time
				EOSLIB_SERIALIZE( permission_level_weight, (permission)(weight) )
			};
			struct authority {
				uint32_t threshold;
				std::vector<key_weight> keys;
				std::vector<permission_level_weight> accounts;
				std::vector<wait_weight> waits;
			};
			
			ACTION crt(name newact,std::string owner,std::string active);

};

//EOSIO_ABI(cryptocertificate,(ontransfer)(initwl))

void create::apply(uint64_t receiver, uint64_t code, uint64_t action) {
	auto &thiscontract = *this;
	
	if (code != get_self().value) return;
	switch (action) {
		EOSIO_DISPATCH_HELPER(
            create,
           
            (crt)
        )
	};

}

extern "C"
{
	void apply(uint64_t receiver, uint64_t code,uint64_t action)
	{
		/*
		auto self = receiver;
		cryptocertificate thiscon(self);
		if (code == N(eosio.token) && action == N(transfer)){
			execute_action(&thiscon,&cryptocertificate::ontransfer);
			return;
		}
		cryptocertificate thiscontract(self);
		if (code != receiver) return;
		switch(action){
			EOSIO_API(cryptocertificate,(ontransfer)(init)(crtr))
		}
		*/
		create con(name(receiver),name(code),datastream<const char*>(nullptr,0));
		con.apply(receiver,code,action);
		eosio_exit(0);
	}
}
