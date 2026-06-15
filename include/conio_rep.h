#ifdef _WIN32
    #include <conio.h>
    #define GETCH() _getch()
	constexpr char enter = 13;
#else
    #include <termios.h>
    #include <unistd.h>
    #include <stdio.h>

    int getch_portable() {
        struct termios oldt, newt;
        int ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO); // Desactiva el buffer de línea y el eco
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }
    #define GETCH() getch_portable()
    constexpr char enter = 10;
#endif