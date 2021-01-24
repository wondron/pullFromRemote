#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cgraphicsview.h"
#include "cgrascene.h"
#include "cgrawidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_addimage_clicked();

private:
    Ui::MainWindow *ui;
    Graphics::CGraScene* scene;
    Graphics::CGraWidget* view;
};
#endif // MAINWINDOW_H
