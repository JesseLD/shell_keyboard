#pragma once
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

class Keyboard {
public:
    // Construtor: Configura o terminal quando a classe é criada
    Keyboard() {
        tcgetattr(STDIN_FILENO, &originalTerm); // Salva config original
        struct termios newTerm = originalTerm;
        newTerm.c_lflag &= ~(ICANON | ECHO); // Desliga buffer e eco na tela
        tcsetattr(STDIN_FILENO, TCSANOW, &newTerm);
        
        // Configura leitura não bloqueante
        int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    }

    // Destrutor: Restaura o terminal quando o jogo fecha (IMPORTANTE!)
    ~Keyboard() {
        tcsetattr(STDIN_FILENO, TCSANOW, &originalTerm);
    }

    // Verifica se tem tecla pressionada
    bool kbhit() {
        char ch;
        int nread = read(STDIN_FILENO, &ch, 1);
        if (nread > 0) {
            // lastChar = ch;
            this->lastChar = ch;
            return true;
        }
        return false;
    }

    // Retorna a última tecla
    char getChar() {
        return this->lastChar;
    }

    void resetChar()
    {
      lastChar = 0;
    }

private:
    struct termios originalTerm;
    char lastChar = 0;
};
