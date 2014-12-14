#include <iostream>
#include "../common/engine.hpp"
using namespace std;

void hint()
{
    puts("\n=== OPERATIONS =============================================");
    puts("show     dir                -- show files in directory dir");
    puts("rename   file    new_name   -- rename file");
    puts("copy     from    to         -- copy file");
    puts("remove   file               -- remove file");
    puts("exec     file               -- executes file.exe");
    puts("reboot                      -- reboots comp");
    puts("============================================================\n");
}

int main()
{
    puts("CLIENT!");
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