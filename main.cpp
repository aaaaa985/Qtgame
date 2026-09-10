#include "gamewidget.h"
#include "welcomewidget.h"
#include "resultwidget.h"
#include <QApplication>
#include <cstdlib>
#include <ctime>

int main(int argc, char *argv[])
{

    //初始化随机数种子
    srand(time(NULL));

    QApplication a(argc, argv);

    WelcomeWidget welcomeWidget;
    GameWidget gameWidget;

    QObject::connect(&welcomeWidget,&WelcomeWidget::startGameClicked,[&](){
        welcomeWidget.hide();
        gameWidget.resetGame();
        gameWidget.show();
    });

    QObject::connect(&gameWidget,&GameWidget::gameEnded,[&](bool isWin){
        gameWidget.hide();
        ResultWidget *resultWidget=new ResultWidget(isWin);
        QObject::connect(resultWidget,&ResultWidget::returnToWelcome,[&](){
            resultWidget->hide();
            welcomeWidget.show();
            delete resultWidget;
        });
        resultWidget->show();
    });

    welcomeWidget.show();

    return a.exec();
}






