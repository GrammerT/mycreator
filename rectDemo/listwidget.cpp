#include "listwidget.h"
#include "ui_listwidget.h"
#include <QCursor>
#include <QMouseEvent>
#include <QDebug>

ListWidget::ListWidget(QWidget *parent) :
    QListWidget(parent)
  ,_delegate(nullptr)
{
    this->setMouseTracking(true);
    initUI();
}

ListWidget::~ListWidget()
{

}

void ListWidget::onListItemEnter(QListWidgetItem *item)
{
    _delegate->onItemChanged(item->text());
}

void ListWidget::onListItemRowChanged(int row)
{
    qDebug()<<__FUNCTION__<< row;
}

void ListWidget::onItemEntered(QListWidgetItem *item)
{
    qDebug()<<__FUNCTION__;
}

void ListWidget::mousePressEvent(QMouseEvent *event)
{
    QListWidgetItem *item = this->itemAt(event->pos());
    if(item!=NULL)
    {
        _delegate->onMousePress(QCursor::pos(),item->text());
    }
    QListWidget::mousePressEvent(event);
}

void ListWidget::mouseMoveEvent(QMouseEvent *event)
{
    _delegate->onMouseMove(QCursor::pos());
    QListWidget::mouseMoveEvent(event);
}

void ListWidget::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug()<<__FUNCTION__;
    _delegate->onMouseRelease(QCursor::pos());
    QListWidget::mouseReleaseEvent(event);
}


void ListWidget::initUI()
{
    connect(this,SIGNAL(itemEntered(QListWidgetItem*)),this,SLOT(onListItemEnter(QListWidgetItem*)));
    connect(this,SIGNAL(currentRowChanged(int)),this,SLOT(onListItemRowChanged(int)));
    connect(this,SIGNAL(itemEntered(QListWidgetItem*)),this,SLOT(onItemEntered(QListWidgetItem*)));

}


void ListWidget::setDelegate(ListWidgetDelegate *delegate)
{
    _delegate = delegate;
}
