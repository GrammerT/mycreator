#include "rect.h"
#include <QPainter>
#include <QBrush>
#include "greenrect.h"
#include "redrect.h"
#include "bluerect.h"
#include <QMouseEvent>
#include <QDebug>

Rect::Rect(QWidget *parent)
    :QFrame(parent)
    ,_leftMousePressed(false)
    ,_direction(eNone)
{
    this->setMouseTracking(true);
}

Rect::~Rect()
{

}


QPoint Rect::startPoint() const
{
    return _startPoint;
}

void Rect::setStartPoint(const QPoint &startPoint)
{
    _startPoint = startPoint;
}

void Rect::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setPen(Qt::NoPen);
    p.setBrush(QBrush(_color));
    p.drawRect(this->rect());
    QFrame::paintEvent(event);
}

void Rect::mousePressEvent(QMouseEvent *event)
{
    qDebug()<<__FUNCTION__;
    _rectDelegate->onSelected(this);
    _leftMousePressed = true;
    _mousePos = QCursor::pos();
    QFrame::mousePressEvent(event);
}

void Rect::mouseMoveEvent(QMouseEvent *event)
{
    QPoint deltPos = QCursor::pos()-_mousePos;
    qDebug()<<__FUNCTION__<<event->pos();
    changedMouseStyle(event->pos());

    _mousePos = QCursor::pos();
    if(_leftMousePressed)
    {
        if(_direction==eNone)
        {
            _rectDelegate->onMouseMoving(this,deltPos);
        }
        else
        {
            _rectDelegate->onChangedRectSize(this,_direction,deltPos);
        }
    }
    QFrame::mouseMoveEvent(event);
}

void Rect::mouseReleaseEvent(QMouseEvent *event)
{
    _direction = eNone;
    _leftMousePressed = false;
    QFrame::mouseReleaseEvent(event);
}

void Rect::changedMouseStyle(QPoint p)
{
    if(p.x()<=1&&(p.y()>1&&p.y()<this->height()-2))
    {
        //左
        _direction = eLeft;
        this->setCursor(Qt::SizeHorCursor);
    }
    if(p.x()<=2&&p.y()<=2)
    {
        //左上
        _direction = eTopLeft;
        this->setCursor(Qt::SizeFDiagCursor);
    }
    if(p.x()<=2&&p.y()>=this->height()-4)
    {
        //左下
        _direction = eBottomLeft;
        this->setCursor(Qt::SizeBDiagCursor);
    }
    if(p.x()>=this->width()-3&&(p.y()>1&&p.y()<this->height()-2))
    {
        //右
        _direction = eRight;
        this->setCursor(Qt::SizeHorCursor);
    }
    if(p.x()>=this->width()-2&&p.y()<=2)
    {
        //右上
        _direction = eTopRight;
        this->setCursor(Qt::SizeBDiagCursor);
    }
    if(p.x()>=this->width()-4&&p.y()>=this->height()-4)
    {
        //右下
        _direction = eBottomRight;
        this->setCursor(Qt::SizeFDiagCursor);
    }

    if(p.y()<=1&&(p.x()>1||p.x()<this->width()-2))
    {
        //上
        _direction = eUp;
        this->setCursor(Qt::SizeVerCursor);
    }
    if(p.y()>=this->height()-2&&(p.x()>1||p.x()<this->width()-2))
    {
        //下
        _direction = eBottom;
        this->setCursor(Qt::SizeVerCursor);
    }

    if(p.x()>2&&p.x()<this->width()-4&&p.y()>2&&p.y()<this->height()-4)
    {
        _direction = eNone;
        this->setCursor(Qt::ArrowCursor);
    }
}

void Rect::onChangedRectSize(QPoint deltPos)
{
    QRect geom = this->geometry();
    qDebug()<<geom;
    switch (_direction) {
    case eLeft:
        geom.setLeft(geom.left()+deltPos.x());
        break;
    case eTopLeft:
        geom.setTopLeft(geom.topLeft()+deltPos);
        break;
    case eBottomLeft:
        geom.setBottomLeft(geom.bottomLeft()+deltPos);
        break;
    case eRight:
        geom.setRight(geom.right()+deltPos.x());
        break;
    case eTopRight:
        geom.setTopRight(geom.topRight()+deltPos);
        break;
    case eBottomRight:
        geom.setBottomRight(geom.bottomRight()+deltPos);
        break;
    case eUp:
        geom.setTop(geom.top()+deltPos.y());
        break;
    case eBottom:
        geom.setBottom(geom.bottom()+deltPos.y());
        break;
    default:
        break;
    }
    this->setGeometry(geom);
}

void Rect::setRectDelegate(RectDelegate *rectDelegate)
{
    _rectDelegate = rectDelegate;
}

Qt::GlobalColor Rect::color() const
{
    return _color;
}

void Rect::setColor(const Qt::GlobalColor &color)
{
    _color = color;
    update();
}

QSize Rect::size() const
{
    return _size;
}

void Rect::setSize(const QSize &size)
{
    _size = size;
    update();
}


Rect *RectFactory::createRect(Qt::GlobalColor color, QWidget *parent)
{
    Rect *rect=0;
    switch (color) {
    case Qt::red:
        rect = new RedRect(parent);
        break;
    case Qt::green:
        rect = new GreenRect(parent);
        break;
    case Qt::blue:
        rect = new BlueRect(parent);
        break;
    default:
        break;
    }
    return rect;
}
