#ifndef RESULTWIDGET_H
#define RESULTWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

class ResultWidget:public QWidget{
    Q_OBJECT
public:
    explicit ResultWidget(bool isWin,QWidget *parent=nullptr);
    ~ResultWidget();
signals:
    void returnToWelcome();
private slots:
    void onReturnButtonClicked();
protected:
    void paintEvent(QPaintEvent *event)override;
private:
    QPushButton *returnButton;
    QPixmap rbackgroundImage;
};

#endif // RESULTWIDGET_H


