#include "cgraphicsview.h"
#include "QScrollBar"
#include "QLineF"
#include "QtMath"
#include "QVBoxLayout"
#include "QLabel"
#include "QMouseEvent"
#include "cgrascene.h"
#include "QTimer"


namespace Graphics
{
class CGraphicsViewPrivate
{
public:
    CGraphicsViewPrivate(CGraphicsView* qq): q(qq),
        aimLineEnable(true),
        autoScrollEnable(false),
        scale(0)
    {
    }

    void updateAutoScroll()
    {
        static const int offset = 10;
        auto p = q->mapToGlobal(QCursor::pos());
        QRect r(q->rect());
        r.adjust(10,10,-10,-10);
        int dt;
        auto h = q->horizontalScrollBar()->value();

        if(p.x() < r.left()) {
            dt = p.x() - r.left();
            dt = dt < -offset ? -offset : dt;
            q->horizontalScrollBar()->setValue(h + static_cast<int>(dt * 0.5));
        } else if(p.x() > r.right()) {
            dt = p.x() - r.right();
            dt = dt > offset ? offset : dt;
            q->horizontalScrollBar()->setValue(h + static_cast<int>(dt * 0.5));
        }

        auto v = q->verticalScrollBar()->value();

        if(p.y() < r.top()) {
            dt = p.y() - r.top();
            dt = dt < -offset ? -offset : dt;
            q->verticalScrollBar()->setValue(v + static_cast<int>(dt * 0.5));
        } else if(p.y() > r.bottom()) {
            dt = p.y() - r.bottom();
            dt = dt > offset ? offset : dt;
            q->verticalScrollBar()->setValue(v + static_cast<int>(dt * 0.5));
        }
    }

    qreal calcRotate() const
    {
        QLineF line = q->transform().map(QLineF(0,0,10,0));
        return qRadiansToDegrees(qAtan2(line.y2(), line.x2()));
    }

    qreal calcScale() const
    {
        //用自己的transform的比例映射长度为10 的直线，然后得到他的缩放比例。
        QLineF line = q->transform().map(QLineF(0,0,10,0));
        return qSqrt(line.y2() * line.y2() + line.x2() * line.x2())/10;
    }

    CGraphicsView* q;
    bool aimLineEnable;
    bool autoScrollEnable;
    QRectF sceneRect;
    qreal scale;
};

CGraphicsView::CGraphicsView(QWidget* parent):
    QGraphicsView(parent), d(new CGraphicsViewPrivate(this))
{
    auto vlayout = new QVBoxLayout(this);
    vlayout->setContentsMargins(1, 1, 1, 1);
    vlayout->setSpacing(1);
    auto label = new QLabel("", this);
    label->setAttribute(Qt::WA_TransparentForMouseEvents);
    vlayout->addWidget(label);
    vlayout->addStretch();
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

CGraphicsView::~CGraphicsView()
{
    delete d;
}

void CGraphicsView::setAimLineEnable(bool enable)
{
    d->aimLineEnable = enable;
    update();
}

void CGraphicsView::centerOnScene()
{
    //定位中心用的。
    if(scene())
        centerOn(scene()->sceneRect().center());
}

void CGraphicsView::fitSceneInView()
{
    if(scene()) {
        auto rw = viewport()->size().width() / scene()->sceneRect().width();
        auto rh = viewport()->size().height() / scene()->sceneRect().height();
        auto sc  = qMin(rw, rh);

        if(qAbs(d->scale - sc) < 0.001)return;

        QTransform tr;
        tr.scale(sc, sc);
        setTransform(tr);
        update();
    }
}

//没弄明白
void CGraphicsView::setScale(qreal scale)
{
    if(d->scale == scale) return;

    QMatrix matrix;
    matrix.scale(scale, scale);
    matrix.rotate(d->calcRotate());
    setMatrix(matrix);
}

void CGraphicsView::mousePressEvent(QMouseEvent* e)
{
    if(e->button() == Qt::LeftButton) d->autoScrollEnable = true;
    //鼠标中键表示拖拽
    else if(e->button() == Qt::MiddleButton) {
        d->autoScrollEnable = false;
        setDragMode(QGraphicsView::ScrollHandDrag);
        //发生中键的按时，返回的还是左键。 第一个left是触发鼠标事件的按键，第二个是触发事件时，鼠标的状态
        QMouseEvent evt(e->type(), e->localPos(), e->windowPos(), e->screenPos(), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier, e->source());
        QGraphicsView::mousePressEvent(&evt);
    }

    QGraphicsView::mousePressEvent(e);
}

void CGraphicsView::mouuseReleaseEvent(QMouseEvent* e)
{
    if(e->button() == Qt::LeftButton)d->autoScrollEnable = false;
    else if(e->button() == Qt::MiddleButton) {
        setDragMode(QGraphicsView::NoDrag);
    }

    QGraphicsView::mouseReleaseEvent(e);
}

void CGraphicsView::mouseMoveEvent(QMouseEvent* e)
{
    QGraphicsView::mouseMoveEvent(e);
}

void CGraphicsView::wheelEvent(QWheelEvent* e)
{
    //QGraphicsView::wheelEvent(e);
    if(e->buttons()&Qt::MiddleButton)e->accept();
}

void CGraphicsView::keyReleaseEvent(QKeyEvent* e)
{
    if(e->modifiers() == Qt::NoModifier) {
        setDragMode(QGraphicsView::NoDrag);
    }

    QGraphicsView::keyReleaseEvent(e);
}

void CGraphicsView::keyPressEvent(QKeyEvent* e)
{
    if(e->modifiers() == Qt::ShiftModifier) {
        d->autoScrollEnable = false;
        setDragMode(QGraphicsView::ScrollHandDrag);
    }

    QGraphicsView::keyPressEvent(e);
}

void CGraphicsView::paintEvent(QPaintEvent* e)
{
    QGraphicsView::paintEvent(e);
    QPainter painter(viewport());
    painter.setPen(QPen(Qt::green, 1, Qt::SolidLine));
    QRect r = viewport()->rect();

    if(d->aimLineEnable) {
        QPoint c = r.center();
        painter.drawLine(QLine(c.x(), c.y() - 3, c.x(), c.y() - 10));
        painter.drawLine(QLine(c.x() + 3, c.y(), c.x() + 10, c.y()));
        painter.drawLine(QLine(c.x(), c.y() + 3, c.x(), c.y() + 10));
        painter.drawLine(QLine(c.x() - 3, c.y(), c.x() - 10, c.y()));
    }

    //获取真实的比例
    qreal scale = d->calcScale();

    if(scale != d->scale) {
        d->scale = scale;
        emit scaleChanged(scale);
    }

    //显示缩放比例的矩形大小
    QRect textRect(r.left() + 10, r.bottom() - 28, 38, 20);
    painter.save(); //和painter.restore()联合使用;
    painter.setBrush(Qt::black);
    painter.setOpacity(0.75); //设置不透明度
    painter.drawRect(textRect);
    painter.restore();//将save的painter回复过来。
    painter.drawText(textRect, Qt::AlignCenter, QString ::asprintf("%03.0f%%", scale*100));
    CGraScene* s = qobject_cast<CGraScene*>(scene());

    if(s) {
        QRect textRect(r.left() + 2, r.top() + 2, 50, 20);
        painter.drawText(textRect, Qt::AlignCenter, QString::asprintf("fps:%03d", s->fps()));
    }
}

//更换view的底色
void CGraphicsView::drawBackground(QPainter* painter, const QRectF& rect)
{
    painter->fillRect(rect, QColor(Qt::lightGray));
}

bool CGraphicsView::event(QEvent* e)
{
    bool fit = false;

    switch(e->type()) {
    case QEvent::Show:
        //fit = true;
        break;

    case QEvent::Resize:

    //fit = true;
    default:
        break;
    }

    if(scene() && fit == false) {
        if(d->sceneRect != scene()->sceneRect()) {
            d->sceneRect = scene()->sceneRect();
            fit = true;
        }
    }

    if(fit)QTimer::singleShot(20, [this]() {
        fitSceneInView();
    });
    return QGraphicsView::event(e);
}

}

