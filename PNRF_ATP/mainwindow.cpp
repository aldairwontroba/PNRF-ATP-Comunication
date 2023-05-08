
#include "mainwindow.h"
#include "ui_mainwindow.h"


//#ifdef __BORLANDC__
//#pragma hdrstop
//#endif


double dStart, dEnd; // start and stop time

bool ATP_SIM = false;
bool ATP_ISCONECTED = false;
bool FILE_ISLOAD = false;

long StartPosition = 0;
long lenDATA = 0;
long count_data = 0;

double ZeroCrossFILETime = 0;

double* DATA_TIME = NULL;
double* DATA_VOLTAGE = NULL;
double* DATA_CURRENT = NULL;

TCHAR MapObjATP_Value[] = TEXT("ATP_Value_Mapping_reader");
TCHAR MapObjATP_WorkEvent_reader[] = TEXT("WorkEvent_reader");
TCHAR MapObjATP_BackEvent_reader[] = TEXT("BackEvent_reader");

HANDLE workHandle;
HANDLE backHandle;
HANDLE hMapFile_Value;
LPCTSTR pBuf_Value;

struct ChannelsBufferValue
{
    double VOLTAGE;
    double CURRENT;
    double ATPtime;
    double ATPtimeStart;
    double ATPtimeStop;
    double ATPtimeStep;
    double TstartTime;
    double errorMsg;
};

ChannelsBufferValue ChannelsBufferATP;

/////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton_LoadFile, &QPushButton::clicked, this, &MainWindow::on_actionLoad_file_triggered);
    connect(ui->pushButton_Connect_ATP, &QPushButton::clicked, this, &MainWindow::on_actionConnect_triggered);
    connect(ui->pushButton_ATP_Run, &QPushButton::clicked, this, &MainWindow::on_actionStart_triggered);

    ///////////////////////////////////////////////////

    restoreData();

}
MainWindow::~MainWindow()
{
    // Salvando o estado atual da janela
    QSettings settings("myapp.ini", QSettings::IniFormat);
    settings.beginGroup("MainWindow");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.setValue("file", file);
    settings.setValue("Mult", multiplicador);
    settings.setValue("Freq", frequencia);
    settings.setValue("showVoltage", showV);
    settings.setValue("showCurrent", showC);
    settings.endGroup();

    delete ui;
}
void MainWindow::restoreData()
{
    // Configurações do arquivo myapp.ini
    QCoreApplication::setOrganizationName("AWI");
    QCoreApplication::setApplicationName("Pnrf ATP");

    // Restaurando o estado anterior da janela
    QSettings settings("myapp.ini", QSettings::IniFormat);
    settings.beginGroup("MainWindow");

    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (!geometry.isEmpty()) {
        restoreGeometry(geometry);
    }
    const QByteArray state = settings.value("windowState", QByteArray()).toByteArray();
    if (!state.isEmpty()) {
        restoreState(state);
    }
    file = settings.value("file", QString()).toString();
    if (settings.contains("Mult")) {
        multiplicador = settings.value("Mult", 0).toDouble();
        frequencia = settings.value("Freq", 0).toDouble();
    }else{
        multiplicador = 1;
        frequencia = 60;
    }
    if (settings.contains("showVoltage")) {
        showV = settings.value("showVoltage", 0).toBool();
        showC = settings.value("showCurrent", 0).toBool();
    }else{
        showV = false;
        showC = true;
    }

    settings.endGroup();
}
void MainWindow::on_actionShow_Graphics_triggered()
{
    if (FILE_ISLOAD)
    {
        setGrafics(ui->customPlot);
        ui->textEdit->append("Grafico atualizado!");
    }
    else
    {
        ui->textEdit->append("Arquivo não carregado!");
    }
}
void MainWindow::setGrafics(QCustomPlot *customPlot)
{
    // Adicionar gráfico para corrente
    customPlot->addGraph(customPlot->xAxis, customPlot->yAxis);

    // Adicionar gráfico para tensão
    QCPAxisRect *axisRect = customPlot->axisRect();
    QCPAxis *axisVoltage = new QCPAxis(axisRect, QCPAxis::atRight);
    axisRect->addAxis(QCPAxis::atRight, axisVoltage);
    customPlot->addGraph(customPlot->xAxis, axisVoltage);

    // Configurar gráfico da corrente
    customPlot->graph(0)->setData(dataVectorT, dataVectorC);
    customPlot->graph(0)->setPen(QPen(Qt::blue));
    customPlot->graph(0)->setName("Current");
    customPlot->graph(0)->setVisible(showC);

    // Configurar gráfico da tensão
    customPlot->graph(1)->setData(dataVectorT, dataVectorV);
    customPlot->graph(1)->setPen(QPen(Qt::red));
    customPlot->graph(1)->setName("Voltage");
    customPlot->graph(1)->setVisible(showV);

    // Configurar eixos
    customPlot->xAxis->setLabel("Time (s)");
    customPlot->yAxis->setLabel("Current (A)");
    axisVoltage->setLabel("Voltage (V)");

    // Replotar o gráfico
    customPlot->rescaleAxes();

    // make left and bottom axes always transfer their ranges to right and top axes:
//    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
//    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:

    // Note: we could have also just called customPlot->rescaleAxes(); instead
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    customPlot->setInteractions(QCP::iRangeZoom | QCP::iSelectPlottables | QCP::iRangeDrag);
    customPlot->setSelectionRectMode(QCP::SelectionRectMode::srmZoom);

    customPlot->replot();

    ///////////////////////////////////////////////////

    // Definir a política de exibição do menu de contexto
    customPlot->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(customPlot, &QCustomPlot::customContextMenuRequested, this, &MainWindow::contextMenuRequest);
}
void MainWindow::contextMenuRequest(QPoint pos)
{
    if (contextMenu == nullptr)
    {
        // Criar o menu de contexto e as ações
        contextMenu = new QMenu(this);

        onZoomOut = new QAction("Unzoom", this);
        connect(onZoomOut, &QAction::triggered, this, &MainWindow::unZoom);

        showVoltageAction = new QAction("Show Voltage", this);
        showVoltageAction->setCheckable(true);
        showVoltageAction->setChecked(showV);
        connect(showVoltageAction, &QAction::toggled, this, &MainWindow::showVoltage);

        showCurrentAction = new QAction("Show Current", this);
        showCurrentAction->setCheckable(true);
        showCurrentAction->setChecked(showC);
        connect(showCurrentAction, &QAction::toggled, this, &MainWindow::showCurrent);

        // Adicionar as ações ao menu
        contextMenu->addAction(onZoomOut);
        contextMenu->addSeparator();
        contextMenu->addAction(showVoltageAction);
        contextMenu->addAction(showCurrentAction);
    }

    // Exibir o menu de contexto
    contextMenu->popup(ui->customPlot->mapToGlobal(pos));
}
void MainWindow::unZoom()
{
    ui->customPlot->rescaleAxes();
    ui->customPlot->replot();
}
void MainWindow::showVoltage()
{
    if (showVoltageAction->isChecked())
    {
        showV = true;
        ui->customPlot->graph(1)->setVisible(true);
        ui->customPlot->replot();
    }else{
        showV = false;
        ui->customPlot->graph(1)->setVisible(false);
        ui->customPlot->replot();
    }
}
void MainWindow::showCurrent()
{
    if (showCurrentAction->isChecked())
    {
        showC = true;
        ui->customPlot->graph(0)->setVisible(true);
        ui->customPlot->replot();
    }else{
        showC = false;
        ui->customPlot->graph(0)->setVisible(false);
        ui->customPlot->replot();
    }

}
/////////////////////////////////////////////////////////////////////////////
void MainWindow::on_actionLoad_file_triggered()
{
    if (FILE_ISLOAD) MainWindow::on_actionClear_file_triggered();


    QFileDialog fileDialog(this);
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setDirectory(file);

    if (fileDialog.exec() == QDialog::Accepted) {
        file = fileDialog.selectedFiles().first();
        // continue with selected file path
        ui->textEdit->append("Arquivo selecionado:");
        ui->textEdit->append(file);
        QFileInfo fileInfo(file);
        if (fileInfo.suffix() != "pnrf") {
            ui->textEdit->append("Arquivo não é um .pnrf");
            return;
        }
    }else{
        return;
    }

    CoInitialize(NULL);

    IRecordingLoaderPtr itfLoader;
    itfLoader.CreateInstance(__uuidof(PNRFLoader));

    // Enter the name of the pnrf recording here "D:\\FAI_DATA\\Gen3i data files\\VT892.pnrf"
    BSTR mybstr = SysAllocString(reinterpret_cast<const wchar_t*>(file.utf16()));

    IRecordingPtr itfRecording = itfLoader->LoadRecording(mybstr);

    // print recording name
    ui->textEdit->append(QString("Titulo do arquivo: %1").arg((wchar_t*)(itfRecording->Title)));

    int countchannels = itfRecording->Channels->Count;
    ui->textEdit->append(QString("Numero de Canais: %1").arg(countchannels));

    for (int j = 1; j <= countchannels; j++)
    {
        ui->textEdit->append(QString("Nome do Canal %1: %2").arg(j).arg((wchar_t*)itfRecording->Channels->Item[j]->Name));
    }

    // connect to data source channel 1 and 3
    IDataSrcPtr MySourceVoltage = itfRecording->Channels->Item[1]->DataSource[DataSourceSelect_Continuous];
    IDataSrcPtr MySourceCurrent = itfRecording->Channels->Item[3]->DataSource[DataSourceSelect_Continuous];
    //IDataSrcPtr MySourceCurrent = itfRecording->Channels->Item[3]->DataSource[DataSourceSelect_Mixed];

    // fetch start and stop time
    MySourceVoltage->Sweeps->get_StartTime(&dStart);
    MySourceVoltage->Sweeps->get_EndTime(&dEnd);
    ui->textEdit->append(QString("Start time: %1 s, End time: %2 s").arg(QString::number(dStart, 'f', 2))
                                                                    .arg(QString::number(dEnd, 'f', 2)));
    CComVariant myDataVoltage;
    CComVariant myDataCurrent;
    // Get data between start and stop time
    MySourceVoltage->Data(dStart, dEnd, &myDataVoltage);
    MySourceCurrent->Data(dStart, dEnd, &myDataCurrent);

    // if object is empty: quit
    if (myDataVoltage.vt == VT_EMPTY || myDataCurrent.vt == VT_EMPTY)
    {
        ui->textEdit->append(("No Data"));
        return;
    }

    // create segments pointer
    IDataSegmentsPtr itfSegmentsVoltage = myDataVoltage.punkVal;
    IDataSegmentsPtr itfSegmentsCurrent = myDataCurrent.punkVal;

    // pointer inside segment data
    IDataSegmentPtr itfSegmentVoltage = NULL;
    IDataSegmentPtr itfSegmentCurrent = NULL;
    itfSegmentsVoltage->get_Item(1, &itfSegmentVoltage);
    itfSegmentsCurrent->get_Item(1, &itfSegmentCurrent);

    lenDATA = itfSegmentVoltage->NumberOfSamples;
    double amostragem = itfSegmentVoltage->SampleInterval;

    int iCount = itfSegmentsVoltage->Count;	// segemnt count

    ui->textEdit->append(QString("Numero de count: %1").arg(iCount));
    ui->textEdit->append(QString("DATA SIZE: %1").arg(lenDATA));

    DATA_TIME = (double*)malloc(lenDATA * sizeof(double));
    DATA_VOLTAGE = (double*)malloc(lenDATA * sizeof(double));
    DATA_CURRENT = (double*)malloc(lenDATA * sizeof(double));

    ///////////////////////
    // variant data array for segment data
    CComVariant varDataVoltage;
    CComVariant varDataCurrent;
    // fetch data
    itfSegmentVoltage->Waveform(DataSourceResultType_Double64, 1, lenDATA, 1, &varDataVoltage);
    itfSegmentCurrent->Waveform(DataSourceResultType_Double64, 1, lenDATA, 1, &varDataCurrent);

    //Get data out through the use of the safe array and store locally
    SAFEARRAY* satmpV = NULL;
    SAFEARRAY* satmpC = NULL;
    satmpV = varDataVoltage.parray;
    satmpC = varDataCurrent.parray;

    double* pDataV;
    double* pDataC;

    SafeArrayAccessData(satmpV, (void**)&pDataV);
    SafeArrayAccessData(satmpC, (void**)&pDataC);

    double X0 = itfSegmentVoltage->StartTime;

    int i = 0;
    for (i = 0; i < lenDATA; i++)
    {
        DATA_TIME[i] = (X0 + i * amostragem)*50/frequencia;
        DATA_VOLTAGE[i] = pDataV[i];
        DATA_CURRENT[i] = pDataC[i]*multiplicador;


        dataVectorT.append(DATA_TIME[i]);
        dataVectorC.append(DATA_CURRENT[i]);
        dataVectorV.append(DATA_VOLTAGE[i]);

    }

    SafeArrayUnaccessData(satmpV);
    SafeArrayUnaccessData(satmpC);

    ui->textEdit->append(QString("Total de elementos no DATA: %1").arg(i));
    ui->textEdit->append("Arquivo carregado!");


    FILE_ISLOAD = true;

    int test = LocalizaInicio();
    if (test == -1)
    {
        ui->textEdit->append("Por favor carregue o arquivo primeiro!");
        return;
    }
    if (test == -2)
    {
        ui->textEdit->append("Arquivo .pnrf invalido!");
        return;
    }

    CoUninitialize();

    return on_actionShow_Graphics_triggered();
}
int MainWindow::LocalizaInicio()
{
    if (FILE_ISLOAD)
    {
        StartPosition = 0;

        for (int i = 0; i < lenDATA; i++)
        {
            if (DATA_VOLTAGE[i] > 10000)
            {
                for (int j = i; j < lenDATA; j++)
                {
                    if ( (DATA_VOLTAGE[j] >= 0) && (DATA_VOLTAGE[j - 1] < 0) )
                    {
                        StartPosition = j;
                        ZeroCrossFILETime = DATA_TIME[j];
                        ui->textEdit->append(QString("StartPosition encontrado: %1").arg(StartPosition));
                        ui->textEdit->append(QString("ZeroCrossFILETime encontrado: %1").arg(ZeroCrossFILETime));
                        ui->textEdit->append(QString("Tempo do ATP: %1").arg(dEnd-ZeroCrossFILETime-dStart));
                        return 0;
                    }
                }
            }
        }
        return -2;
    }
    else
    {
        ui->textEdit->append("O arquivo não foi carregado!");
        return -1;
    }
}
void MainWindow::on_actionClear_file_triggered()
{
    // Reseta variaveis
    StartPosition = 0;
    lenDATA = 0;
    ZeroCrossFILETime = 0;

    free(DATA_TIME);
    free(DATA_CURRENT);
    free(DATA_VOLTAGE);


    dataVectorT.clear();
    dataVectorC.clear();
    dataVectorV.clear();

    FILE_ISLOAD = false;

    ui->textEdit->append("\nArquivo deletado!\n\n");

}
void MainWindow::on_actionConnect_triggered()
{
    if (ATP_ISCONECTED)
    {
        ui->textEdit->append("O ATP já está conectado!");
        return;
    }
    hMapFile_Value = OpenFileMapping
    (
        FILE_MAP_ALL_ACCESS, //Read,write access
        FALSE, //do not inherit the name
        MapObjATP_Value //name of mapping object
    );
    if (hMapFile_Value == NULL) {
        ui->textEdit->append(QString("Could not open file mapping object: %1").arg(GetLastError()));
        return;
    }
    pBuf_Value = (LPTSTR)MapViewOfFile
    (
        hMapFile_Value,
        FILE_MAP_ALL_ACCESS,
        0,
        0,
        32
    );
    if (pBuf_Value == NULL) {
        ui->textEdit->append(QString("Could not map view of file: %d\n").arg(GetLastError()));
        return;
    }
    ui->textEdit->append("File mapping VALUE conectado!");

    ui->textEdit->append("ATP conectado!");


    workHandle = CreateEvent(NULL, TRUE, FALSE, MapObjATP_WorkEvent_reader); // Manually. no signal
    if (workHandle == NULL)
    {
        ui->textEdit->append(QString("CreateEvent failed %d\n").arg(GetLastError()));
    }
    backHandle = CreateEvent(NULL, TRUE, FALSE, MapObjATP_BackEvent_reader); // Manually. no signal
    if (backHandle == NULL)
    {
        ui->textEdit->append(QString("CreateEvent failed %d\n").arg(GetLastError()));
    }

    CopyMemory(&ChannelsBufferATP, (PVOID)pBuf_Value, sizeof(ChannelsBufferATP));

    ChannelsBufferATP.CURRENT = 0;

    CopyMemory((PVOID)pBuf_Value, &ChannelsBufferATP, sizeof(ChannelsBufferATP));

    if (!SetEvent(backHandle))
    {
        ui->textEdit->append("backHandle failed!");
    }

    ATP_ISCONECTED = true;
}
void MainWindow::on_actionStart_triggered()
{
    if (FILE_ISLOAD)
    {
        if (ATP_ISCONECTED)
        {
            ATP_SIM = true;
            ui->textEdit->append("Simulação iniciada");
            ui->textEdit->append(QString("Step time: %1 segundos \nEnd time: %2 segundos").arg(ChannelsBufferATP.ATPtimeStep)
                                                                           .arg(ChannelsBufferATP.ATPtimeStop));

            WorkerAtpSim = new atpSimulationTr(ui, this);
            connect(WorkerAtpSim, &atpSimulationTr::finished, WorkerAtpSim, &atpSimulationTr::deleteLater);
            WorkerAtpSim->start();

        }
        else
        {
            ui->textEdit->append("ATP não conectado!");
            ATP_SIM = false;
        }
    }
    else
    {
        ui->textEdit->append("Arquivo .pnrf não esta carregado!");
    }
}
void MainWindow::on_actionSettings_triggered()
{
    QGridLayout *gridLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QLabel *label;
    QLabel *label_2;

    janela = new QDialog(this);

    janela->setObjectName("JanelaConfig");
    janela->setWindowTitle("Config");
    janela->resize(366, 183);

    gridLayout = new QGridLayout(janela);

    pushButton = new QPushButton(janela);
    pushButton->setText("OK");

    gridLayout->addWidget(pushButton, 2, 0, 1, 1);

    pushButton_2 = new QPushButton(janela);
    pushButton_2->setText("Cancelar");

    gridLayout->addWidget(pushButton_2, 2, 1, 1, 1);

    label = new QLabel(janela);
    label->setText("Frequencia:");

    gridLayout->addWidget(label, 0, 0, 1, 1);

    label_2 = new QLabel(janela);
    label_2->setText("Multiplicador da Corrente:");

    gridLayout->addWidget(label_2, 1, 0, 1, 1);

    doubleSpinBox = new QDoubleSpinBox(janela);
    doubleSpinBox->setValue(frequencia);

    gridLayout->addWidget(doubleSpinBox, 0, 1, 1, 1);

    doubleSpinBox_2 = new QDoubleSpinBox(janela);
    doubleSpinBox_2->setValue(multiplicador);

    gridLayout->addWidget(doubleSpinBox_2, 1, 1, 1, 1);

    connect(pushButton, &QPushButton::clicked, this, &MainWindow::configok);
    connect(pushButton_2, &QPushButton::clicked, this, &MainWindow::configcancel);

    janela->show();
}
void MainWindow::configok()
{
    frequencia = doubleSpinBox->value();
    multiplicador = doubleSpinBox_2->value();
    janela->close();
}
void MainWindow::configcancel()
{
   janela->close();
}
/////////////////////////////////////////////////////////////////////////////
atpSimulationTr::atpSimulationTr(Ui::MainWindow *uiptr, QObject *parent) :QThread(parent)
{
    if (static_cast<Ui::MainWindow *>(uiptr) != nullptr) ui = uiptr;

    ui->textEdit->append("Simulando...");
}
void atpSimulationTr::run()
{
    double LastVoltage = 0.1;
    double VoltageMedio = 0;
    double LastVoltageMedio;
    double LastTime = 0.0;
    double a, b, c, d;
    double ZeroCrossATPTime = 0;
    double aux;
    bool FirtATPzeroCross = true;
    int countciclo = StartPosition;

    while (ATP_SIM)
    {
        DWORD timeout;
        timeout  = WaitForSingleObject(workHandle, 3000);
        if (timeout == WAIT_TIMEOUT)
        {
            ATP_SIM = false;
            ui->textEdit->append(QString("WAIT_TIMEOUT Failed %1").arg(GetLastError()));
            return;
        }
        if (!ResetEvent(workHandle))
        {
            ui->textEdit->append(QString("ResetEvent Failed: %1").arg(GetLastError()));
            return;
        }

        CopyMemory(&ChannelsBufferATP, (PVOID)pBuf_Value, sizeof(ChannelsBufferATP));

        LastVoltageMedio = VoltageMedio;
        VoltageMedio = (ChannelsBufferATP.VOLTAGE + LastVoltage)/2;

        ////////

        if (ChannelsBufferATP.ATPtime >= ChannelsBufferATP.TstartTime)
        {
            if (FirtATPzeroCross)
            {
                if (VoltageMedio > 0 && LastVoltageMedio <= 0)
                {
                    a = VoltageMedio;
                    b = LastVoltageMedio;
                    c = ChannelsBufferATP.ATPtime;
                    d = LastTime;

                    ZeroCrossATPTime = c - (a / (a - b)) * (c - d);

                    FirtATPzeroCross = false;
                    ui->textEdit->append(QString("ZeroCrossATPTime encontrado: %1").arg(ZeroCrossATPTime));
                }
                ChannelsBufferATP.CURRENT = 0;
                LastVoltage = ChannelsBufferATP.VOLTAGE;
                LastTime = ChannelsBufferATP.ATPtime;
            }
            else
            {

                if (VoltageMedio > 0 && LastVoltageMedio <= 0)
                {
                    a = VoltageMedio;
                    b = LastVoltageMedio;
                    c = ChannelsBufferATP.ATPtime;
                    d = LastTime;

                    ZeroCrossATPTime = c - (a / (a - b)) * (c - d);

                    if (int k = ZeroCrossFileLocate() == -1) countciclo = lenDATA;
                    else countciclo = k;
                }

                LastVoltage = ChannelsBufferATP.VOLTAGE;
                LastTime = ChannelsBufferATP.ATPtime;

                double temp1 = ChannelsBufferATP.ATPtime - ZeroCrossATPTime;

                if (countciclo >= lenDATA)
                {
                    ChannelsBufferATP.CURRENT = 0;
                }
                else
                {
                    countciclo--;

                    while (!((countciclo >= lenDATA - 1) || ((DATA_TIME[countciclo] - ZeroCrossFILETime) > temp1 && temp1 >= (DATA_TIME[countciclo - 1] - ZeroCrossFILETime))))
                    {
                        countciclo++;
                    }

                    a = DATA_TIME[countciclo] - ZeroCrossFILETime;
                    b = DATA_TIME[countciclo - 1] - ZeroCrossFILETime;
                    c = DATA_CURRENT[countciclo];
                    d = DATA_CURRENT[countciclo - 1];

                    aux = c - ((a - temp1) / (a - b)) * (c - d);

                    if (countciclo < lenDATA) ChannelsBufferATP.CURRENT = aux;
                }
            }
        }

        ////////

        CopyMemory((PVOID)pBuf_Value, &ChannelsBufferATP, sizeof(ChannelsBufferATP));

        if (!SetEvent(backHandle))
        {
            ui->textEdit->append("backHandle failed!");
        }

        if (ChannelsBufferATP.ATPtime >= ChannelsBufferATP.ATPtimeStop - (ChannelsBufferATP.ATPtimeStep / 2))
        {
            ui->textEdit->append("Simulação terminada!");
            ui->textEdit->append("ATP desconectado!");
            ATP_ISCONECTED = false;
            ATP_SIM = false;
        }
    }

}
int atpSimulationTr::ZeroCrossFileLocate()
{
    double a, b, c, d;

    for (int j = StartPosition + 1; j < lenDATA; j++)
    {
        if ((DATA_VOLTAGE[j] > 0) && (DATA_VOLTAGE[j - 1] <= 0))
        {

            a = DATA_VOLTAGE[j];
            b = DATA_VOLTAGE[j-1];
            c = DATA_TIME[j];
            d = DATA_TIME[j-1];

            ZeroCrossFILETime = c - (a / (a - b)) * (c - d);

            StartPosition = j;

            return j;
        }
    }
    StartPosition = lenDATA;
    return -1;
}






