#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "censemble.h"
#include "cmat2qimage.h"
#include "CIniInfo.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

	 CEnsemble* m_pEnsemble[2];

	 CIniInfo m_cls_info ;

public slots:
	void updatePicture_Top(cv::Mat image);
	void updatePicture_Bottom(cv::Mat image);

};

#endif // MAINWINDOW_H
