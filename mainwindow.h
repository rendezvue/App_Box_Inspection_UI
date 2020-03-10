#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPainter>
#include <qprogressbar.h>
#include <QThread>

#include "censemble.h"
#include "cmat2qimage.h"
#include "CIniInfo.h"

#include <QDesktopServices> 

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

	 CEnsemble* m_pEnsemble;

	 CIniInfo m_cls_info ;

	 int m_top_point_x ;
	 int m_top_point_y ;

	 int m_bottom_point_x ;
	 int m_bottom_point_y ;

	 SelectRegion m_select_region ;

	 bool m_set_user_level_top ;
	 bool m_set_user_level_bottom ;

public:
	void UI_UpdateProgressbarColorStatus(QProgressBar *pbar, const float quality, const float level) ;
	
protected:
	void showEvent(QShowEvent *ev) override;
	//bool eventFilter(QObject*, QEvent*);
#if 1
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
#endif
	//thread
    void run(void) ;

	
public slots:
	void updatePicture(cv::Mat image_top, cv::Mat image_bottom);	
	void updatePicture_Top(cv::Mat image);
	void updatePicture_Bottom(cv::Mat image);

	void updateObjectPicture(cv::Mat image_top, cv::Mat image_bottom);
	void updateObjectPicture_Top(cv::Mat image);
	void updateObjectPicture_Bottom(cv::Mat image);

	void updateNetwork(bool b_con_top, bool b_con_bottom);
	void updateNetwork_Top(bool b_con);
	void updateNetwork_Bottom(bool b_con);

	void updateJobInfo(QString qstr_info_top, QString qstr_info_bottom);
	void updateJobInfo_Top(QString qstr_info);
	void updateJobInfo_Bottom(QString qstr_info);

	//button
	void OnButton_Config(void) ;
	void OnButton_Config_New(void) ;
	void OnButton_Config_Save(void) ;
	void OnButton_Config_Load(void) ;

	void OnButton_Test_Run(void) ;
	void OnButton_Open_Log(void) ;

	void updateRunCrack(bool b_run_top, bool b_run_bottom) ;
	void updateRunCrack_Top(bool b_run) ;
	void updateRunCrack_Bottom(bool b_run) ;

	void updateRunColor(bool b_run_top, bool b_run_bottom) ;
	void updateRunColor_Top(bool b_run) ;
	void updateRunColor_Bottom(bool b_run) ;

	void updateLevelCrack(int level_top, int level_bottom) ;
	void updateLevelCrack_Top(int level) ;
	void updateLevelCrack_Bottom(int level) ;

	void updateLevelColor(int level_top, int level_bottom) ;
	void updateLevelColor_Top(int level) ;
	void updateLevelColor_Bottom(int level) ;

	void updateSensitivityColor(int level_top, int level_bottom) ;
	void updateSensitivityColor_Top(int level) ;
	void updateSensitivityColor_Bottom(int level) ;

	void updateQualityCrack(float quality_top, float quality_bottom) ;
	void updateQualityCrack_Top(float quality) ;
	void updateQualityCrack_Bottom(float quality) ;
	void updateQualityColor(float quality_top, float quality_bottom) ;
	void updateQualityColor_Top(float quality) ;
	void updateQualityColor_Bottom(float quality) ;

	void updateCountRun(int count_top, int count_bottom) ;
	void updateCountRun_Top(int count) ;
	void updateCountRun_Bottom(int count) ;
	void updateCountPass(int count_top, int count_bottom) ;
	void updateCountPass_Top(int count) ;
	void updateCountPass_Bottom(int count) ;
	void updateCountNg(int count_top, int count_bottom) ;
	void updateCountNg_Top(int count) ;
	void updateCountNg_Bottom(int count) ;
	void updateCountNgCrack(int count_top, int count_bottom) ;
	void updateCountNgCrack_Top(int count) ;
	void updateCountNgCrack_Bottom(int count) ;
	void updateCountNgColor(int count_top, int count_bottom) ;
	void updateCountNgColor_Top(int count) ;
	void updateCountNgColor_Bottom(int count) ;
	
	//slider Crack
	void OnSliderSetTopLevel(void) ;
    void OnSliderTopMove(int value) ;
	//slider Crack
	void OnSliderSetBottomLevel(void) ;
    void OnSliderBottomMove(int value) ;

	//slider Color : Sensitivity
	void OnSliderSetTopSensitivity_Color(void) ;
    void OnSliderTopSensitivityMove_Color(int value) ;
	//slider Color : Sensitivity
	void OnSliderSetBottomSensitivity_Color(void) ;
    void OnSliderBottomSensitivityMove_Color(int value) ;
	//slider Color : Check Level
	void OnSliderSetTopLevel_Color(void) ;
	void OnSliderTopLevelMove_Color(int value) ;
	//slider Color : Check Level
	void OnSliderSetBottomLevel_Color(void) ;
	void OnSliderBottomLevelMove_Color(int value) ;

};

#endif // MAINWINDOW_H
