#include "sneak.hh"

SneakType::SneakType(int s_col, int s_row) {
    s_x = s_col;
    s_y = s_row;
}

SneakType::SneakType(){
    s_x = 0;
    s_y = 0;
}
 
SneakGame::SneakGame(){     // construtor da classe
    initscr();
    nodelay( stdscr, true); // para a sneak não parar 
    keypad(stdscr, true);   // key...
    noecho();               // para nao exibir teclas na tela
    curs_set( 0 );          // desliga cursos do teclado

     m_maxwidth = getmaxx( stdscr );
     m_maxheight = getmaxy( stdscr );

    m_sneak_char = 'O';
    m_food_char = '*';
    srand( time( NULL ) ); // ao inicar o jogo posição da comida e randomica
    m_insert_food();

    m_delay = 100000;
    m_direction = 'L';
    m_tail_stop = false;
    m_score = 0; 

    for(int i=0; i < m_maxwidth -1; i++) {
        move(0, i);
        if(i == 0) {
            addch('+');
        }else if( i == m_maxwidth -2 ) {
            addch('+');
        }else {
            addch('-');
        }
    }    

    for(int i=1; i < m_maxheight -1; i++) {
        move(i, 0);
        if(i == m_maxheight -2 ) {
            addch('+');
        }else {
            addch('|');
        }
    }

    for(int i= 1; i < m_maxwidth -1; i++) {
        move(m_maxwidth - 2, i );
        if( i == 0 ) {
            addch('+');
        }else if( i == m_maxwidth - 2 ) {
            addch('+');
        }else {
            addch('-');
        }
    } 

    for(int i=1; i < m_maxheight -1; i++) {
        move(i, m_maxwidth - 2);
        if(i == (m_maxheight -2) ) {
            addch('+');
        }else if ( i == 0 ){
            addch('+');
        }else{
            addch('|');
        }
    }
    //desenha o tamanho inicial da sneak
    for( int i=0; i < 5; i++ ) {
        sneak.push_back( SneakType( 40 + i, 10 ));
    }

    //logica para posicionar
    for( int i=0; i < sneak.size(); i++ ) {
        move( sneak[i].s_y, sneak[i].s_x );
            addch( m_sneak_char );
    }
    //exibe pontiação inicial
    move(m_maxheight -1, 0);
    printw("%d", m_score);
    move(v_food.s_x, v_food.s_y);
    addch(m_food_char);
    refresh();
    }               

SneakGame::~SneakGame(){   // destrutor da classe
    nodelay( stdscr, false );
    getch();
    endwin();
} 
// função
void SneakGame::m_insert_food(){
    while( true ) {
        int tmpx = rand() % m_maxwidth + 1;
        int tmpy = rand() % m_maxheight + 1;
        for( int i = 0; i < sneak.size(); i++) {
            if(sneak[i].s_x == tmpx && sneak[i].s_y == tmpy) {
                continue;
            }
        }           
        if(tmpx >= m_maxwidth -2 || tmpy >= m_maxheight -3) {
            continue;
        }
        v_food.s_x = tmpx;
        v_food.s_y = tmpy;
        break;    
        move(v_food.s_x, v_food.s_y);
        addch(m_food_char);
        refresh();
    } 
}// fim m_insert_food

void SneakGame::movesneak(){
    int tmp = getch();
    switch ( tmp ) {
    case KEY_LEFT:
        if(m_direction != 'R') {
            m_direction = 'L';
        }
        break;
        case KEY_UP:
        if(m_direction != 'D') {
            m_direction = 'U';
        }
        break;
        case KEY_DOWN:
        if( m_direction != 'U') {
            m_direction = 'D';
        }
        break;
        case KEY_RIGHT:
        if(m_direction != 'L') {
            m_direction = 'R';
        }
        break;
        case 'q':
         m_direction = 'Q';
         break;
    }
    if( !m_tail_stop ) {
        move(sneak[sneak.size() -1].s_y, sneak[sneak.size() -1].s_x ); 
        printw(" ");
        refresh();
        sneak.pop_back();   
    }
    if( m_direction == 'L'){
        sneak.insert( sneak.begin(), SneakType( sneak[0].s_x - 1, sneak[0].s_y ) );
    }
    if( m_direction == 'R'){
        sneak.insert( sneak.begin(), SneakType( sneak[0].s_x + 1, sneak[0].s_y ) );
    }
    if( m_direction == 'U'){
        sneak.insert( sneak.begin(), SneakType( sneak[0].s_x, sneak[0].s_y - 1 ) );
    }
    if( m_direction == 'D'){
        sneak.insert( sneak.begin(), SneakType( sneak[0].s_x, sneak[0].s_y + 1 ) );
    }
    move( sneak[0].s_y, sneak[0].s_x );
    addch( m_sneak_char );
    refresh();  
}

bool SneakGame::collide(){
    // colisão com as bordas 
    if(sneak[0].s_x == 0 || sneak[0].s_x == m_maxwidth - 1 || 
       sneak[0].s_y == 0 || sneak[0].s_y == m_maxheight - 2 ) {
           return true;
       }
       //colisão com a cauda
    for(int i = 2; i < sneak.size(); i++ ) {
        if(sneak[0].s_x == sneak[i].s_x && sneak[0].s_y == sneak[i].s_y) {
            return true;
        }
    }
    //colisao de pontuação
    if( sneak[0].s_x == v_food.s_x && sneak[0].s_y == v_food.s_y ) {
        m_tail_stop = true;
        m_insert_food();
        m_score += 10;
        move( m_maxheight - 1, 0 );
        printw("%d", m_score);
        if( (m_score % 50 ) == 0 ){
            m_delay -= 10000;
        }
    }else {
        m_tail_stop = false;                
    }   
    return false;
}

void SneakGame::start(){
    while( true ){
        if(collide()) {
        move( m_maxheight / 2, (m_maxwidth / 2) -4 );
        printw("GAME OVER");
        break;
        }
    movesneak();
    if( m_direction == 'Q'){
        break;
    }
    usleep( m_delay ); 
    }
}            

