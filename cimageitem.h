#pragma once

#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QImage>

namespace Graphics
{

class CImageItem:public QGraphicsObject
{
    Q_OBJECT
public:
    explicit CImageItem(QGraphicsItem* parennt = Q_NULLPTR);
    ~CImageItem() override;

    QImage image() const;

    QPointF offset() const;
    void setOffset(const QPointF& offset);
    inline void setOffset(qreal x, qreal y);

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

public slots:
    void setImage(const QImage& image);

private:
    bool event(QEvent* ev) override;
    QImage m_image;
    QPointF m_offset;
};

void CImageItem::setOffset(qreal x, qreal y)
{
    setOffset(QPointF(x, y));
}

}
