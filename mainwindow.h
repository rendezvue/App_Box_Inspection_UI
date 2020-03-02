#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPainter>

#include "censemble.h"
#include "cmat2qimage.h"
#include "CIniInfo.h"

namespace Ui {
class MainWindow;
}

typedef struct _SelectRegion
{	
	int x = -1,y = -1,w = 0,h = 0 ;
	int type = -1 ;
}SelectRegion;

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

	 int m_top_point_x ;
	 int m_top_point_y ;

	 int m_bottom_point_x ;
	 int m_bottom_point_y ;

	 SelectRegion m_select_region ;

	 bool m_set_user_level_top ;
	 bool m_set_user_level_bottom ;
	 
protected:
	void showEvent(QShowEvent *ev) override;
	//bool eventFilter(QObject*, QEvent*);
#if 1
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
#endif
	
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

	void OnButton_Test_Run(void) ;

	void updateRunCrack_Top(bool b_run) ;
	void updateRunCrack_Bottom(bool b_run) ;

	void updateLevelCrack_Top(int level) ;
	void updateLevelCrack_Bottom(int level) ;

	//slider
	void OnSliderSetTopLevel(void) ;
    void OnSliderTopMove(int value) ;

	//slider
	void OnSliderSetBottomLevel(void) ;
    void OnSliderBottomMove(int value) ;

};

#endif // MAINWINDOW_H
