#include <SFML/Graphics.hpp>
#include <cassert>
#include <chrono>
#include <random>
#include <ctime>
#include <iostream>
#include <thread>
#include <vector>
#include <cmath>

/*constexpr int widthG = 700;
int day = 50;*/

enum class State { Susc, Inf, Rec };

class Board 
{
  private:

  int n_;
  std::vector<State> board_;

  public:

  Board(int n) : n_(n), board_(n * n) {}

  State operator()(int i, int j) const {
    return (i >= 0 && i < n_ && j >= 0 && j < n_) ? board_[i * n_ + j] : State::Rec;
  }

  State& operator()(int i, int j) {
    assert(i >= 0 && i < n_ && j >= 0 && j < n_);
    return board_[i * n_ + j];
  }

  int size() const { return n_; }
};

Board evolve(Board const& current, double const beta, double const gamma) {
  // add exception for value of beta, gamma
  if (beta > 1 || gamma > 1 || beta < 0 || gamma < 0) {
    throw std::runtime_error{"Coefficients Beta and Gamma must be between 0 and 1"};
  }

  int n = current.size();
  Board next(n);
  //seed for random generation
  std::mt19937 gen{std::random_device{}()};
  std::uniform_real_distribution<double> dist{0, 1};
  //cycle on matrix
  for (int i = 0; i != n; ++i) {
    for (int j = 0; j != n; ++j) {
      if (current(i, j) == State::Inf) {
        double prob1 = dist(gen);
        if (prob1 < gamma) {
          //recovery/death
          next(i, j) = State::Rec;
        } else {
          next(i, j) = State::Inf;
          //cycle on neighbors
          for (int l = i - 1; l <= i + 1; ++l) {
            for (int m = j - 1; m <= j + 1; ++m) {
              double prob2 = dist(gen);
              if (current(l, m) == State::Susc && prob2 < beta) {
                //infection!
                next(l, m) = State::Inf;
              }
            }
          }
        }
      } else {
        if (next(i,j)!=State::Inf){//non può disinfettarsi in automatico!
          next(i,j)=current(i,j);
        }
      }
    }
  }
  return next;
}

int count(State const& state,Board const& board) {
  int c=heightG-50;
  State x;
  if(pow(board.size(),2)==heightG-80) {
    for (int i = 0; i != board.size(); i++) {
      for (int j = 0; j != board.size(); j++) {
        x = board(i, j);
        if (x== state) {
          --c;
        }
      }
    }
  }
  if(pow(board.size(),2)<heightG-80) {
    for (int i = 0; i != board.size(); i++) {
      for (int j = 0; j != board.size(); j++) {
        x = board(i, j);
        if (x== state) {
          c=-floor((heightG-80)/pow(board.size(),2));
        }
      }
    }
  }
    if(pow(board.size(),2)>heightG-80) {
    for (int i = 0; i != board.size(); i++) {
      for (int j = 0; j != board.size(); j++) {
        x = board(i, j);
        if (x== state) {
          c=-(pow(board.size(),2)/(heightG-80));
        }
      }
    }
    c=floor(c);
  }
  return c;
}

int countR(Board const &board) {
  int r = 0;
  for (int l = 1; l <= board.size(); ++l) {
    for (int j = 1; j <= board.size(); ++j) {
      if (board(l, j) == State::Rec) {
        ++r;
      };
    };
  }
  return r;
}

struct Data{  
  int S_;
  int I_;
  int R_;
}

class Graph {
  sf::VertexArray curveS;
  sf::VertexArray curveR;
  sf::VertexArray curveI;
  static constexpr int heightG = 500;
  static constexpr int widthG = 700;
  static constexpr auto graph_title "Grafico andamento contagi";
  sf::RenderWindow graph_window
  int day
  
  public:
  Graph() : day{0}, curveS{sf::PrimitiveType::LineStrip, 0},
  curveR{sf::PrimitiveType::LineStrip, 0},
  curveI{sf::PrimitiveType::LineStrip, 0},
  graph_window{sf::VideoMode(widthG, heightG), 
  graph_title, 
  sf::Style::Close}{}



  /*void update(board){
    data_by_day today_data;
    today.S_=board.countS();
    today.R_=board.countR();
    today.I=board.size()-board.countS()-board.countR();
    graphdata.push_back(today_data);
  };*/
      /*sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      };
    }*/
  //questo lo facciamo andare solo una volta all'inizio, ma non penso sia saggio
  //metterlo nel costruttore... void e non ha argomenti

  
    /*sf::VertexArray curveS(sf::PrimitiveType::LineStrip, 0);
    sf::VertexArray curveR(sf::PrimitiveType::LineStrip, 0);
    sf::VertexArray curveI(sf::PrimitiveType::LineStrip, 0);*/
    // questa parte va fatta andare ogni ciclo, prende argomento board
  void update(Board board){
    sf::Vector2f coordS = sf::Vector2f(day, count(State::Susc, board));
    sf::Vector2f coordR = sf::Vector2f(day, count(State::Rec, board));
    sf::Vector2f coordI = sf::Vector2f(day, count(State::Inf, board));
    //penso: dovremmo fare count fuori e poi dentro associarlo a una grandezza step_size_y
    //step_size_x associato a day
    //i due step sarebbe bello fossero legati al width o height/numero totale di step
    //come faccio però a conoscerlo questo numero totale? o passo lunghezza simulazione o se implementeremo
    //while true dovrei tenere vettori coord per quando disegno alla fine. Le info su s, r, i nel tempo andrebbero allora in un vettore
    //di tre int direi 
    /*
    while(countR(board)!=pow(board.size(),2)) {
      day+=15;
      curveS.append(sf::Vertex(sf::Vector2f(day, count(State::Susc,board))));
      curveR.append(sf::Vertex(sf::Vector2f(day, count(State::Rec,board))));
      curveI.append(sf::Vertex(sf::Vector2f(day, count(State::Inf,board))));
*/
    curveS.append(sf::Vertex(coordS));
    curveR.append(sf::Vertex(coordR));
    curveI.append(sf::Vertex(coordI));
    day++;
  }
    
    //questo va solo alla fine
  void draw(){
    graph.clear(sf::Color::White);

    sf::RectangleShape Xaxis;
    Xaxis.setPosition(0, heightG - 50);
    Xaxis.setSize(sf::Vector2f(widthG, 3));
    Xaxis.setFillColor(sf::Color::Black);
    sf::RectangleShape Yaxis;
    Yaxis.setPosition(50, 0);
    Yaxis.setSize(sf::Vector2f(3, heightG));
    Yaxis.setFillColor(sf::Color::Black);

    graph.draw(Xaxis);
    graph.draw(Yaxis);
    graph.draw(curveS);
    graph.draw(curveR);
    graph.draw(curveI);

    graph.display();
  }
  return 0;
};


class Display{
  
  int m_board_side;
  sf::RenderWindow m_window;
  
  static constexpr int cell_size = 10;

  static int display_side(int board_side){
    
    return board_side * cell_size;

  }

  static constexpr auto s_window_title = "Virus simulation";

  public:
  
  Display(int board_side)
    : m_board_side{board_side}
    , m_window{
      sf::VideoMode(display_side(board_side), display_side(board_side)),
      s_window_title,
      sf::Style::Close}
    {
    }

   //while (window.isOpen()) {
    /* if (clock.getElapsedTime().asSeconds() >= fps) {
       redraw = true;
       clock.restart();
         };*/

    /*sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      };
    }*/

  void wait_key_pressed()
    {
      sf::Event event;

      m_window.waitEvent(event);
      while (event.type != sf::Event::KeyPressed) {
        m_window.waitEvent(event);
      }
    }

  int draw(Board const &board) {
 
    const sf::Vector2f cell_vector = sf::Vector2f(cell_size, cell_size);
  /*const int width = board.size();
  const int height = width;
  const int n_cells = width * height;
  bool redraw = true;
  const float fps = 1.0f;
  sf::RenderWindow window(sf::VideoMode(cell_size * width, cell_size * height),
                          "Covid");
  /*window.setFramerateLimit(fps);
  sf::Clock clock;*/
    sf::RectangleShape cell(cell_vector);
    cell.setOutlineThickness(1);
    cell.setOutlineColor(sf::Color::Black);

  
    m_window.clear(sf::Color::White);

    for (int i = 0; i != width; ++i) {
      for (int j = 0; j != height; ++j) {
        //sf::RectangleShape cell;
        cell.setPosition(i * cell_size, j * cell_size);
        //cell.setSize(cell_vector);
        //cell.setOutlineThickness(1);
        //cell.setOutlineColor(sf::Color::Black);
        if (board(i, j) == State::Susc) {
          cell.setFillColor(sf::Color::Blue);
        }
        if (board(i, j) == State::Inf) {
          cell.setFillColor(sf::Color::Red);
        }
        if (board(i, j) == State::Rec) {
          cell.setFillColor(sf::Color::Green);
        }
        m_window.draw(cell);
      }
    }

    m_window.display();
   //window.clear(sf::Color::White);
   //std::cout << "\033c";
  }
  return 0;
}

int main() {
  int dim = 50;
  Board board(dim);
  board(1, 1) = State::Inf;
  board(1, 2) = State::Inf;
  board(4, 5) = State::Inf;
  board(11, 16) = State::Inf;
  board(5, 19) = State::Inf;
  Display display(dim)
  Graph graph;
  display.draw(board);
  graph.update(board);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  for (int i = 0; i != 3; ++i) {
    board = evolve(board, 0.3, 0.4);
    display.draw(board);
    graph.update(board);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  }
  graph.draw();
}