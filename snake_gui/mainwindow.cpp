/* snake_gui.cpp
 * Description:
 *  Parts of snake are drawn with ellipses and all those ellipses are saved
 *  in Deque called snake_. The idea behind drawing the snake is this:
 *  Snakes head doesn't actually move but a new ellipse is created in the
 *  direction where the snake is going. The tail ellipse is removed at the
 *  same time as new head is created if the snake doesn't eat.
 *
 * * Program author:
 * Name: Timo Virtanen
 * Student number: 150769440
 * UserID: kktivi
 * E-Mail: timo.virtanen@tuni.fi
 *
 *
 * */

#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <iostream>
#include <QDebug>
#include <QTimer>
#include <QKeyEvent>
#include <QGraphicsEllipseItem>
#include <QPointF>
#include <QApplication>
#include <QProcess>
#include <iomanip>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // Connects buttons to corresponding class functions
    ui->setupUi(this);
    connect(&timer_, &QTimer::timeout, this,
            &MainWindow::move_snake);

    connect(&clock_, &QTimer::timeout, this, &MainWindow::add_time);

    connect(ui->RightButton, &QPushButton::clicked, this,
            &MainWindow::right_button);

    connect(ui->LeftButton, &QPushButton::clicked, this,
            &MainWindow::left_button);

    connect(ui->DownButton, &QPushButton::clicked, this,
            &MainWindow::down_button);

    connect(ui->UpButton, &QPushButton::clicked, this,
            &MainWindow::up_button);

    connect(ui->setGameButton, &QPushButton::clicked, this,
            &MainWindow::set_new_game);

    connect(ui->quitButton, &QPushButton::clicked, this,
            &MainWindow::quit_button);

    connect(ui->StartButton, &QPushButton::clicked, this,
            &MainWindow::start_button);

    // Changes background to blue
    ui->GameText->setStyleSheet(QStringLiteral
                                ("QLabel{color: rgb(153, 0, 0);}"));
    this->setStyleSheet("background-color: rgb(0, 151, 102);");

    this->setFixedSize(MAINWINDOW_WIDTH,MAINWINDOW_HEIGHT);
    // Disables start button until game is set.
    ui->StartButton->setEnabled(false);
}

// Destructor
MainWindow::~MainWindow()
{
    delete ui;
}

// Implements Left -button
void MainWindow::left_button()
{
    if (directionX_ != SIZE){
        directionX_ = -SIZE;
        directionY_ = 0;
        direction_= "a";
    }

}

// Implements Up -button
void MainWindow::up_button()
{
    if (directionY_ != SIZE){
        directionX_ =  0;
        directionY_ =-SIZE;
        direction_= "w";
    }
}


// Implements Right -button
void MainWindow::right_button()
{
    if (directionX_ != -SIZE){
        directionX_ = SIZE;
        directionY_ = 0;
        direction_= "d";
    }
}
// Implements Down -button
void MainWindow::down_button()
{
    if (directionY_ != -SIZE){
        directionX_ = 0;
        directionY_ = SIZE;
        direction_= "s";
    }
}

// Moves snake
void MainWindow::move_snake()
{
    // Snake goes to right if no other direction is given.
    if (directionX_ == 0 && directionY_ == 0){
        directionX_ = SIZE;
        direction_ = "d";
    }
    // Moving and printing snake. Game can be viewed
    // in ASCII as well.
    field_->moveSnake(direction_);
    field_->print();

    // Disables button that points away from the current direction
    disable_button();

    // If game isn't over
    if (not field_->gameOver()){

        // If graphical snake is smaller than the actual one
        // Snake has aten something.
        if (snake_.size() == field_->return_snake().size()-1){
            ate_ = true;
        }

        // Creating new head
        new_head();


        // Removes tail if snake didnt eat anything.
        if (not ate_){
            delete snake_.at(0);
            snake_.pop_front();

            // Change snakes tail color
            if (snake_.size() > 1){
                snake_.at(0)->setBrush(QColorConstants::Svg::lightgreen);
            }

        }
        else{
            snake_eats();
            ate_ = false;
        }
    }
    // Checks if game is over by death or victory.
    else{
        // Victory
        if (not field_->death()){
            // creates new head in the place of the last food
            new_head();

            delete food_;

            // Updating stats after eating the last food.
            ui->GameText->setText("SNEK WON");
            length_ += 1;
            pts_ = (length_*length_*2)-time_;
            QString points = QString::number(pts_);
            QString length = QString::number(length_);
            ui->LengthLabel->setText("LENGTH: " + length);

            if (pts_ >= 0){
                ui->scoreLabel->setText("SCORE: " + points);
            }
            // Changes color of the background and informs player of victory
            ui->GameText->setStyleSheet
                    (QStringLiteral("QLabel{color: rgb(120, 120, 255);}"));

            this->setStyleSheet("background-color: green;");
        }

        else{
            death();
        }
        // stopping timers
        timer_.stop();
        clock_.stop();
    }
    // Changes snakes body color
    if (snake_.size() > 2){
        snake_.at(snake_.size()-2)->setBrush(QColorConstants::Svg::lime);
    }
}

// Creates new head.
void MainWindow::new_head()
{
    // Int reduce makes snakes parts the normal size
    // and when snake eats something, the integer is changed
    // to 0. This way snakes part appears bigger.
    int reduce = 20;
    if (ate_){
        reduce = 0;
    }

    // Creating new head
    QGraphicsEllipseItem* new_head;
    QPen blackPen(Qt::black);
    blackPen.setWidth(4);
    new_head = screen_->addEllipse(0, 0, SIZE-reduce, SIZE-reduce,
                                blackPen, QColorConstants::Svg::forestgreen);

    // Centered coordinate values for ellipse. First we find the
    // coordinate and then we times it by SIZE.
    // item can be centered by adding reduce divided by 2.
    int centered_x = field_->get_x()*SIZE+reduce/2;
    int centered_y = field_->get_y()*SIZE+reduce/2;

    // Positions new head
    new_head->setPos(centered_x,centered_y);

    snake_.push_back(new_head);
}

// Adds to time counter and changes Timelabel
void MainWindow::add_time()
{
    time_ += 0.5;
    QString time = QString::number(time_);
    ui->TimeLabel->setText("TIME: " + time);
}

// Implements snakes eating
void MainWindow::snake_eats()
{
    // Deletes and creates new snack.
    delete food_;
    create_food();

    // changes snakes tail color.
    snake_.at(0)->setBrush(QColorConstants::Svg::lightgreen);

    // Speeds up the game by 3 milliseconds
    speed_ = speed_-3;
    timer_.start(speed_);

    length_ += 1;

    // Points are calculated with the formula(length*length*2)-time_.
    // If points are calcualted this way, game rewards you exponentially
    // the longer snake becomes but punishes you the longer it takes.
    pts_ = (length_*length_*2)-time_;
    QString points = QString::number(pts_);
    QString length = QString::number(length_);
    ui->LengthLabel->setText("LENGTH: " + length);
    // Changes scorelabel if score isn't negative. Score might be negative
    // if player doesn't eat anything in the beginning.
    if (pts_ >= 0){
        ui->scoreLabel->setText("SCORE: " + points);
    }
}

// Creates new snack to the board.
void MainWindow::create_food()
{
    // Add new ellipse to game board
    QBrush redBrush(Qt::red);
    QPen blackPen(Qt::black);
    blackPen.setWidth(2);
    food_ = screen_->addEllipse(0, 0, SIZE/2, SIZE/2, blackPen, redBrush);

    // Food is positioned to the middle of square with SIZE/4
    food_->setPos(field_->return_foodX()*SIZE+SIZE/4,
                  field_->return_foodY()*SIZE+SIZE/4);
}

// Disables button that point to opposite direction.
// Enables other movement buttons.
void MainWindow::disable_button()
{
    if (direction_ == "d"){
        ui->LeftButton->setEnabled(false);
        ui->RightButton->setEnabled(true);
        ui->UpButton->setEnabled(true);
        ui->DownButton->setEnabled(true);
    }
    if (direction_ == "s"){
        ui->LeftButton->setEnabled(true);
        ui->RightButton->setEnabled(true);
        ui->UpButton->setEnabled(false);
        ui->DownButton->setEnabled(true);
    }
    if (direction_ == "a"){
        ui->LeftButton->setEnabled(true);
        ui->RightButton->setEnabled(false);
        ui->UpButton->setEnabled(true);
        ui->DownButton->setEnabled(true);
    }
    if (direction_ == "w"){
        ui->LeftButton->setEnabled(true);
        ui->RightButton->setEnabled(true);
        ui->UpButton->setEnabled(true);
        ui->DownButton->setEnabled(false);
    }
}

// Sets up a new game with parameters given by user.
void MainWindow::set_new_game()
{
    reset_stats();

    // Parameters given by the user
    int height_ = ui->heightSpinBox->text().toInt();
    int width_ = ui->widthSpinBox->text().toInt();
    int seed_ = ui->SeedSpinBox->text().toInt();

    // width and height in pixels
    int pixel_width = SIZE*width_;
    int pixel_height = SIZE*height_;

    field_ = make_shared<GameBoard>(width_, height_, seed_);

    // Surface to snake to snake on
    screen_ = new QGraphicsScene(this);
    screen_->setSceneRect(0, 0, SIZE*width_, SIZE*height_);
    screen_->setBackgroundBrush(Qt::cyan);

    // add vertical lines
    for (int x=0; x<=SIZE*width_; x+=SIZE)
        screen_->addLine(x,0,x,pixel_height, QPen(Qt::black));

    // Now add the horizontal lines
    for (int y=0; y<=SIZE*height_; y+=SIZE)
        screen_->addLine(0,y,pixel_width,y, QPen(Qt::black));

    // Max value is 10. The maximum height/width.
    // The game_screen is calcultated to be in the middle
    // with this formula.
    int left_margin = (MAX_VALUE-width_)*(SIZE/2)+ADD_LEFT_MARGIN; //
    int top_margin = (MAX_VALUE-height_)*(SIZE/2)+ADD_TOP_MARGIN; // y coordinate

    // positions graphicsview in desired location.
    ui->game_screen->setGeometry(left_margin, top_margin,
                                   2+pixel_width, 2+pixel_height);
    ui->game_screen->setScene(screen_);

    // Creates snakes head in the board
    new_head();

    //creates snack in the board
    create_food();
    ui->StartButton->setEnabled(true);
    ui->setgamefirst->setText("");

}

// Sets every value to original form
// Enables set to be used mid game if desired.
void MainWindow::reset_stats()
{
    height_ = ui->heightSpinBox->text().toInt();
    width_ = ui->widthSpinBox->text().toInt();
    seed_ = ui->SeedSpinBox->text().toInt();
    snake_.clear();
    ui->TimeLabel->setText("TIME: 0");
    ui->scoreLabel->setText("SCORE: 0");
    ui->LengthLabel->setText("LENGTH: 1");
    ui->selectSizeLabel->setText("Select size 3-10");
    ui->LeftButton->setEnabled(true);
    ui->RightButton->setEnabled(true);
    ui->UpButton->setEnabled(true);
    ui->DownButton->setEnabled(true);
    time_ = 0;
    length_= 1;
    speed_ = 500;
    ui->GameText->setStyleSheet(QStringLiteral
                                ("QLabel{color: rgb(153, 0, 0);}"));
    this->setStyleSheet("background-color: rgb(0, 151, 102);");
    ui->StartButton->setText("PLAY");
    timer_on = false;
    ui->GameText->setText("RATTLESNAKE");
    timer_.stop();
    clock_.stop();
    directionX_ = 0;
    directionY_ = 0;
    direction_ = "d";
}

// implements quit
void MainWindow::quit_button()
{
    qApp->quit();
}

// Start and stop in same button
void MainWindow::start_button()
{
    // if timer is not running. Starts game and changes button text to pause
    if (not timer_on){
        timer_.start(speed_);
        clock_.start(TIME);
        timer_on = true;
        ui->setGameButton->setText("RESET");
        ui->StartButton->setText("PAUSE");
    }
    // timer is running. Stops game and changes buttons text to play
    else{
        timer_.stop();
        clock_.stop();
        ui->StartButton->setText("PLAY");
        timer_on = false;
    }
}

// Implements death
void MainWindow::death()
{
    // Imforms player about death and changes background color to red.
    ui->GameText->setText("YOU DIED");
    ui->StartButton->setEnabled(false);
    ui->GameText->setStyleSheet
            (QStringLiteral("QLabel{color: rgb(120, 120, 255);}"));

    this->setStyleSheet("background-color: red;");

    // Width and height in  graphical setting
    int GUI_width = width_*SIZE;
    int GUI_height = height_*SIZE;

    int X_coordinate = field_->get_x();
    int Y_coordinate = field_->get_y();

    // If snake would be inside wall in next step
    if (X_coordinate*SIZE+directionX_ <= 0 ||
            Y_coordinate*SIZE+directionY_ <= 0){
        ui->selectSizeLabel->setText("RAN INTO WALL");
    }
    else if (X_coordinate*SIZE+directionX_ >= GUI_width ||
             Y_coordinate*SIZE+directionY_ >= GUI_height){
        ui->selectSizeLabel->setText("RAN INTO WALL");
    }
    // Snake died but didn't die of running to wall.
    // So snake tried to eat itself.
    else{
        ui->selectSizeLabel->setText("RAN INTO ITSELF");
    }
    ui->setgamefirst->setText("Set game first");
}
