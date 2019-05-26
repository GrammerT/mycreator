#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDialog>
#include <QListWidget>
#include "rectwidget.h"
#include "listwidget.h"

namespace Ui {
class Widget;
}

class RectWidget;

class ListWidget;
class QLabel;
class Rect;


class Widget : public QDialog
                    ,public RectWidget::RectUiDelegate
                    ,public ListWidget::ListWidgetDelegate
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();


protected:
    virtual void mouseMoveEvent(QMouseEvent *event)override;

    virtual void onMousePress(QPoint curPos, QString colorName)override;
    virtual void onMouseMove(QPoint curPos)override;
    virtual void onMouseRelease(QPoint curPos)override;

    virtual void onItemChanged(QString colorName)override;
    virtual void leaveWidget()override;


private:
    void initUI();

    void initListWidget();

private:
    Ui::Widget *ui;
    ListWidget *m_pListWidget;
    RectWidget  *m_pRectWidget;
    Rect *_hoverRect;
    QMap<QString,Qt::GlobalColor> strMapColor;
};

#endif // WIDGET_H
