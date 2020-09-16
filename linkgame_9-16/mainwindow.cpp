#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QLayout>
#include<QWidget>
#include"map.h"
#include"help.h"
#include<QPushButton>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    help =new Help;
    connect(ui->helpText,SIGNAL(clicked()),help,SLOT(exec()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
