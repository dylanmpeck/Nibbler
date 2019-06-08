#include "helpers.hpp"

extern uint8_t     g_gameMode;
extern int         g_windowHeight;
extern int         g_windowWidth;

std::vector<std::string> split(char *phrase, std::string delimiter){
    std::vector<std::string> list;
    std::string s = std::string(phrase);
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
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

    while ((opt = getopt(ac, av, "s:m:") != -1))
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
                break;
            case 's':
                windowSize = split(optarg, ":");
                if (windowSize.size() != 2)
                {
                    std::cerr << "Usage: -s height:width" << std::endl;
                    ::exit(EXIT_SUCCESS);
                }
                g_windowHeight = std::stoi(windowSize[0]);
                g_windowWidth = std::stoi(windowSize[1]);
                if (g_windowHeight > WINDOW_MAX_HEIGHT || WINDOW_MIN_HEIGHT < 0
                || g_windowWidth > WINDOW_MAX_WIDTH || g_windowWidth < WINDOW_MIN_WIDTH)
                {
                    std::cerr << "Height of the window must be between " << WINDOW_MIN_HEIGHT;
                    std::cerr << " ~ " << WINDOW_MAX_HEIGHT << ". " << std::endl;
                    std::cerr << "Weight of the window must be between " << WINDOW_MIN_WIDTH;
                    std::cerr << " ~ " << WINDOW_MAX_WIDTH << ". " << std::endl;
                    ::exit(EXIT_SUCCESS);
                }
                break ;
            default:
               std::cerr << "Unknown arguments" << std::endl;
               ::exit(EXIT_SUCCESS);
        }
    }
}