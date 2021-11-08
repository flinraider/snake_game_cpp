#ifndef SNEAK_GAME_H
#define SNEAK_GAME_H
#include <iostream>  // TIME E RANGE     
#include <ncurses.h> // PARA AS FUNÇÕES 
#include <vector>    // PARA LOGICA DAS COORDENADAS
#include <unistd.h>

// Logica para armazenar as coordenadas
// struct
struct SneakType {
   int s_x, s_y; 
   SneakType(int, int);
   SneakType();
};
// struct nao precisa de destrutor

class SneakGame {
    protected:
        int m_maxwidth;  // tamanho da largura da tela
        int m_maxheight; // tamanho da altura da tela
        std::vector<SneakType> sneak;
        char m_sneak_char; // desenha a sneak
        SneakType v_food;
        char m_food_char;
        int m_delay;
        char m_direction;
        bool m_tail_stop;
        int m_score;
    public:
    //construtor
    SneakGame();               
    //destrutor
    ~SneakGame(); 

        // funções
        void m_insert_food();
        void movesneak();
        bool collide();
        void start();            
};
#endif
