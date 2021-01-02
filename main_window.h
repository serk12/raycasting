// Author: Marc Comino 2019

#ifndef MAIN_WINDOW_H_
#define MAIN_WINDOW_H_

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

namespace gui {

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  virtual void show();

 private slots:
  /**
   * @brief on_actionQuit_triggered Closes the application.
   */
  void on_actionQuit_triggered();

  /**
   * @brief on_actionLoad_triggered Opens a file dialog to load a PLY mesh.
   */
  void on_actionLoad_triggered();

 private:
  Ui::MainWindow *ui_;
};

}  //  namespace gui

#endif  //  MAIN_WINDOW_H_
