#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QWidget>
#include <QListWidget>

namespace Ui {
class ListWidget;
}

class ListWidget : public QListWidget
{
    Q_OBJECT

public:

    class ListWidgetDelegate
    {
    public:
        ~ListWidgetDelegate(){}
        virtual void leaveWidget(){}
        virtual void onMousePress(QPoint curPos,QString colorName){}
        virtual void onMouseMove(QPoint curPos){}
        virtual void onMouseRelease(QPoint curPos){}
        virtual void onItemChanged(QString colorName){}
    };

    explicit ListWidget(QWidget *parent = 0);
    ~ListWidget();

    void setDelegate(ListWidgetDelegate *delegate);



public slots:
    void onListItemEnter(QListWidgetItem *item);
    void onListItemRowChanged(int row);
    void onItemEntered(QListWidgetItem *item);

protected:
    virtual void mousePressEvent(QMouseEvent *event)override;
    virtual void mouseMoveEvent(QMouseEvent *event)override;
    virtual void mouseReleaseEvent(QMouseEvent *event)override;

private:
    void initUI();

private:
    Ui::ListWidget *ui;
    ListWidgetDelegate *_delegate;
};

#endif // LISTWIDGET_H
