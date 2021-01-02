// Author: Marc Comino 2018

#include <main_window.h>

#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>

#include "./ui_main_window.h"

namespace gui {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow) {
    ui_->setupUi(this);
    PlotsRGBA* a = new PlotsRGBA(*ui_->Configuration);
    ui_->glwidget->setPlotRGBA(a);
}

MainWindow::~MainWindow() { delete ui_; }

void MainWindow::show() { QMainWindow::show(); }

void MainWindow::on_actionQuit_triggered() { close(); }

void MainWindow::on_actionLoad_triggered() {
    QString filename = QFileDialog::getExistingDirectory(
      this, "Choose a directory.", ".", QFileDialog::Option::ShowDirsOnly);
    if (!filename.isNull()) {
        if (!ui_->glwidget->LoadVolume(filename))
            QMessageBox::warning(this, tr("Error"), tr("The selected volume could not be opened."));
    }
}

}  //  namespace gui
