#include <iostream>
#include "../common/engine.hpp"
using namespace std;
int main()
{
    cout << "CLIENT!" << endl;
    engine_t engine("client", "127.0.0.1", 5001);
    engine.connect();
    cout << endl;
    while (true)
    {
        string query, ans;
        getline(cin, query);
        engine.write(query);
        engine.read(ans);
        cout << "server: " << ans << endl << endl;
    }
    cout << "BYE!" << endl;
    return 0;
}