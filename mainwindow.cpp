#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    gameWidget=new GameWidget(this);
    setCentralWidget(gameWidget);
    resize(1000,600);

    QTimer::singleShot(0,gameWidget,[this](){
        gameWidget->setFocus();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}




