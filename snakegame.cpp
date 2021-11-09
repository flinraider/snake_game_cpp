#include "snake.hh"

// Lógica para armazenar as coordenadas

SnakeType::SnakeType( int s_col, int s_row ){
  s_x = s_col;
  s_y = s_row;
}

SnakeType::SnakeType(){
  s_x = 0;
  s_y = 0;
}

SnakeGame::SnakeGame(){
  initscr();
  nodelay( stdscr, true ); // para a snake não parar
  keypad( stdscr, true ); // KEY_..., 
  noecho(); // para não exibir teclas na tela
  curs_set( 0 ); // desliga o curso

  m_maxwidth = getmaxx( stdscr ) / 2;
  m_maxheight = getmaxy( stdscr ) / 2;

  m_snake_char = 'O'; // desenha a snake

  m_food_char = '*';
  srand( time(NULL) ); // para toda vez que iniciar o game a posição da comida ser randômica/aleatória
  m_insert_food();

  m_delay = 100000;
  m_direction = 'L';
  m_tail_stop = false;

  m_score = 0;

  // DESENHA o ESPAÇO do game
  for( int i = 0; i < m_maxwidth - 1; ++i ){
    move( 0, i );
    if( i == 0 ){
      addch('+');
    }else if( i == ( m_maxwidth - 2 ) ){
      addch('+');
    }else{
      addch('-');
    }
  }

  for( int i = 1; i < m_maxheight - 1; ++i ){
    move( i, 0 );
    if( i == ( m_maxheight - 2 ) ){
      addch('+');
    }else{
      addch('|');
    }
  }

  for( int i = 0; i < m_maxwidth - 1; ++i ){
    move( m_maxheight - 2 , i );
    if( i == 0 ){
      addch('+');
    }else if( i == ( m_maxwidth - 2 ) ){
      addch('+');
    }else{
      addch('-');
    }
  }

  for( int i = 1; i < m_maxheight - 1; ++i ){
    move( i, m_maxwidth - 2 );
    if( i == ( m_maxheight - 2 ) ){
      addch('+');
    }else if ( i == 0 ){
      addch('+');
    }else{
      addch('|');
    }
  }

  // desenha o tamanho inicial da snake
  for( int i = 0; i < 5; ++i ){
    snake.push_back( SnakeType( 40 + i, 10 ) );
  }

  // Lógica para posicionar
  for( size_t i = 0; i < snake.size(); ++i ){
    move( snake[i].s_y, snake[i].s_x );
    addch( m_snake_char );
  }

  // mostra o score já no início
  move( m_maxheight - 1, 0 );
  printw("%d", m_score);

  move( v_food.s_y, v_food.s_x );
  addch( m_food_char );
  refresh();

}

SnakeGame::~SnakeGame(){
  nodelay( stdscr , false );
  getch();
  endwin();
}

void SnakeGame::m_insert_food(){
  while( true ){
    int tmpx = rand() % m_maxwidth + 1;
    int tmpy = rand() % m_maxheight + 1;

    for( size_t i = 0; i < snake.size(); ++i ){
      if( snake[i].s_x == tmpx && snake[i].s_y == tmpy ){
        continue;
      }
    }

    if( tmpx >= m_maxwidth - 2 || tmpy >= m_maxheight - 3 ){
      continue;
    }

    v_food.s_x = tmpx;
    v_food.s_y = tmpy;
    break;
  }
  move( v_food.s_y, v_food.s_x );
  addch( m_food_char );
  refresh();
}

void SnakeGame::movesnake(){
  int tmp = getch();
  switch( tmp ){
    case KEY_LEFT:
      if( m_direction != 'R' ){
        m_direction = 'L';
      }
      break;
    case KEY_UP:
      if( m_direction != 'D' ){
        m_direction = 'U';
      }
      break;
    case KEY_DOWN:
      if( m_direction != 'U' ){
        m_direction = 'D';
      }
      break;
    case KEY_RIGHT:
      if( m_direction != 'L' ){
        m_direction = 'R';
      }
      break;
    case 'q':
      m_direction = 'Q';
      break;
  }


  if( !m_tail_stop ){
    move( snake[ snake.size() - 1 ].s_y, snake[ snake.size() - 1 ].s_x );
    printw(" ");
    refresh();
    snake.pop_back();
  }

  if( m_direction == 'L' ){
    snake.insert( snake.begin(), SnakeType( snake[0].s_x - 1 , snake[0].s_y ) );
  }else if( m_direction == 'R' ){
    snake.insert( snake.begin(), SnakeType( snake[0].s_x + 1 , snake[0].s_y ) );
  }else if( m_direction == 'U' ){
    snake.insert( snake.begin(), SnakeType( snake[0].s_x , snake[0].s_y - 1 ) );
  }else if( m_direction == 'D' ){
    snake.insert( snake.begin(), SnakeType( snake[0].s_x , snake[0].s_y + 1 ) );
  }

  move( snake[0].s_y, snake[0].s_x );
  addch( m_snake_char );
  refresh();

}

bool SnakeGame::_collide(){

  // colisão com as bordas
  if( snake[0].s_x == 0 || snake[0].s_x == m_maxwidth - 1 || snake[0].s_y == 0 || snake[0].s_y == m_maxheight - 2 ){
    return true;
  }

  // colisão com a própria cauda
  for( size_t i = 2; i < snake.size() ; ++i ){
    if( snake[0].s_x == snake[i].s_x && snake[0].s_y == snake[i].s_y ){
      return true;
    }
  }

  // colisão de ponto
  if( snake[0].s_x == v_food.s_x && snake[0].s_y == v_food.s_y ){
    m_tail_stop = true;
    m_insert_food();
    m_score += 10;
    move( m_maxheight - 1, 0 );
    printw("%d", m_score);
    if( ( m_score % 50 ) == 0 ){
      m_delay -= 10000;
    }


  }else{
    m_tail_stop = false;
  }

  return false;

}

void SnakeGame::start(){
  while( true ){
    if( _collide() ){
      move( m_maxheight / 2, ( m_maxwidth / 2 ) - 4 );
      printw("GAME OVER");
      break;
    }

    movesnake();

    if( m_direction == 'Q' ){
      break;
    }

    usleep( m_delay );
  }

}
