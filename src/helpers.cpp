#include "helpers.hpp"
#include "sysconfig.hpp"

int         g_gameMode;
int         g_windowRows;
int         g_windowCols;

inline std::vector<std::string> split(char *phrase, std::string delimiter)
{
    std::vector<std::string> list;
    std::string s = std::string(phrase);
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos)
    {
        token = s.substr(0, pos);
        list.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    list.push_back(s);
    return list;
}

void        getArgs(int ac, char **av)
{
    int opt;
    std::vector<std::string> windowSize;

    while ((opt = getopt(ac, av, "s:m:")) != -1)
    {
        switch (opt)
        {
            case 'm':
                g_gameMode = atoi(optarg);
                if (g_gameMode > 3 || g_gameMode < 1)
                {
                    std::cerr << "Game mode must be between 1 ~ 3" << std::endl;
                    ::exit(EXIT_SUCCESS);
                }
                std::cout << "m: " << std::to_string(g_gameMode) << std::endl;
                break;
            case 's':
                windowSize = split(optarg, ":");
                if (windowSize.size() != 2)
                {
                    std::cerr << "Usage: -s row:col" << std::endl;
                    ::exit(EXIT_SUCCESS);
                }
                try {
                    g_windowRows = std::stoi(windowSize[0]);
                    g_windowCols = std::stoi(windowSize[1]);
                } catch(std::exception &e) {
                    std::cout << e.what() << std::endl;
                    ::exit(EXIT_SUCCESS);
                }
                if (g_windowRows > WINDOW_MAX_HEIGHT || WINDOW_MIN_HEIGHT < 0
                || g_windowCols > WINDOW_MAX_WIDTH || g_windowCols < WINDOW_MIN_WIDTH)
                {
                    std::cerr << "Row number must be between " << WINDOW_MIN_HEIGHT;
                    std::cerr << " ~ " << WINDOW_MAX_HEIGHT << ". " << std::endl;
                    std::cerr << "Col number must be between " << WINDOW_MIN_WIDTH;
                    std::cerr << " ~ " << WINDOW_MAX_WIDTH << ". " << std::endl;
                    ::exit(EXIT_SUCCESS);
                }
                std::cout << "s: " << std::to_string(g_windowRows)
                     << " " << std::to_string(g_windowCols) <<  std::endl;
                break ;
            default:
               std::cerr << "Unknown arguments" << std::endl;
               ::exit(EXIT_SUCCESS);
        }
    }
}
