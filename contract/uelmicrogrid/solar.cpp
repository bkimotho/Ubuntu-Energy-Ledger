#include <eosiolib/eosio.hpp>

using namespace std;
using namespace eosio;


class solar : public contract {
    public:
        solar(account_name self):contract(self),
                                    _donors(self, self),
                                    _clients(self, self),
                                    _charges(self,self),
                                    _allocations(self, self)
        {}


        [[eosio::action]]
        void charge(account_name client, string timestamp, uint64_t amount, uint64_t id, uint64_t percentage){
            _charges.emplace(client, [&]auto& rcrd){
                rcrd.client = client;
                rcrd.timestamp = timestamp;
                rcrd.amount = amount;
                rcrd.id = id;

            }
            // charge()

           
        }
    private:

        void charge(account_name to, account_name from, amount, percentage){
             // Allocate transaction costs
             quantity = amount * percentage;  
            action(
                permission_level{ from, N(active) },
                N(eosio.token), N(transfer),
                std::make_tuple(from, to, quantity, std::string(""))
                ).send();
        }

        struct [[eosio::table]] donor{
            uint64_t id;
            account_name name;
            uint64_t primary_key() const{};
        };
        typedef eosio::multi_index<N(donors), donor> donor_table;

        struct [[eosio::table]] client{
            uint64_t id;
            account_name name;
             uint64_t primary_key() const{};
        };
        typedef eosio::multi_index<N(clients), client> client_table;

        struct [[eosio::table]] charge{
            uint64_t id;
            account_name client;
            string timestamp;
            uint64_t amount;
            
            uint64_t primary_key() const{};
        };
        typedef eosio::multi_index<N(charges), charge> charge_table;

        struct [[eosio::table]] allocation{
            account_name recipient;
            uint64_t amount;

            uint64_t primary_key() const{};
        };

        typedef eosio::multi_index<N(allocations), allocation> allocation_table;

        donor_table _donors;
        client_table _clients;
        charge_table _charges;
        allocation_table _allocations;
};
