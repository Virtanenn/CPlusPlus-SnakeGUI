/* snake_gui
 * Description:
 *  This module implements game snake with graphics. Game asks user for
 * width, height and seed for the gameboard. Game requires user to set
 * the game with desired values (or default width and height
 *  of 7 and seed value of 1) before
 * starting the game. Movement is implemented with pushbuttons with
 * corresponding direction. The opposite direction is disabled when moving
 * to any direction. When game is over, a new game can be opened by pressing
 * RESTART or SET GAME. Game can also be paused by pressing the same button
 * as PLAY. When play is clicked, the play button is changed to a pause button.
 *
 *
 *
 * * Program author:
 * Name: Timo Virtanen
 * Notes about the program and it's implementation:
 *      Snake can be made more smooth if third QTimer were added
 *      and every part of snake will be given moveBy() command direction.
 *
 * */
#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <iostream>
#include <gameboard.hh>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QTimer>
#include <QPushButton>
#include <memory>

// Size of a square in map
const int SIZE = 75;
const int TIME = 500;
const int ADD_LEFT_MARGIN = 50;
const int ADD_TOP_MARGIN = 100;
const int MAINWINDOW_WIDTH = 1230;
const int MAINWINDOW_HEIGHT = 900;

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private slots:
    // Implements left button click
    void left_button();

    // Implements up button click
    void up_button();

    // Implements right button click
    void right_button();

    // Implements down button click
    void down_button();

    // moving snake
    void move_snake();

    // creates new head
    void new_head();

    // Gets called ever 0.5 seconds. Keeps track of time
    void add_time();

    // Implements changes if snake eats
    void snake_eats();

    // Creates new food after old on is eaten
    void create_food();

    // disables button to the opposite of direction
    void disable_button();

    // Sets up new game with seed, width and height
    void set_new_game();

    // resets stats to their original values
    void reset_stats();

    // Implements quit button
    void quit_button();

    // starts game and pauses game when game is on-going
    void start_button();

    // Implements death
    void death();

private:
    Ui::MainWindow *ui;
    // Default values of gameboard
    int width_ = 7;
    int height_ = 7;
    int seed_ = 1;

    // GameBoard object called field_.
    // Graphics module relies solely on GameBoard.
    // Shared pointer is used here due to difficulties
    // of deleting the object without crashes.
    shared_ptr<GameBoard> field_ = nullptr;

    // for continuous moving
    QTimer timer_;
    bool timer_on = false;

    // For clock
    QTimer clock_;
    double time_ = 0;


    // screen_ is Qgraphics scene where snake and food is drawn in
    QGraphicsScene* screen_;
    int directionX_ = 0;
    int directionY_ = 0;
    string direction_ = "";

    // deque of ellipses. Creates a snake
    deque<QGraphicsEllipseItem*> snake_ = {};


    int length_ = 1;
    int pts_ = 0;
    double speed_ = 500;

    QGraphicsEllipseItem* food_;

    bool ate_ = false;


};
#endif // MAINWINDOW_HH
