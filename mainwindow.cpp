#include "mainwindow.h"
#include "editorgutter.h"
#include "ui_mainwindow.h"

class EditorGutter *gutter = nullptr;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    gutter = new EditorGutter(ui->plainTextEdit);
}

MainWindow::~MainWindow()
{
    delete ui;
}
