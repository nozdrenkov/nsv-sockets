#include <iostream>
#include "../common/engine.hpp"
using namespace std;

void hint()
{
    puts("\n=== OPERATIONS ==========================================================");
    puts("info id            -- gets information about account id");
    puts("open               -- opens new account and gets new id for user");
    puts("close id           -- tries to close account with id");
    puts("add id amount      -- deposits money to account id");
    puts("get id amount      -- tries to take money from account id");
    puts("mov src dst amount -- tries to move money from account src to account dst");
    puts("=========================================================================\n");
}

int main()
{
    puts("CLIENT-BANK!");
    engine_t engine("client", "127.0.0.1", 5001);
    engine.connect();

    hint();
    
    while (true)
    {
        printf("> ");

        string query, ans;
        getline(cin, query);
        
        engine.write(query);
        engine.read(ans);
        
        puts("\n======= RESULT ====================================");
        puts(ans.c_str());
        puts("===================================================\n");
    }

    cout << "GOOD BYE!" << endl;
    return 0;
}