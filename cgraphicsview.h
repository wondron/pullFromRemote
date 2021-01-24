#pragma once

#include "qgraphicsview.h"

namespace  Graphics
{

class CGraphicsViewPrivate;
class CGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    CGraphicsView(QWidget* parent = Q_NULLPTR);
    virtual ~CGraphicsView();
    void setAimLineEnable(bool enable);
    void centerOnScene();
    void fitSceneInView();

signals:
    void scaleChanged(qreal scale);

public slots:
    void setScale(qreal scale);

protected:
    void mousePressEvent(QMouseEvent* e);
    void mouuseReleaseEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void wheelEvent(QWheelEvent* e);
    void paintEvent(QPaintEvent* e);
    void keyReleaseEvent(QKeyEvent* e);
    void keyPressEvent(QKeyEvent* e);
    void drawBackground(QPainter* painter, const QRectF& rect);
    bool event(QEvent* e);

private:
    CGraphicsViewPrivate* d;
};

}



