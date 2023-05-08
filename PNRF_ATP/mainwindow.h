
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QMainWindow>
#include <QFileDialog>
#include <QThread>
#include <QVariant>

#include <Windows.h>
#include <comutil.h>
//#include <stdio.h>
//#include <conio.h>
//#include <iostream>
#include <atlcomcli.h>
#include <malloc.h>
//#include "stdafx.h"

#include "qcustomplot.h"

#import "percRecordingInterface.olb" no_namespace
//#import "libid:8098371E-98AD-0070-BEF3-21B9A51D6B3E" no_namespace
#import "percPNRFLoader.dll" no_namespace
//#import "libid:8098371E-98AD-0062-BEF3-21B9A51D6B3E" no_namespace

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class atpSimulationTr;
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setGrafics(QCustomPlot *customPlot);
    void restoreData();

private slots:
    void on_actionLoad_file_triggered();
    void on_actionClear_file_triggered();
    void on_actionConnect_triggered();
    void on_actionStart_triggered();

    int LocalizaInicio();

    void on_actionShow_Graphics_triggered();
    void contextMenuRequest(QPoint pos);
    void unZoom();
    void showVoltage();
    void showCurrent();

    void on_actionSettings_triggered();
    void configok();
    void configcancel();

private:
    Ui::MainWindow *ui;

    QDialog *janela;
    QDoubleSpinBox *doubleSpinBox;
    QDoubleSpinBox *doubleSpinBox_2;

    QString file;
    double multiplicador = 1;
    double frequencia = 60;
    bool showV = false;
    bool showC = true;

    atpSimulationTr *WorkerAtpSim;

    QMenu *contextMenu = nullptr;
    QAction *onZoomOut;
    QAction *showVoltageAction;
    QAction *showCurrentAction;

    //////////////////////////////////////////////////
    QVector<double> dataVectorT;
    QVector<double> dataVectorC;
    QVector<double> dataVectorV;
    //////////////////////////////////////////////////
};

class atpSimulationTr : public QThread
{
public:
    explicit atpSimulationTr(Ui::MainWindow *uiptr = nullptr, QObject *parent = nullptr);

private:
    void run() override;
    int ZeroCrossFileLocate();

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
