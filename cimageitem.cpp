#include "cimageitem.h"
#include <QPainter>
#include <QDebug>
#include <chrono>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStyleOptionGraphicsItem>
#include <QCoreApplication>
#include <QEvent>
#include <QTimer>
#include "cimageitem.h"

namespace Graphics
{

class CGraphicsImageItemEvent : public QEvent
{
public:
    enum { Type = QEvent::MaxUser - 88};

    CGraphicsImageItemEvent(const QImage& image)
        : QEvent(QEvent::Type(Type)), m_image(image) {}

    QImage image() const
    {
        return m_image;
    }

private:
    QImage m_image;

};

CImageItem::CImageItem(QGraphicsItem* parent)
    :QGraphicsObject(parent)
{
}

CImageItem::~CImageItem()
{
}

QImage CImageItem::image() const
{
    return m_image;
}

void CImageItem::setImage(const QImage& image)
{
    qApp->postEvent(this, new CGraphicsImageItemEvent(image));
}

bool CImageItem::event(QEvent* ev)
{
    if(ev->type() == CGraphicsImageItemEvent::Type) {
        auto img = static_cast<CGraphicsImageItemEvent*>(ev)->image();
        bool flag = (m_image.size() != img.size()) || m_image.isNull();
        m_image = img;

        if(flag) {
            scene()->setSceneRect(boundingRect());
            //            QTimer::singleShot(0,[this](){
            //                scene()->setSceneRect(boundingRect());
            //                qDebug()<<boundingRect();
            //            });
        }

        update();
        return true;
    }

    return QGraphicsObject::event(ev);
}

QPointF CImageItem::offset() const
{
    return  m_offset;
}

void CImageItem::setOffset(const QPointF& offset)
{
    if(m_offset == offset)return;

    //不清楚怎么用，
    prepareGeometryChange();
    m_offset = offset;
    update();
}

QRectF CImageItem::boundingRect() const
{
    if(m_image.isNull()) return QRectF(-100, -100, 200, 200);

    return QRectF(m_offset, m_image.size() / m_image.devicePixelRatioF());
}

void CImageItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if(m_image.isNull())return;

//    auto start = std::chrono::steady_clock::now();
    painter->drawImage(option->exposedRect, m_image, m_image.rect(), Qt::NoFormatConversion);
//    auto end = std::chrono::steady_clock::now();
//    qInfo() << "t="
//            << QByteArray::number(std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count(), 'f', 9).data()
//            << 's';
}

}
