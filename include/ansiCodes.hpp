#ifndef ANSICODES_HPP
#define ANSICODES_HPP

#define RESET       "\033[0m"
#define BOLD        "\033[1m"

#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"

inline void clearTerminalLines(const int lines = 1) {
    for (int i = 0; i < lines; ++i) {
        std::cout << "\33[2K\r"; // Clear the current line
        if (i < lines - 1)
            std::cout << "\033[A"; // Move the cursor up one line
    }
}

#endif //ANSICODES_HPP
