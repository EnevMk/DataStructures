#include "CommandLine.hpp"

using namespace std;

CommandLine::CommandLine() {}

CommandLine::CommandLine(const std::string& cmd) {
    parseCommand(cmd);

    if (count() != 3) throw length_error("Invalid argument count");
}

inline string CommandLine::clearWhitespaces(const std::string& str) {
    string s = str;
    int cnt = 0;
    while (s[cnt] == ' ' || s[cnt] == '\n') {
        cnt++;
    }

    s.erase(s.begin(), s.begin() + cnt);

    cnt = s.size() - 1;

    while (s[cnt] == ' ' || s[cnt] == '\n') {
        cnt--;
    }

    s.erase(s.begin() + cnt + 1, s.end());

    return s;
}

void CommandLine::parseCommand(const std::string& cmd) {
    args.clear();
    int begin = 0, end = 0;

    while (end < cmd.size() - 1) {

        end = cmd.find_first_of(' ', begin);
        if (end == string::npos) end = cmd.size();

        string argument = cmd.substr(begin, end - begin);
        this->args.push_back(clearWhitespaces(argument));

        begin = end + 1;
    }
}

const std::string& CommandLine::operator[](size_t index) const {
    return this->args[index];
}

int CommandLine::count() const {
    return this->args.size();
}