#pragma once

#include <string>

class Params
{
public:
    static Params parseRaw(int argc, char **argv);

    static std::string getUsage(int argc, char **argv);

    bool isGood() const;

    const std::string &getInputFilename() const;

    int getMinsup() const;

    bool isLogEnabled() const;

private:
    bool m_good = false;

    std::string m_inputFilename;

    int m_minsup;

    bool m_logEnabled = false;
};
