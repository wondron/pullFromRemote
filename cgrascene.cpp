#include "cgrascene.h"
#include "qgraphicsitem.h"
#include "qpainter.h"
#include "QTimer"

#include "cimageitem.h"

namespace Graphics
{

class CAimLineItem: public QGraphicsItem
{
public:
    CAimLineItem(QGraphicsItem* parent): QGraphicsItem(parent)
    {
        m_pen.setStyle(Qt::DashDotLine);
        m_pen.setColor(Qt::yellow);
        m_pen.setWidth(1);
        m_pen.setCosmetic(true);
        setZValue(1000);
    }
    virtual QRectF boundingRect() const override
    {
        return parentItem()->boundingRect();
    }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override
    {
        Q_UNUSED(option)
        Q_UNUSED(widget)
        painter->setPen(m_pen);
        auto r = parentItem()->boundingRect();
        auto c = r.center();
        painter->drawLine(QPointF(c.x(), r.top()), QPointF(c.x(),r.bottom()));
        painter->drawLine(QPointF(r.left(), c.y()), QPointF(r.right(), c.y()));
    }

private:
    QPen m_pen;
};

class CGraPixmapItem : public QGraphicsPixmapItem
{
public:
    explicit CGraPixmapItem(QGraphicsItem* parent = nullptr)
        : QGraphicsPixmapItem(parent)
    {
    }

    virtual ~CGraPixmapItem() override
    {
    }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override
    {
        QGraphicsPixmapItem::paint(painter, option, widget);
    }
};

class CGraScenePrivate
{
public:
    CGraScenePrivate(CGraScene* qq) : q(qq)
    {
        centerImage = new CImageItem();
        centerImageAim = new CAimLineItem(centerImage);
        fpsTimer = new QTimer(q);
        fps = 0;
        count = 0;
        QObject::connect(fpsTimer, &QTimer::timeout,[this]() {
            if(fps != count) {
                fps = count;
                emit q->fpsChanged();
            }

            count = 0;
        });
        fpsTimer->start(1000);
        q->addItem(centerImage);
        QObject::connect(q, &CGraScene::sceneRectChanged, [this](const QRectF& rect) {
            Q_UNUSED(rect)
            q->setSceneRect(centerImage->boundingRect());
        });
    }

    ~CGraScenePrivate()
    {
        delete centerImage;
    }

public:
    CGraScene* q;
    CImageItem* centerImage;
    CAimLineItem* centerImageAim;
    QTimer* fpsTimer;
    int count;
    int fps;
};

CGraScene::CGraScene(QObject* parent)
    : QGraphicsScene(parent),
      d(new CGraScenePrivate(this))
{
}

CGraScene::~CGraScene()
{
    delete d;
}

CImageItem* CGraScene::centerImageItem() const
{
    return d->centerImage;
}

void CGraScene::setCenterImageAimVisible(bool visible)
{
    d->centerImageAim->setVisible(visible);
}

int CGraScene::fps() const
{
    return d->fps;
}

void CGraScene::setCenterImage(const QImage& image)
{
    //    if (!image.isNull() || d->centerImage->image().size() != image.size())
    //        setSceneRect(0,0,image.size().width(),image.size().height());
    d->centerImage->setImage(image);
    d->count ++;
    emit centerImageChanged();
}


}
