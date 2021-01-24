#include "cgrawidget.h"
#include "cgrascene.h"
#include "cgraphicsview.h"

#include <QToolBar>
#include <QLabel>
#include <QSlider>
#include <QLayout>
#include <QtMath>
#include <QWheelEvent>

namespace Graphics
{

class CGraWidgetPrivate
{
public:
    CGraWidgetPrivate(CGraWidget* qq): q(qq) {}
    CGraWidget* q;
    QToolBar* toolbar;
    QSlider* scalelider;
    CGraphicsView* view;
};

CGraWidget::CGraWidget(QWidget* parent):
    QFrame(parent),
    d(new CGraWidgetPrivate(this))
{
    d->view = new CGraphicsView(this);
    d->toolbar =new QToolBar(this);
    d->toolbar->setProperty("background", QColor(0xEEEEEE));
    d->scalelider = new QSlider(this);
    d->scalelider->setOrientation(Qt::Horizontal);
    d->scalelider->setMaximumWidth(200);
    d->scalelider->setRange(100, 2000);
    setFrameStyle(QFrame::Box);
    d->view->setFrameStyle(QFrame::NoFrame);
    auto lineFrame = new QFrame(this);
    lineFrame->setFrameStyle(QFrame::HLine | QFrame::Plain);
    auto l = new QVBoxLayout(this);
    l->setMargin(0);
    l->setSpacing(0);
    l->addWidget(d->view);
    l->addWidget(lineFrame);
    l->addWidget(d->toolbar);
    d->scalelider->hide();
    d->toolbar->addWidget(d->scalelider);
    d->toolbar->addAction(QIcon(QPixmap(":/image/48/Zoom Out.png")), "Zoom Out", [this]() {
        d->scalelider->setValue(d->scalelider->value() - 40);
    });
    d->toolbar->addAction(QIcon(QPixmap(":/image/48/Zoom In.png")), "Zoom In", [this]() {//QPixmap(":/image/48/Zoom In.png")
        d->scalelider->setValue(d->scalelider->value() + 40);
    });
    d->toolbar->addAction(QIcon(QPixmap(":/image/48/Original Size.png")), "1:1", [this]() {
        d->scalelider->setValue(1000);
    });
    d->toolbar->addAction(QIcon(QPixmap(":/image/48/Collapse.png")), "Fit", [this]() {
        d->view->fitSceneInView();
    });
    d->toolbar->addAction(QIcon(QPixmap(":/image/48/Collapse.png")), "Center", [this]() {
        d->view->centerOnScene();
    });
    d->toolbar->addSeparator();
    auto action = new QAction("Show Aim", d->toolbar);
    //action->setIcon(ICON_CHECK.icon());
    action->setCheckable(true);
    action->setChecked(true);
    d->toolbar->addAction(action);
    auto labelaim = new QLabel("AIM");
    labelaim->setProperty("lightColored", true);
    d->toolbar->addWidget(labelaim);
    d->toolbar->setIconSize(QSize(48, 48));
    connect(action, &QAction::triggered, [this](bool value) {
        auto obj = qobject_cast<CGraScene*>(d->view->scene());

        if(obj)obj->setCenterImageAimVisible(value);
    });
    connect(d->scalelider, &QSlider::valueChanged, [this](int v) {
        qreal scale = qreal(v) / qreal(1000);
        scale *= scale;
        d->view->setScale(scale);
    });
    d->view->installEventFilter(this);
}

CGraWidget::~CGraWidget()
{
    delete d;
}

bool CGraWidget::eventFilter(QObject* watched, QEvent* e)
{
    if(e->type() == QEvent::Wheel) {
        auto event = static_cast<QWheelEvent*>(e);
        auto v = d->scalelider->value();

        if(event->delta() > 0)d->scalelider->setValue(v + 20);
        else d->scalelider->setValue(v - 20);

        event->accept();
        return true;
    } else if(e->type() == QEvent::MouseButtonDblClick) {
        auto event = static_cast<QMouseEvent*>(e);

        if(event->button() == Qt::LeftButton)
            emit doubleClicked();
    }

    return QFrame::eventFilter(watched, e);
}

CGraphicsView* CGraWidget::view() const
{
    return d->view;
}

void CGraWidget::setScene(QGraphicsScene* scene)
{
    d->view->setScene(scene);
}

}
