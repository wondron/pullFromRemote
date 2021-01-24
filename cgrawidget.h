#pragma once

#include <QObject>
#include "cgraphicsview.h"

#include <QFrame>

class QGraphicsScene;

namespace Graphics
{

class CGraWidgetPrivate;

class CGraWidget : public QFrame
{
    Q_OBJECT
public:
    CGraWidget(QWidget* parent = Q_NULLPTR);
    virtual ~CGraWidget();
    bool eventFilter(QObject* watched, QEvent* e);
    CGraphicsView* view() const;
    void setScene(QGraphicsScene* scene);

signals:
    void doubleClicked();

private:
    CGraWidgetPrivate* d;
};

}
