#include "rectwidget.h"
#include "ui_rectwidget.h"
#include <QDebug>
#include <QMouseEvent>
#include <QCursor>
#include "rect.h"


RectWidget::RectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RectWidget)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setStyleSheet("border:2px solid gray;");
    this->setMouseTracking(true);
    this->setAcceptDrops(true);
    _pressLeftMouse = false;
    _generateRect = false;
}

RectWidget::~RectWidget()
{
    delete ui;
}

void RectWidget::setDelegate(RectUiDelegate *delegate)
{
    _delegate = delegate;
}

void RectWidget::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug()<<__FUNCTION__;

    QWidget::mouseReleaseEvent(event);
}

void RectWidget::onMouseMoving(Rect *rect,QPoint p)
{
    rect->setStartPoint(rect->startPoint()+p);
    qDebug()<< this->mapFromGlobal(rect->startPoint());
    collision(rect);
    rect->move(rect->startPoint());
    update();
}

void RectWidget::enterEvent(QEvent *event)
{
    QPoint p = this->mapFromGlobal(QCursor::pos());
    if(_generateRect&&this->rect().contains(p))
    {
        qDebug()<<__FUNCTION__;
        createNewRect(QCursor::pos());
        _generateRect = false;
    }
    QWidget::enterEvent(event);
}


void RectWidget::mouseMoveEvent(QMouseEvent *event)
{

}

void RectWidget::onSelected(Rect *rect)
{
    int index = _rects.indexOf(rect);
    Rect *trect = _rects.at(index);
    _rects.removeAt(index);
    _rects.push_back(trect);
    update();
}

void RectWidget::onChangedRectSize(Rect *rect, Rect::PullDirection direction, QPoint deltPos)
{
    rect->onChangedRectSize(deltPos);

//    QPoint p = this->mapToGlobal(rect->rect().topLeft());
//    qDebug()<<p;
//    rect->setStartPoint(p);

}


void RectWidget::createNewRect(QPoint &p)
{
    RectFactory rf;
    Rect *rect = rf.createRect(_color,this);
    rect->setWindowFlags(rect->windowFlags() | Qt::FramelessWindowHint
                         | Qt::Tool /*| Qt::WindowStaysOnTopHint*/);
    rect->setSize(QSize(80,80));
    rect->setStartPoint(p);
    rect->setRectDelegate(this);
    rect->setGeometry(p.x()+2,p.y()+2,80,80);
    _rects.push_back(rect);
    update();
}

void RectWidget::collision(Rect *rect)
{
    QPoint topLeft = rect->startPoint();
    QPoint parentPoint = this->mapFromGlobal(rect->startPoint());
    QPoint bottomRight = parentPoint+QPoint(rect->size().width(),rect->size().height());

    if(parentPoint.x()<=0)
    {
        parentPoint.setX(0);
    }
    if(parentPoint.y()<=0)
    {
        parentPoint.setY(0);
    }
    if(bottomRight.x()>=this->width())
    {
        bottomRight.setX(this->width());
        parentPoint.setX(bottomRight.x()-rect->width());
    }
    if(bottomRight.y()>=this->height())
    {
        bottomRight.setY(this->height());
        parentPoint.setY(bottomRight.y()-rect->height());
    }


    topLeft = this->mapToGlobal(parentPoint);

    rect->setStartPoint(topLeft);

}

void RectWidget::setGenerateRect(bool generateRect)
{
    _generateRect = generateRect;
}

void RectWidget::paintEvent(QPaintEvent *event)
{
    for(int i=0;i<_rects.size();++i)
    {
        Rect *rect = _rects.at(i);
        QPoint p = rect->startPoint();
        rect->show();
    }
}

void RectWidget::setColor(const Qt::GlobalColor &color)
{
    _color = color;
}
