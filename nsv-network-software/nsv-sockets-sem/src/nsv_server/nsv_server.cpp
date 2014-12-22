#include "../common/engine.hpp"
using namespace std;
int main()
{
    cout << "SERVER!" << endl;
    engine_t engine("server", "127.0.0.1", 5001);
    engine.connect();
    while (true)
    {
        string s;
        if (engine.read(s))
        {
            cout << endl << "client: " << s << endl;
            engine.write("abalahaba");
        }
        else
        {
            cout << "Connection closed!" << endl;
            engine.connect();
        }
    }
    cerr << "READY!" << endl;
    return 0;
}