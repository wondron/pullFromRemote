#pragma once
#include <QObject>

#include <QGraphicsScene>

namespace Graphics
{

class CImageItem;
class CGraScenePrivate;

class CGraScene : public QGraphicsScene
{
    Q_OBJECT
public:
    CGraScene(QObject* parent = nullptr);
    virtual ~CGraScene();

    CImageItem* centerImageItem() const;
    void setCenterImageAimVisible(bool visible);
    int fps() const;

public slots:
    void setCenterImage(const QImage& image);

signals:
    void centerImageChanged();
    void fpsChanged();

private:
    CGraScenePrivate* d;
};

}





