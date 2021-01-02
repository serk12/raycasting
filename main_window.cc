// Author: Marc Comino 2018

#include <main_window.h>

#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>

#include "plotrgb.h"
#include "./ui_main_window.h"

namespace gui {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow) {
    ui_->setupUi(this);
    PlotRGB* r = new PlotRGB(RGBaType::Red);
    ui_->Configuration->addWidget(r);

    PlotRGB* g = new PlotRGB(RGBaType::Green);
    ui_->Configuration->addWidget(g);

    PlotRGB* b = new PlotRGB(RGBaType::Blue);
    ui_->Configuration->addWidget(b);

    PlotRGB* a = new PlotRGB(RGBaType::Alpha);
    ui_->Configuration->addWidget(a);
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
