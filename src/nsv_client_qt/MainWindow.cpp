#include "MainWindow.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->goButton, SIGNAL(clicked()), this, SLOT(onUpdate()));
}

/**
@brief Update web page
*/
void MainWindow::onUpdate()
{
    QMessageBox::warning(this, "", ui->lineEdit->text());
    ui->webView->load(QUrl(ui->lineEdit->text()));

    //ui->webView->setHtml(QString::fromStdString(answer), QUrl(QString::fromStdString(html)));
}

MainWindow::~MainWindow()
{
    delete ui;
}
