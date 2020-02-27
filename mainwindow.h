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

protected:
	void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
	
public slots:
	void updatePicture_Top(cv::Mat image);
	void updatePicture_Bottom(cv::Mat image);

	void updateObjectPicture_Top(cv::Mat image);
	void updateObjectPicture_Bottom(cv::Mat image);

	void updateNetwork_Top(bool b_con);
	void updateNetwork_Bottom(bool b_con);

	void updateJobInfo_Top(QString qstr_info);
	void updateJobInfo_Bottom(QString qstr_info);

	//button
	void OnButton_Config(void) ;
	void OnButton_Config_New(void) ;
	void OnButton_Config_Save(void) ;

};

#endif // MAINWINDOW_H
