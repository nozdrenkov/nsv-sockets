#include "../common/engine.hpp"
using namespace std;
int main()
{
    cout << "SERVER!" << endl;
    engine_t engine("server", "127.0.0.1", 5001);
    while (true)
    {
        string s;
        if (engine.read(s))
        {
            cout << "client: " << s << endl << endl;
            engine.write("abalahaba");
        }
    }
    cerr << "READY!" << endl;
    return 0;
}