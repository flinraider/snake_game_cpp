#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H
#include <iostream>  // TIME E RANGE     
#include <ncurses.h> // PARA AS FUNÇÕES 
#include <vector>    // PARA LOGICA DAS COORDENADAS
#include <unistd.h>
// Logica para armazenar as coordenadas
// struct
struct SnakeType {
   int s_x, s_y; 
   SnakeType(int, int);
   SnakeType();
};
// struct nao precisa de destrutor

class SnakeGame {
    protected:
        int m_maxwidth;  // tamanho da largura da tela
        int m_maxheight; // tamanho da altura da tela
        std::vector<SnakeType> snake;
        char m_snake_char; // desenha a sneak
        SnakeType v_food;
        char m_food_char;
        int m_delay;
        char m_direction;
        bool m_tail_stop;
        int m_score;
    public:
    //construtor
    SnakeGame();               
    //destrutor
    ~SnakeGame(); 
        // funções
       void m_insert_food();
       void movesnake();
       bool _collide();
       void start();            
};
#endif
