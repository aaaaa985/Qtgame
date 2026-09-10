#include "gamewidget.h"
#include <QPainter>
#include <QDebug>

GameWidget::GameWidget(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(1000,600);

    //加载背景图片
    backgroundImage=QPixmap(":/images/background.png");
    if(backgroundImage.isNull()){
        qDebug()<<"Failed to load background image";
    }


    timer=new QTimer(this);
    connect(timer,&QTimer::timeout,this,&GameWidget::updateGame);
    if(timer!=nullptr){
        timer->start(16);
    }


    //初始化一些目标
    for(int i=0;i<6;i++){
        Target target;
        int retryCount=0;
        do{
            target.position.setX(rand()%700+100);
            target.position.setY(rand()%400+20);
        }while(isOverlapping(target.getRect(),targets,obstacles,character,healingPoints,portals)&&++retryCount<500);
        if(retryCount<500){
            targets.append(target);
        }
        else{
            qDebug()<<"Target"<<i<<"placement failed!";
        }
    }

    //初始化一些障碍
    for(int i=0;i<8;i++){
        Obstacle obstacle;
        int retryCount=0;
        do{
            obstacle.position.setX(rand()%700+100);
            obstacle.position.setY(rand()%400+20);
        }while(isOverlapping(obstacle.getRect(),targets,obstacles,character,healingPoints,portals)&&++retryCount<500);
        if(retryCount<500){
            obstacles.append(obstacle);
        }
        else{
            qDebug()<<"Obstacle"<<i<<"placement failed!";
        }
    }

    //初始化回血点
    for(int i=0;i<5;i++){
        HealingPoint healingPoint;
        int retryCount=0;
        do{
            healingPoint.position.setX(rand()%700+100);
            healingPoint.position.setY(rand()%400+20);
        }while (isOverlapping(healingPoint.getRect(),targets,obstacles,character,healingPoints,portals)&&++retryCount<500);
        if(retryCount<500){
            healingPoints.append(healingPoint);
        }
        else{
            qDebug()<<"HealingPoint"<<i<<"placement failed!";
        }
    }

    //初始化传送门
    qDeleteAll(portals);//先释放对象
    portals.clear();//再清空指针列表
    int maxRetries=1000;

    for(int i=0;i<4;i++){
        Portal* portal=new Portal();
        int retryCount=0;
        bool validPosition=false;

        do{
            portal->position.setX(rand()%700+100);
            portal->position.setY(rand()%400+20);
            validPosition=!isOverlapping(portal->getRect(),targets,obstacles,character,healingPoints,portals);
            retryCount++;
        }while(!validPosition&&retryCount<maxRetries);
        if(validPosition){
            portals.append(portal);
        }
        else{
            qDebug()<<"Portal"<<i<<"placement failed after"<<retryCount<<"attemps";
            delete portal;
        }
    }
    //配对门
    if(portals.size()>=4){
        portals.at(0)->pairedPortal=portals.at(1);
        portals.at(1)->pairedPortal=portals.at(0);
        portals.at(2)->pairedPortal=portals.at(3);
        portals.at(3)->pairedPortal=portals.at(2);
    }
    else{
        qDebug()<<"Warning:Not enough portals for pairing!";
    }


    gameOver=false;

    //确保GameWidget可以获得焦点
    setFocusPolicy(Qt::StrongFocus);
    setFocus();
}


GameWidget::~GameWidget(){
    delete timer;
    qDeleteAll(portals);
    portals.clear();
}

void GameWidget::resetGame(){
    character=Character();
    targets.clear();
    obstacles.clear();
    healingPoints.clear();
    qDeleteAll(portals);
    portals.clear();

    for(int i=0;i<6;i++){
        Target target;
        int retryCount=0;
        do{
            target.position.setX(rand()%700+100);
            target.position.setY(rand()%400+20);
        }while(isOverlapping(target.getRect(),targets,obstacles,character,healingPoints,portals)&&++retryCount<500);
        if(retryCount<500){
            targets.append(target);
        }
        else{
            qDebug()<<"Target"<<i<<"placement failed!";
        }
    }

    for(int i=0;i<8;i++){
        Obstacle obstacle;
        int retryCount=0;
        do{
            obstacle.position.setX(rand()%700+100);
            obstacle.position.setY(rand()%400+20);
        }while(isOverlapping(obstacle.getRect(),targets,obstacles,character,healingPoints,portals)&&++retryCount<500);
        if(retryCount<500){
            obstacles.append(obstacle);
        }
        else{
            qDebug()<<"Obstacle"<<i<<"placement failed!";
        }
    }

    for(int i=0;i<5;i++){
        HealingPoint healingPoint;
        int retryCount=0;
        do{
            healingPoint.position.setX(rand()%700+100);
            healingPoint.position.setY(rand()%400+20);
        }while (isOverlapping(healingPoint.getRect(),targets,obstacles,character,healingPoints,portals)&&++retryCount<500);
        if(retryCount<500){
            healingPoints.append(healingPoint);
        }
        else{
            qDebug()<<"HealingPoint"<<i<<"placement failed!";
        }
    }

    qDeleteAll(portals);//先释放对象
    portals.clear();//再清空指针列表
    int maxRetries=1000;

    for(int i=0;i<4;i++){
        Portal* portal=new Portal();
        int retryCount=0;
        bool validPosition=false;

        do{
            portal->position.setX(rand()%700+100);
            portal->position.setY(rand()%400+20);
            validPosition=!isOverlapping(portal->getRect(),targets,obstacles,character,healingPoints,portals);
            retryCount++;
        }while(!validPosition&&retryCount<maxRetries);
        if(validPosition){
            portals.append(portal);
        }
        else{
            qDebug()<<"Portal"<<i<<"placement failed after"<<retryCount<<"attemps";
            delete portal;
        }
    }
    if(portals.size()>=4){
        portals.at(0)->pairedPortal=portals.at(1);
        portals.at(1)->pairedPortal=portals.at(0);
        portals.at(2)->pairedPortal=portals.at(3);
        portals.at(3)->pairedPortal=portals.at(2);
    }
    else{
        qDebug()<<"Warning:Not enough portals for pairing!";
    }

    gameOver=false;

    if(timer->isActive()){
        timer->stop();
    }
    timer->start(16);

    update();
}

void GameWidget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);

    //绘制背景图片
    if(!backgroundImage.isNull()) {
        QPixmap scaledImage = backgroundImage.scaled(rect().size(), Qt::KeepAspectRatioByExpanding);
        painter.drawPixmap(rect(), scaledImage);
    }

    //绘制人物
    if(!character.image.isNull()){

        painter.drawPixmap(character.position.x(),character.position.y(),character.image);
        if(character.isHit){
            if(character.hitFramesLeft%2==0){
                painter.setPen(QPen(Qt::red,2));
                painter.drawRect(character.getRect());
            }
        }
        else if(character.isHealing){
            if(character.healFramesLeft%2==0){
                painter.setPen(QPen(Qt::green,2));
                painter.drawRect(character.getRect());
                if(!character.plusIcon.isNull()){
                    painter.drawPixmap(character.position.x()+character.image.width(),character.position.y(),character.plusIcon);

                }
            }
        }

        //绘制血条
        if(character.health>0){
            painter.save();
            qDebug()<<"Drawing lifebar at("<<character.lifebar.getX()<<","<<character.lifebar.getY()<<")with width"<<character.lifebar.getWidth()<<"and height"<<character.lifebar.getHeight();
            character.lifebar.draw(painter,character.health);
            painter.restore();
        }
    }

    //绘制目标
    for(const auto &target:targets){
        if(!target.image.isNull()){
            painter.drawPixmap(target.position.x(),target.position.y(),target.image);
            if(target.isHit){
                if(target.hitFramesLeft%2==0){
                    painter.setPen(QPen(Qt::red,2));
                    painter.drawRect(target.getRect());
                }
            }
        }
    }

    //绘制障碍
    for(const auto &obstacle:obstacles){
        if(!obstacle.image.isNull()){
            painter.drawPixmap(obstacle.position.x(),obstacle.position.y(),obstacle.image);
        }
    }

    //绘制回血点
    for(const auto &healingPoint:healingPoints){
        if(!healingPoint.image.isNull()){
            painter.drawPixmap(healingPoint.position.x(),healingPoint.position.y(),healingPoint.image);
        }
    }

    //绘制传送门
    for(const auto &portal:portals){
        if(portal&&!portal->image.isNull()){
            painter.drawPixmap(portal->position.x(),portal->position.y(),portal->image);
        }
    }
}

void GameWidget::keyPressEvent(QKeyEvent *event){
    QPointF newPosition=character.position;
    bool shiftPressed=event->modifiers()&Qt::ShiftModifier;
    qDebug()<<"Shift key pressed: "<<shiftPressed;
    if(shiftPressed){
        character.setBoostSpeed();
        event->accept();
        return;
    }
    else{
        character.resetSpeed();
    }
    switch(event->key()){
    case Qt::Key_Left:
        newPosition.rx()-=character.speed;
        if(newPosition.x()<0){
            newPosition.setX(0);
        }
        break;
    case Qt::Key_Right:
        newPosition.rx()+=character.speed;
        if(newPosition.x()+25>width()){
            newPosition.setX(width()-25);
        }
        break;
    case Qt::Key_Up:
        newPosition.ry()-=character.speed;
        if(newPosition.y()<0){
            newPosition.setY(0);
        }
        break;
    case Qt::Key_Down:
        newPosition.ry()+=character.speed;
        if(newPosition.y()+50>height()){
            newPosition.setY(height()-50);
        }
        break;
    case Qt::Key_Space:
        character.isAttacking=true;
        handleAttack();
        break;
    }

    Character tempCharacter=character;
    tempCharacter.position=newPosition;
    QRectF newRect=tempCharacter.getRect();

    bool collision=false;
    for(const auto &obstacle:obstacles){
        Character tempCharacter=character;
        tempCharacter.position=newPosition;
        if(isCollisionWithObstacle(tempCharacter,obstacle)){
            collision =true;
            if(character.health>0){
                character.health--;
                character.startHitEffect();
            }
            break;
        }
    }

    for(auto it=healingPoints.begin();it!=healingPoints.end();){
        if(isCollisionWithHealingPoint(tempCharacter,*it)){
            if(character.health<15){
                character.health++;
                character.startHealEffect();
            }
            it=healingPoints.erase(it);
        }
        else{
            ++it;
        }
    }

    for(auto* portal:portals){
        if(portal&&isCollisionWithPortal(tempCharacter,*portal)){
            if(portal->pairedPortal){
                character.position=portal->pairedPortal->position;
            }
            break;
        }
    }

    if(!collision){
        character.position=newPosition;
    }

    if(character.health<=0){
        emit gameEnded(false);
        gameOver=true;
    }

    update();

    QWidget::keyPressEvent(event);

}


void GameWidget::keyReleaseEvent(QKeyEvent *event){
    if(event->key()==Qt::Key_Shift){
        shiftPressed=false;
        character.resetSpeed();
        event->accept();
    }
    QWidget::keyReleaseEvent(event);
}


void GameWidget::focusInEvent(QFocusEvent *event){
    setFocus();
    QWidget::focusInEvent(event);
}


bool GameWidget::isOverlapping(const QRectF& rect,const QList<Target>& targets,const QList<Obstacle>& obstacles,const Character& character,const QList<HealingPoint>& healingPoints,const QList<Portal*>& portals){
    if(rect.intersects(character.getRect())){
        return true;
    }
    for(const auto& target:targets){
        if(rect.intersects(target.getRect())){
            return true;
        }
    }
    for(const auto& obstacle:obstacles){
        if(rect.intersects(obstacle.getRect())){
            return true;
        }
    }
    for(const auto& healingPoint:healingPoints){
        if(rect.intersects(healingPoint.getRect())){
            return true;
        }
    }
    for(const auto& portal:portals){
        if(portal&&rect.intersects(portal->getRect())){
            return true;
        }
    }
    return false;
}

void GameWidget::updateGame(){
    if(gameOver)
        return;

    for(auto& obstacle:obstacles){
        obstacle.move();

        //处理障碍碰到边界的情况
        if(obstacle.position.x()<0||obstacle.position.x()+obstacle.width>width()||obstacle.position.y()<0||obstacle.position.y()+obstacle.height>height()){
            obstacle.changeDirection();
        }
    }


    for(auto& target:targets){
        target.move();
        target.updateEffects();//更新目标受击效果

        //处理目标碰到边界的情况
        if(target.position.x()<0||target.position.x()+target.image.width()||target.position.y()<0||target.position.y()+target.image.height()){
            target.changeDirection();
        }
    }


    character.updateEffects();//更新人物效果

    for(auto& target:targets){
        target.updateEffects();//确保目标状态更新
    }

    if(targets.empty()){
        emit gameEnded(true);
        gameOver=true;
    }

    if(character.health<=0){
        emit gameEnded(false);
        gameOver=true;
    }

    update();
}

bool GameWidget::isCollision(const Character &character,const Target &target){
    QRectF charRect(character.position.x(),character.position.y(),25,50);
    QRectF targetRect(target.position.x(),target.position.y(),20,20);
    return charRect.intersects(targetRect);
}

bool GameWidget::isCollisionWithObstacle(const Character &character,const Obstacle &obstacle){
    QRectF charRect(character.position.x(),character.position.y(),25,50);
    QRectF obstacleRect(obstacle.position.x(),obstacle.position.y(),60,60);
    bool collision=charRect.intersects(obstacleRect);
    if(collision){
        qDebug()<<"Collision with obstacle detected!";
    }
    return collision;
}

bool GameWidget::isCollisionWithHealingPoint(const Character &character,const HealingPoint &healingPoint){
    QRectF charRect(character.position.x(),character.position.y(),25,50);
    QRectF healingPointRect(healingPoint.position.x(),healingPoint.position.y(),healingPoint.width,healingPoint.height);
    bool collision=charRect.intersects(healingPointRect);
    if(collision){
        qDebug()<<"Collision with healing point detected!";
    }
    return collision;
}

bool GameWidget::isCollisionWithPortal(const Character &character,const Portal &portal){
    QRectF charRect(character.position.x(),character.position.y(),25,50);
    QRectF portalRect(portal.position.x(),portal.position.y(),portal.image.width(),portal.image.height());
    bool collision=charRect.intersects(portalRect);
    if(collision){
        qDebug()<<"Collision with portal detected!";
    }
    return collision;
}

void GameWidget::handleAttack(){
    QRectF attackRect=character.getAttackRect();
    for(auto it=targets.begin();it!=targets.end();){
        if(attackRect.intersects(it->getRect())){
            it->health-=1;
            it->startHitEffect();
            if(it->health<=0){
                it=targets.erase(it);
            }
            else{
                ++it;
            }
        }
        else{
            ++it;
        }
    }
    character.isAttacking=false;
}










