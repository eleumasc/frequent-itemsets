#include "Params.h"

#include <stdexcept>

Params Params::parseRaw(int argc, char **argv)
{
    Params params;

    int status = 0;
    int *idst = &params.m_minsup;
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
            try
            {
                int i = std::stoi(word);
                if (i > 0)
                {
                    *idst = i;
                }
                else
                {
                    break;
                }
            }
            catch (const std::invalid_argument &ex)
            {
                break;
            }
            status = 2;
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
    return std::string(argv[0]) + " <filename> <minsup> [-l]";
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
