#ifndef RECT_H
#define RECT_H

#include <QObject>
#include <QColor>
#include <QPoint>
#include <QSize>
#include <QLabel>
#include <QFrame>
#include <QMouseEvent>

class Rect : public QFrame
{
    Q_OBJECT
public:

    enum PullDirection
    {
        eNone,
        eLeft,
        eTopLeft,
        eBottomLeft,
        eRight,
        eTopRight,
        eBottomRight,
        eUp,
        eBottom
    };

    class RectDelegate
    {
    public:
        virtual ~RectDelegate(){}
        virtual void onSelected(Rect *rect){}
        virtual void onMouseMoving(Rect *rect,QPoint p){}
        virtual void onChangedRectSize(Rect *rect,PullDirection direction,QPoint deltPos){}
    };

    explicit Rect(QWidget *parent = 0);
    virtual ~Rect();
    QPoint startPoint() const;
    void setStartPoint(const QPoint &startPoint);

    QSize size() const;
    void setSize(const QSize &size);

    Qt::GlobalColor color() const;
    void setColor(const Qt::GlobalColor &color);

    void setRectDelegate(RectDelegate *rectDelegate);

protected:
    virtual void paintEvent(QPaintEvent *event)override;
    virtual void mousePressEvent(QMouseEvent *event)override;
    virtual void mouseMoveEvent(QMouseEvent *event)override;
    virtual void mouseReleaseEvent(QMouseEvent *event)override;
signals:

public slots:

public:

    void onChangedRectSize(QPoint deltPos);
private:
    void changedMouseStyle(QPoint p);


private:
    Qt::GlobalColor _color;
    QPoint _startPoint;
    QSize _size;

    bool _leftMousePressed;
    QPoint _mousePos;
    PullDirection _direction;

    RectDelegate *_rectDelegate;
};

class RectFactory
{
public:
    Rect *createRect(Qt::GlobalColor color,QWidget *parent = 0);
};

#endif // RECT_H
