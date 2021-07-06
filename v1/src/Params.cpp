#include "Params.h"

#include <stdexcept>

Params Params::parseRaw(int argc, char **argv)
{
    Params params;

    int status = 0;
    for (int i = 1; i < argc; ++i)
    {
        std::string word = argv[i];

        if (status == 0)
        {
            params.m_inputFilename = word;
            status = 1;
        }
        else if (status == 1)
        {
            if (parseInt(word, &params.m_minsup) && params.m_minsup > 0)
            {
                status = 2;
            }
            else
            {
                break;
            }
        }
        else if (status == 2)
        {
            if (word == "-f")
            {
                params.m_logEnabled = true;
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    if (status == 2)
    {
        params.m_good = true;
    }
    return params;
}

std::string Params::getUsage(int argc, char **argv)
{
    return std::string(argv[0]) + " <filename> <minsup> [-f]";
}

bool Params::isGood() const
{
    return m_good;
}

const std::string &Params::getInputFilename() const
{
    return m_inputFilename;
}

int Params::getMinsup() const
{
    return m_minsup;
}

bool Params::isLogEnabled() const
{
    return m_logEnabled;
}

bool Params::parseInt(const std::string &s, int *i)
{
    try
    {
        *i = std::stoi(s);
    }
    catch (const std::invalid_argument &ex)
    {
        return false;
    }
    return true;
}
