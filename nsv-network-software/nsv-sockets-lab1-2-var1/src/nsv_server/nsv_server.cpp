#include "../common/engine.hpp"
#include <unordered_map>
#include <sstream>
#include <vector>
using namespace std;

/**
@brief Bank emulating class
*/
class bank_t
{
    // data <account, money>
    unordered_map<int, int> data;
    
    int new_id;

    engine_t *engine;

public:
    /**
    @brief Creates new bank
    */
    bank_t()
    {
        engine = new engine_t("server", "127.0.0.1", 5001);
        engine->connect();
        new_id = 1000;
    }

    /**
    @brief Starts main process
    */
    void start()
    {
        while (true)
        {
            string s;
            if (engine->read(s))
                process(s);
            else
            {
                cout << "Connection closed!" << endl;
                engine->connect();
            }
        }
    }

    /**
    @brief Switch how to process request req
    @param req - clients request
    */
    void process(const string &req)
    {
        istringstream is(req);
        string type; is >> type;

        if (type == "open")
        {
            open();
        }
        else if (type == "info")
        {
            int id;
            is >> id;
            info(id);
        }
        else if (type == "close")
        {
            int id;
            is >> id;
            close(id);
        }
        else if (type == "add")
        {
            int id, cnt;
            is >> id >> cnt;
            add(id, cnt);
        }
        else if (type == "get")
        {
            int id, cnt;
            is >> id >> cnt;
            get(id, cnt);
        }
        else if (type == "mov")
        {
            int from, to, cnt;
            is >> from >> to >> cnt;
            mov(from, to, cnt);
        }
        else
            engine->write("Invalid request!");
    }

    /**
    @brief Opens new account and gets new id for user
    */
    void open()
    {
        int id = new_id++;
        data[id] = 0;
        ostringstream os;
        os << "Opened new account!" << endl;
        os << "New id = " << id << endl;
        os << "Money = " << 0 << endl;
        engine->write(os.str());
    }

    /**
    @brief Gets main information about account id
    */
    void info(int id)
    {
        ostringstream os;
        os << "Information about account id = " << id << endl;
        if (data.count(id))
            os << "Money = " << data[id] << endl;
        else
            os << "Account is not opened!" << endl;

        engine->write(os.str());
    }

    /**
    @brief Tries to close account with id
    @param id - account id
    */
    void close(int id)
    {
        ostringstream os;
        os << "Close account id = " << id << endl;
        if (data.count(id))
            os << "Complete!" << endl;
        else
            os << "Account is not exist!" << endl;
        
        engine->write(os.str());
    }

    /**
    @brief Deposits money to account id
    @param id - account id
    @param cnt - amount of money
    */
    void add(int id, int cnt)
    {
        ostringstream os;
        os << "Add " << cnt << " $ to account id = " << id << endl;
        if (data.count(id))
        {
            os << "Before: " << data[id] << endl;
            data[id] += cnt;
            os << "After: "  << data[id] << endl;
        }
        else
            os << "Account is not exist!" << endl;

        engine->write(os.str());
    }

    /**
    @brief Tries to take money from account id
    @param id - account id
    @param cnt - amount of money
    */
    void get(int id, int cnt)
    {
        ostringstream os;
        os << "Get " << cnt << " $ from account id = " << id << endl;
        if (data.count(id))
        {
            if (data[id] >= cnt)
            {
                os << "Before: " << data[id] << endl;
                data[id] -= cnt;
                os << "After: "  << data[id] << endl;
            }
            else
                os << "Insufficient funds!" << endl;
        }
        else
            os << "Account is not exist!" << endl;

        engine->write(os.str());
    }

    /**
    @brief Tries to move money from account src to account dst
    @param from - source account
    @param to - destination account
    @param cnt - amount of money
    */
    void mov(int from, int to, int cnt)
    {
        ostringstream os;
        os << "Mov " << cnt << " $ from account id = " << from << " to account id = " << to << endl;
        if (!data.count(from))
            os << "Account id = " << from << " is not exist!" << endl;
        else if (!data.count(to))
            os << "Account id = " << to   << " is not exist!" << endl;
        else
        {
            if (data[from] >= cnt)
            {
                os << endl;
                os << "     \t from\tto" << endl;
                os << "Before:\t" << data[from] << "\t" << data[to] << endl;
                data[from] -= cnt;
                data[to]   += cnt;
                os << "After:\t"  << data[from] << "\t" << data[to]  << endl;
            }
            else
                os << "Insufficient funds!" << endl;
        }

        engine->write(os.str());
    }
};

int main()
{
    cout << "SERVER-BANK-TERMINAL" << endl;

    bank_t bank;
    bank.start();

    cerr << "Good bye!" << endl;
    return 0;
}