#include <functional>
#include <map>
#include <string>
#include <set>
#include <iostream>

//
// supporting tools and software
//
// Validate and test your json commands
// https://jsonlint.com/

// RapidJSON : lots and lots of examples to help you use it properly
// https://github.com/Tencent/rapidjson
//

// std::function
// std::bind
// std::placeholders
// std::map
// std::make_pair

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/error.h"

using namespace rapidjson;
using namespace std;

bool g_done = false;

//
// TEST COMMANDS
//
auto help_command = R"(
{
    "command":"help",
    "payload": {
        "usage":"Enter json command in {\"command\":\"<command>\",\"payload\":<some payload>}"
    }
}
)";

auto exit_command = R"(
{
    "command":"exit",
    "payload": {
        "reason":"Exiting program on user request."
    }
}
)";

auto assert_command = R"(
{
    "command":"assert",
    "payload": {
        "reason":"Asserting program on user request."
    }
}
)";

auto hello_command = R"(
{
    "command":"hello",
    "payload": {
        "display_text":"Hello World!"
    }
}
)";

auto counter_command = R"(
{
    "command":"counter",
    "payload": {
        "set":10,
        "add":1,
        "subtract":1
    }
}
)";

auto name_command = R"(
{
    "command":"name",
    "payload": {
        "add":["Adam","Eve"],
        "remove":"Eve"
    }
}
)";

bool CheckFormat(Document& document, ParseResult& ok) {
        if (!ok) {
            cout << "\tJSON parse error: " << ok.Code() << " (" << ok.Offset() << ")" << endl;
            return false;
        }

        if(!document.IsObject()) {
            cout << "\troot element not object" << endl;
            return false;
        }

        if(!document.HasMember("command")) {
            cout << "\tmissing command element" << endl;
            return false;
        }

        if(!document.HasMember("payload")) {
            cout << "\tmissing payload element" << endl;
            return false;
        }

        if(!document["command"].IsString()) {
            cout << "\tcommand is not string" << endl;
            return false;
        }
        return true;
}

class Controller {
public:
    Controller()
    {
        Document document;
        ParseResult ok = document.Parse(help_command);

        if(!CheckFormat(document, ok))
            assert(0 && "invalid help");

        if(!document["payload"].HasMember("usage"))
            assert(0 && "invalid help - no usage");

        if(!document["payload"]["usage"].IsString())
            assert(0 && "invalid help - usage not string");

        m_help_string = document["payload"]["usage"].GetString();

        ok = document.Parse(hello_command);

        if(!CheckFormat(document, ok))
            assert(0 && "invalid hello");

        if(!document["payload"].HasMember("display_text"))
            assert(0 && "invalid hello - no display_text");

        if(!document["payload"]["display_text"].IsString())
            assert(0 && "invalid hello - display_text not string");

        m_hello_string = document["payload"]["display_text"].GetString();

        ok = document.Parse(counter_command);

        if(!CheckFormat(document, ok))
            assert(0 && "invalid counter");

        if(!document["payload"].HasMember("set"))
            assert(0 && "invalid counter - no set");

        if(!document["payload"]["set"].IsInt())
            assert(0 && "invalid counter - set not number");

        m_counter = document["payload"]["set"].GetInt();

        ok = document.Parse(name_command);

        if(!CheckFormat(document, ok))
            assert(0 && "invalid name");

        if(document["payload"].HasMember("add")) {
            if(document["payload"]["add"].IsArray()) {
                for (SizeType i = 0; i < document["payload"]["add"].Size(); i++) {
                    if(document["payload"]["add"][i].IsString())
                        m_name_set.insert(document["payload"]["add"][i].GetString());
                }
            } else if (document["payload"]["add"].IsString())
                m_name_set.insert(document["payload"]["add"].GetString());
        }

        if(document["payload"].HasMember("remove")) {
            if(document["payload"]["remove"].IsArray()) {
                for (SizeType i = 0; i < document["payload"]["remove"].Size(); i++) {
                    if(document["payload"]["remove"][i].IsString())
                        m_name_set.erase(document["payload"]["remove"][i].GetString());
                }
            } else if (document["payload"]["remove"].IsString())
                m_name_set.erase(document["payload"]["remove"].GetString());
        }
    }

    ~Controller()
    {

    }

    bool help(rapidjson::Value &payload)
    {
        cout << "Controller:: calling help command" << endl;
        cout << "\tusage: " << m_help_string << endl;

        return true;
    }

    bool exit(rapidjson::Value &payload)
    {
        cout << "Controller:: calling exit command" << endl;

        g_done = true;

        if(!payload.IsObject()) {
            cout << "\tinvalid command content (payload not object), but still exiting" << endl;
            return false;
        }
        if(!payload.HasMember("reason")) {
            cout << "\tinvalid command content (missing reason element), but still exiting" << endl;
            return false;
        }
        if(!payload["reason"].IsString()) {
            cout << "\tinvalid command content (reason is not string), but still exiting" << endl;
            return false;
        }

        cout << "\treason: " << payload["reason"].GetString() << endl;

        return true;
    }

    bool assert_(rapidjson::Value &payload)
    {
        cout << "Controller:: calling assert command" << endl;

        g_done = true;

        if(!payload.IsObject()) {
            cout << "\tinvalid command content (payload not object), but still asserting" << endl;
            return false;
        }
        if(!payload.HasMember("reason")) {
            cout << "\tinvalid command content (missing reason element), but still asserting" << endl;
            return false;
        }
        if(!payload["reason"].IsString()) {
            cout << "\tinvalid command content (reason is not string), but still asserting" << endl;
            return false;
        }

        cout << "\treason: " << payload["reason"].GetString() << endl;

        assert(0);

        return true;
    }

    bool hello(rapidjson::Value &payload)
    {
        cout << "Controller:: calling hello command" << endl;
        cout << "\ttext: " << m_hello_string << endl;

        return true;
    }

    bool counter(rapidjson::Value &payload)
    {
        cout << "Controller:: calling counter command" << endl;

        if(!payload.IsObject()) {
            cout << "\tinvalid command content (payload not object)" << endl;
            return false;
        }
        if(!payload.HasMember("set") && !payload.HasMember("add") && !payload.HasMember("subtract")) {
            cout << "\tinvalid command content (missing set/add/subtract element)" << endl;
            return false;
        }
        if(payload.HasMember("set")) {
            if(!payload["set"].IsInt()) {
                cout << "\tinvalid command content (set is not int)" << endl;
                return false;
            } else
                m_counter = payload["set"].GetInt();
        }

        if(payload.HasMember("add")) {
            if(!payload["add"].IsInt()) {
                cout << "\tinvalid command content (add is not int)" << endl;
                return false;
            } else
                m_counter += payload["add"].GetInt();
        }

        if(payload.HasMember("subtract")) {
            if(!payload["subtract"].IsInt()) {
                cout << "\tinvalid command content (subtract is not int)" << endl;
                return false;
            } else
                m_counter -= payload["subtract"].GetInt();
        }

        cout << "\tcounter: " << m_counter << endl;


        return true;
    }

    bool name(rapidjson::Value &payload)
    {
        cout << "Controller:: calling name command" << endl;

        if(!payload.IsObject()) {
            cout << "\tinvalid command content (payload not object)" << endl;
            return false;
        }
        if(!payload.HasMember("add") && !payload.HasMember("remove")) {
            cout << "\tinvalid command content (missing add/remove element)" << endl;
            return false;
        }

        if(payload.HasMember("add")) {
            if(payload["add"].IsArray()) {
                for (SizeType i = 0; i < payload["add"].Size(); i++) {
                    if(payload["add"][i].IsString())
                        m_name_set.insert(payload["add"][i].GetString());
                }
            } else if (payload["add"].IsString())
                m_name_set.insert(payload["add"].GetString());
        }

        if(payload.HasMember("remove")) {
            if(payload["remove"].IsArray()) {
                for (SizeType i = 0; i < payload["remove"].Size(); i++) {
                    if(payload["remove"][i].IsString())
                        m_name_set.erase(payload["remove"][i].GetString());
                }
            } else if (payload["remove"].IsString())
                m_name_set.erase(payload["remove"].GetString());
        }

        cout << "\tnames: " << m_name_set.size() << ": " ;
        for(auto it: m_name_set)
            cout << it << " ";

        cout << endl;

        return true;
    }

private:
    string m_help_string;
    string m_hello_string;
    int m_counter;
    set<string> m_name_set;
};

// Bonus Question: why did I type cast this?
// Answer: because it will not work without it later during assignment (local function inside class)

typedef std::function<bool(rapidjson::Value &)> CommandHandler;
//typedef bool (*CommandHandler)(rapidjson::Value &);

class CommandDispatcher {
public:
    CommandDispatcher() : m_help_command(nullptr)
    {

    }

    virtual ~CommandDispatcher()
    {
        // question why is it virtual? Is it needed in this case?
        // Answer: should not be needed in this case to be virtual, only when making derrived classes
    }

    bool addCommandHandler(std::string command, CommandHandler handler)
    {
        cout << "CommandDispatcher: addCommandHandler: " << command << std::endl;

        if(command == "unknown")
            return false;

        if(command == "help")
            m_help_command = handler;

        auto res = command_handlers_.insert(make_pair(command, handler));

        return res.second;
    }

    bool dispatchCommand(std::string command_json)
    {
        cout << "COMMAND: " << command_json << endl;

        Document document;
        ParseResult ok = document.Parse(command_json.c_str());
        if(!CheckFormat(document, ok))
            return false;

        auto it = command_handlers_.find(document["command"].GetString());
        if(it != command_handlers_.end()) {
            return it->second(document["payload"]);
        }

        cout << "\tunknown command" << endl;
        return false;
    }

private:
    CommandHandler m_help_command;
    std::map<std::string, CommandHandler> command_handlers_;

    // Question: why delete these?
    // Answer: because we are using pointers to local functions that should not be copied

    CommandDispatcher (const CommandDispatcher&) = delete;
    CommandDispatcher& operator= (const CommandDispatcher&) = delete;

};

std::string GetCommand(const char* command) {
    Document document;
    ParseResult ok = document.Parse(command);

    if(!CheckFormat(document, ok))
        return "unknown";

    return document["command"].GetString();
}

int main()
{
    std::cout << "COMMAND DISPATCHER: STARTED" << std::endl;

    CommandDispatcher command_dispatcher;
    Controller controller;                 // controller class of functions to "dispatch" from Command Dispatcher

    CommandHandler cmd_help = std::bind(&Controller::help, &controller, std::placeholders::_1);
    CommandHandler cmd_exit = std::bind(&Controller::exit, &controller, std::placeholders::_1);
    CommandHandler cmd_assert = std::bind(&Controller::assert_, &controller, std::placeholders::_1);
    CommandHandler cmd_hello = std::bind(&Controller::hello, &controller, std::placeholders::_1);
    CommandHandler cmd_counter = std::bind(&Controller::counter, &controller, std::placeholders::_1);
    CommandHandler cmd_name = std::bind(&Controller::name, &controller, std::placeholders::_1);

    command_dispatcher.addCommandHandler(GetCommand(help_command), cmd_help);
    command_dispatcher.addCommandHandler(GetCommand(exit_command), cmd_exit);
    command_dispatcher.addCommandHandler(GetCommand(assert_command), cmd_assert);
    command_dispatcher.addCommandHandler(GetCommand(hello_command), cmd_hello);
    command_dispatcher.addCommandHandler(GetCommand(counter_command), cmd_counter);
    command_dispatcher.addCommandHandler(GetCommand(name_command), cmd_name);

    // command line interface
    string command;
    cout << "example command: {\"command\":\"exit\", \"payload\":{\"reason\":\"User requested exit.\"}}\n";
    while( ! g_done ) {
        cout << "\tenter command : ";
        getline(cin, command);
        command_dispatcher.dispatchCommand(command);
    }

    std::cout << "COMMAND DISPATCHER: ENDED" << std::endl;
    return 0;
}
