#include "mainwindow.h"
#include "editorgutter.h"
#include "ui_mainwindow.h"

class EditorGutter *gutter = nullptr;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //ui->plainTextEdit->moveCursor(QTextCursor::Start);
    //ui->plainTextEdit->insertPlainText("00:00:01,010 --> 00:00:03,630");

    gutter = new EditorGutter(ui->plainTextEdit);
}

MainWindow::~MainWindow()
{
    delete ui;
}
