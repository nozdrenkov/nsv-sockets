#include "../common/engine.hpp"
#include <sstream>
using namespace std;

/**
@brief Simple file manager class
*/
class fm_t
{
    engine_t *engine;

public:
    /**
    @brief Initialisation
    */
    fm_t()
    {
        engine = new engine_t("server", "127.0.0.1", 5001);
        engine->connect();
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

        if (type == "show")
        {
            string dir; is >> dir;
            show(dir);
        }
        else if (type == "rename")
        {
            string file, name;
            is >> file >> name;
            rename(file, name);
        }
        else if (type == "copy")
        {
            string from, to;
            is >> from >> to;
            copy(from, to);
        }
        else if (type == "remove")
        {
            string file; is >> file;
            remove(file);
        }
        else if (type == "exec")
        {
            string file; is >> file;
            exec(file);
        }
        else if (type == "reboot")
        {
            reboot();
        }
        else
            engine->write("Invalid request!");
    }

    /**
    @brief Shows files in directory dir
    */
    void show(const string &dir)
    {
        ostringstream os;
        os << "blabla = " << 0 << endl;
        engine->write(os.str());
    }

    /**
    @brief Renames file
    */
    void rename(const string &file, const string &new_name)
    {
    }

    /**
    @brief Copies file
    */
    void copy(const string &from, const string &to)
    {
    }

    /**
    @brief Remove file
    */
    void remove(const string &file)
    {
    }

    /**
    @brief Executes file
    */
    void exec(const string &file)
    {
    }

    /**
    @brief Reboots comp
    */
    void reboot()
    {
        engine->write("OK, let's reboot!");
        system("shutdown -r -t 0");
    }
};

int main()
{
    cout << "SERVER-FILE-MANAGER" << endl;

    fm_t manager;
    manager.start();

    cerr << "Good bye!" << endl;
    return 0;
}