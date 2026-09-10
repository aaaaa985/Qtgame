#include "resultwidget.h"

ResultWidget::ResultWidget(bool isWin,QWidget *parent):QWidget(parent){
    setFixedSize(600,600);

    if(isWin){
        rbackgroundImage=QPixmap(":/images/win_background.png");
    }
    else{
        rbackgroundImage=QPixmap(":/images/lose_background.png");
    }
    if(rbackgroundImage.isNull()){
        qDebug()<<"Failed to load rbackground image!";
    }
    returnButton=new QPushButton("Return",this);
    QSizePolicy sizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    returnButton->setSizePolicy(sizePolicy);

    QVBoxLayout *verticalLayout=new QVBoxLayout(this);

    QSpacerItem *verticalSpacerTop=new QSpacerItem(20,400,QSizePolicy::Minimum,QSizePolicy::Expanding);
    verticalLayout->addItem(verticalSpacerTop);

    QHBoxLayout *horizontalLayout=new QHBoxLayout();

    QSpacerItem *horizontalSpacerLeft=new QSpacerItem(200,20,QSizePolicy::Expanding,QSizePolicy::Minimum);
    horizontalLayout->addItem(horizontalSpacerLeft);

    horizontalLayout->addWidget(returnButton);

    QSpacerItem *horizontalSpacerRight=new QSpacerItem(200,20,QSizePolicy::Expanding,QSizePolicy::Minimum);
    horizontalLayout->addItem(horizontalSpacerRight);

    verticalLayout->addLayout(horizontalLayout);

    QSpacerItem *verticalSpacerBottom=new QSpacerItem(20,160,QSizePolicy::Minimum,QSizePolicy::Expanding);
    verticalLayout->addItem(verticalSpacerBottom);

    QFont buttonFont("Arial",30);
    buttonFont.setBold(true);
    returnButton->setFont(buttonFont);
    returnButton->update();
    returnButton->setStyleSheet(
        "QPushButton{"
        "   width:200px;"
        "   height:40px;"
        "   background-color:#4CAF50;"
        "   color:white;"
        "   border:2px solid #0000FF;"
        "   border-radius:10px;"
        "   padding:5px;"
        "   margin:5px;"
        "   font-weight:bold;"
        "}"
        "QPushButton:hover{"
        "   background-color:#45a049;"
        "}"
        "QPushButton:pressed{"
        "   background-color:#3e8e41;"
        "}"
    );



    connect(returnButton,&QPushButton::clicked,this,&ResultWidget::onReturnButtonClicked);
}

void ResultWidget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    if(!rbackgroundImage.isNull()){
        painter.drawPixmap(rect(),rbackgroundImage.scaled(size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    }
}

ResultWidget::~ResultWidget(){

}

void ResultWidget::onReturnButtonClicked(){
    emit returnToWelcome();
}




