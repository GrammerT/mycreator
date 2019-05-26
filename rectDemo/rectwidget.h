#ifndef RECTWIDGET_H
#define RECTWIDGET_H

#include <QWidget>
#include "rect.h"

namespace Ui {
class RectWidget;
}

class Rect;

class RectWidget : public QWidget
        ,public Rect::RectDelegate
{
    Q_OBJECT

public:

    class RectUiDelegate
    {
    public:
        ~RectUiDelegate(){}
        virtual void on(){}
    };

    explicit RectWidget(QWidget *parent = 0);
    ~RectWidget();

    void setDelegate(RectUiDelegate *delegate);


    virtual void mouseMoveEvent(QMouseEvent *event)override;
    virtual void mouseReleaseEvent(QMouseEvent *event)override;

    virtual void onMouseMoving(Rect *rect, QPoint p)override;
    virtual void enterEvent(QEvent *event)override;
    virtual void onSelected(Rect *rect)override;
    virtual void onChangedRectSize(Rect *rect,Rect::PullDirection direction,QPoint deltPos)override;


    void setColor(const Qt::GlobalColor &color);
    void setGenerateRect(bool generateRect);

protected:
    virtual void paintEvent(QPaintEvent *event)override;

private:
    void createNewRect(QPoint &p);
    void collision(Rect *rect);
private:
    Ui::RectWidget *ui;
    bool _pressLeftMouse;
    bool _generateRect;
    RectUiDelegate *_delegate;
    Qt::GlobalColor _color;
    QVector<Rect*> _rects;
};

#endif // RECTWIDGET_H
