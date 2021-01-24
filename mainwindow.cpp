#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace Graphics;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new CGraScene();
    view = new CGraWidget();
    view->setScene(scene);
    ui->verlayout->addWidget(view);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btn_addimage_clicked()
{
    scene->setCenterImage(QImage("F:\\IMG_9307.JPG"));
}
