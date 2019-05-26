#include "widget.h"
#include "ui_widget.h"
#include <QHBoxLayout>
#include "rectwidget.h"
#include <QSizePolicy>
#include <QDebug>
#include <QCursor>
#include "rectwidget.h"
#include "rect.h"
#include "listwidget.h"




Widget::Widget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Widget)
  ,_hoverRect(nullptr)
{
    this->setMouseTracking(true);
    ui->setupUi(this);
    this->setFixedSize(800,600);
    initUI();
}

Widget::~Widget()
{
    delete m_pListWidget;
    delete m_pRectWidget;
    delete ui;

}


void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(_hoverRect)
    {
        _hoverRect->setGeometry(QCursor::pos().x()+2,QCursor::pos().y()+2,20,20);
    }
    QWidget::mouseMoveEvent(event);
}

void Widget::onMousePress(QPoint curPos,QString colorName)
{
    Qt::GlobalColor color = strMapColor[colorName];
    if(_hoverRect==nullptr)
    {
        RectFactory rf;
        _hoverRect = rf.createRect(color,this);
        _hoverRect->setWindowFlags(_hoverRect->windowFlags() | Qt::FramelessWindowHint
                                   | Qt::Tool /*| Qt::WindowStaysOnTopHint*/);
        _hoverRect->setMouseTracking(false);
    }
    _hoverRect->setColor((color));
    _hoverRect->setGeometry(QCursor::pos().x()+2,QCursor::pos().y()+2,20,20);
    _hoverRect->show();
}

void Widget::onMouseMove(QPoint curPos)
{
    if(_hoverRect)
    {
        _hoverRect->setGeometry(QCursor::pos().x()+2,QCursor::pos().y()+2,20,20);
    }
}

void Widget::onMouseRelease(QPoint curPos)
{
    if(_hoverRect)
    {
        if(!_hoverRect->isVisible())
            return ;
        _hoverRect->setVisible(false);
        m_pRectWidget->setColor(_hoverRect->color());
        m_pRectWidget->setGenerateRect(true);
        delete _hoverRect;
        _hoverRect = 0;
    }
}

void Widget::onItemChanged(QString colorName)
{
    Qt::GlobalColor color = strMapColor[colorName];
    if(_hoverRect)
    {
        _hoverRect->setColor((color));
        _hoverRect->setGeometry(QCursor::pos().x()+2,QCursor::pos().y()+2,20,20);
        _hoverRect->show();
    }
}

void Widget::leaveWidget()
{
    if(_hoverRect)
    {
        if(!_hoverRect->isVisible())
            return ;
        _hoverRect->setVisible(false);
        m_pRectWidget->setColor(_hoverRect->color());
        m_pRectWidget->setGenerateRect(true);
    }
}

void Widget::initUI()
{
    m_pListWidget = new ListWidget(this);
    m_pListWidget->setDelegate(this);
    m_pListWidget->setMouseTracking(true);
    m_pListWidget->setFixedWidth(80);

    m_pRectWidget = new RectWidget(this);
    m_pRectWidget->setDelegate(this);
    m_pRectWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QHBoxLayout *hlayout = new QHBoxLayout(this);
    hlayout->addWidget(m_pListWidget);
    hlayout->addWidget(m_pRectWidget);
    this->setLayout(hlayout);
    initListWidget();

}

void Widget::initListWidget()
{
    strMapColor[tr("红色")] = Qt::red;
    strMapColor[tr("绿色")] = Qt::green;
    strMapColor[tr("蓝色")] = Qt::blue;
    m_pListWidget->addItem(tr("红色"));
    m_pListWidget->addItem(tr("绿色"));
    m_pListWidget->addItem(tr("蓝色"));
}
