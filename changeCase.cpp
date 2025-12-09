// Windows batch has no simple way to convert character case.
// Here is a case converter program. 
// Yes, its stupid. but I preferred this to writing 100 lines of windows batch
#include <io.h>
#include <fcntl.h>
#include <string>
#include <sstream>

static void writeOut(char c, bool toLower)
{
    if (isalpha(c))
    {
        if (toLower)
            c = tolower(c);
        else
            c = toupper(c);
    }
    _write(_fileno(stdout), &c, 1);
}

int main(int argc, char **argv)
{
    bool toLower = true;
    std::string toConvert;
    for (int i = 1; i < argc; i++)
    {
        bool ok = false;
        if (_stricmp(argv[i], "-U") == 0)
        {
            toLower = false;
            ok = true;
        }
        else if (_stricmp(argv[i], "-l") == 0)
        {
            toLower = true;
            ok = true;
        }
        else if (_stricmp(argv[i], "-s") == 0)
        {
            i += 1;
            if (i < argc)
            {
                toConvert = argv[i];
                if (!toConvert.empty())
                    ok = true;
            }
        }
        if (!ok)
        {
            std::stringstream ss;
            ss << "usage: changeCase [-U] [-l] [-s \"string-to-convert\"]" << " string-to-cvt" << std::endl;
            const std::string &s = ss.str();
            _write(_fileno(stderr), s.c_str(), s.size());
            return 1;
        }
    }
    _setmode(_fileno(stdout), _O_BINARY);

    if (toConvert.empty())
    {
        _setmode(_fileno(stdin), _O_BINARY);
        char c;
        while (_read(_fileno(stdin), &c, 1))
            writeOut(c, toLower);
    }
    else
    {
        for (auto c : toConvert)
            writeOut(c, toLower);
    }
    _close(_fileno(stdin));
    _close(_fileno(stdout));
    return 0;
}

