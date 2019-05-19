#include "mainwindow.h"

MainWin::MainWin(QWidget *parent)
    :QMainWindow(parent),m_controller(this)
{
    this->setWindowState(Qt::WindowMaximized);
    this->createToolsBar();
    m_height=QApplication::desktop()->availableGeometry().height();
    m_width=QApplication::desktop()->availableGeometry().width();
    m_height=m_height*0.90;
    m_width=m_width*0.99;

    QFile cssFile("D:/Projets/DocTemplate/DocTemplate/mainStyle.css");
    cssFile.open(QIODevice::ReadOnly|QIODevice::Text);
    QString style(cssFile.readAll());
    m_style=style;
    this->setStyleSheet(style);
    this->setCentralWidget(&m_graphicsView);
    this->createMenuBar();
    this->createCentralWidget();
    m_statusBar=this->statusBar();
    m_controller.initParameters();
    m_controller.initTempDataBase();
    m_controller.refreshNameTemplateCompletion(true);
}


void MainWin::createMenuBar()
{
    m_menuBar=this->menuBar();
    QPixmap pix(":/images/background.jpg");
    //pix=pix.scaled(m_menuBar->size(),Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background,pix);
    m_menuBar->setPalette(palette);

    m_editMenu=m_menuBar->addMenu(tr("Edit"));
    QAction *newDocTemplate=m_editMenu->addAction(tr("New Document Template"));
    connect(newDocTemplate,SIGNAL(triggered(bool)),&m_controller,SLOT(createNewTemplate()));

    m_import=m_menuBar->addMenu(tr("Import"));
    QAction *importExistingFile=m_import->addAction(tr("Import Existing File"));
    connect(importExistingFile,SIGNAL(triggered(bool)),&m_controller,SLOT(importExistingTemplate()));

}


void MainWin::createToolsBar()
{
    m_toolBar=this->addToolBar(tr("Tools"));

    QIcon homeIcon(":/images/tools/home.png");
    QAction *homeAction=new QAction(homeIcon,tr("Home"),this);
    homeAction->setStatusTip("Home");
    m_toolBar->addAction(homeAction);
    connect(homeAction,SIGNAL(triggered(bool)),&m_controller,SLOT(home()));

}


void MainWin::createDock()
{

}


void MainWin::createStatusBar()
{

}


void MainWin::createCentralWidget()
{
    m_graphicsView.setScene(&m_graphicScene);
    m_graphicScene.setSceneRect(0,0,m_width,m_height);
    m_graphicsView.setAlignment(Qt::AlignLeft|Qt::AlignTop);
    m_graphicsView.setBackgroundBrush(QPixmap(":/images/background/mercury.jpg"));


    m_errorLabel=new QLabel();
    m_errorLabel->setObjectName("errorLabel");
    m_errorLabel->setAlignment(Qt::AlignHCenter);
    m_errorLabel->setStyleSheet(m_style);

    m_errorLabelProxy=new ProxyWidget();
    m_proxyWidgetVector.push_back(m_errorLabelProxy);
    m_graphicScene.addItem(m_errorLabelProxy);
    m_errorLabelProxy->setWidget(m_errorLabel);
    m_errorLabel->setVisible(false);

    m_tempForm=new TemplateForm();
    m_tempForm->setStyleSheet(m_style);
    m_tempFormProxy=new ProxyWidget();
    m_proxyWidgetVector.push_back(m_tempFormProxy);
    m_graphicScene.addItem(m_tempFormProxy);
    m_tempFormProxy->setWidget(m_tempForm);
    m_tempForm->setVisible(false);

    QTransform matrixForm;
    matrixForm.rotate(-15,Qt::YAxis);
    m_tempFormProxy->setTransform(matrixForm,true);

    QGridLayout *searchAndPrintLayout=new QGridLayout();
    m_searchAndPrintGroupBox=new QGroupBox();
    m_searchAndPrintGroupBox->setLayout(searchAndPrintLayout);

    m_searchDocLabel=new QLabel(tr("Name Of The Document"));
    m_searchDocLabel->setAlignment(Qt::AlignRight);
    m_searchDocEdit= new QLineEdit();

    m_previewBrowser=new QTextBrowser();
    //m_previewBrowser->setFixedSize(420,594);
    m_printButton=new QPushButton(tr("Print Document"));
    connect(m_searchDocEdit,SIGNAL(returnPressed()),&m_controller,SLOT(searchDocument()));
    m_saveAsPdfButton=new QPushButton(tr("Save As Pdf"));
    m_previewDocButton=new QPushButton(tr("Preview Document"));
    connect(m_previewDocButton,SIGNAL(clicked(bool)),&m_controller,SLOT(previewDocument()));
    m_editTempButton=new QPushButton(tr("Edit Document"));
    connect(m_editTempButton,SIGNAL(clicked(bool)),&m_controller,SLOT(editDocumentTemplate()));
    

    searchAndPrintLayout->addWidget(m_searchDocLabel,0,1,1,1);
    searchAndPrintLayout->addWidget(m_searchDocEdit,0,2,1,2);
    searchAndPrintLayout->addWidget(m_previewBrowser,1,1,1,2);
    searchAndPrintLayout->addWidget(m_printButton,2,0);
    searchAndPrintLayout->addWidget(m_saveAsPdfButton,2,1);
    searchAndPrintLayout->addWidget(m_previewDocButton,2,2);
    searchAndPrintLayout->addWidget(m_editTempButton,2,3);
    m_searchAndPrintGroupBox->setStyleSheet(m_style);


    m_searchPrintProxy=new ProxyWidget();
    m_proxyWidgetVector.push_back(m_searchPrintProxy);
    m_graphicScene.addItem(m_searchPrintProxy);
    m_searchPrintProxy->setWidget(m_searchAndPrintGroupBox);
    m_searchAndPrintGroupBox->setVisible(false);

    m_editTemplateGroupBox=new QGroupBox();
    m_editTemplateGroupBox->setVisible(false);
    QGridLayout *saveNewDocLayout=new QGridLayout();
    m_editTemplateGroupBox->setLayout(saveNewDocLayout);
    m_editTemplateGroupBox->setStyleSheet(m_style);

    m_nameOfNewDocLabel=new QLabel(tr("Name Of The New Document"));
    m_nameOfNewDocEdit=new QLineEdit();
    m_textEditor=new TextEditor();
    m_templateBrowser=new QTextBrowser();
    m_templateBrowser->setVisible(false);
    m_saveNewDocButton=new QPushButton(tr("Save"));
    connect(m_saveNewDocButton,SIGNAL(clicked(bool)),&m_controller,SLOT(saveDocumentSource()));
    m_continueTempEditingButton=new QPushButton(tr("Continue Changes"));
    m_continueTempEditingButton->setVisible(false);
    connect(m_continueTempEditingButton,SIGNAL(clicked(bool)),&m_controller,SLOT(continueTempEditing()));
    m_cancelButton=new QPushButton(tr("Cancel"));
    m_previewTemplateButton=new QPushButton(tr("Preview Template"));
    connect(m_previewTemplateButton,SIGNAL(clicked(bool)),&m_controller,SLOT(previewTemplate()));

    saveNewDocLayout->addWidget(m_nameOfNewDocLabel,0,0,1,2);
    saveNewDocLayout->addWidget(m_nameOfNewDocEdit,0,2,1,2);
    saveNewDocLayout->addWidget(m_textEditor,1,0,1,4);
    saveNewDocLayout->addWidget(m_templateBrowser,2,0,1,4);
    saveNewDocLayout->addWidget(m_saveNewDocButton,3,0);
    saveNewDocLayout->addWidget(m_continueTempEditingButton,3,1);
    saveNewDocLayout->addWidget(m_cancelButton,3,2);
    saveNewDocLayout->addWidget(m_previewTemplateButton,3,3);

    m_editTemplateProxy=new ProxyWidget();
    m_proxyWidgetVector.push_back(m_editTemplateProxy);
    m_graphicScene.addItem(m_editTemplateProxy);
    m_editTemplateProxy->setWidget(m_editTemplateGroupBox);
    m_editTemplateGroupBox->setVisible(false);

    m_importTemplateGroupBox=new QGroupBox();
    m_importTemplateGroupBox->setStyleSheet(m_style);
    QGridLayout *importTemplateLayout=new QGridLayout();
    m_importTemplateGroupBox->setLayout(importTemplateLayout);

    m_chooseFileEdit=new QLineEdit();
    m_chooseFileEdit->setEnabled(false);
    m_chooseFileButton=new QPushButton(tr("Choose File"));
    connect(m_chooseFileButton,SIGNAL(clicked(bool)),&m_controller,SLOT(selectFile()));
    m_importedFormatLabel=new QLabel(tr("Format of the File"));

    QStringList importedFormat;
    importedFormat<<"docx";
    m_importedFormatCombo=new QComboBox();
    m_importedFormatCombo->addItems(importedFormat);

    m_importedNameLabel=new QLabel(tr("Name Of the template"));
    m_importedNameEdit=new QLineEdit();
    m_startImportingButton=new QPushButton(tr("Start Importing"));
    connect(m_startImportingButton,SIGNAL(clicked(bool)),&m_controller,SLOT(startImporting()));

    importTemplateLayout->addWidget(m_chooseFileEdit,0,0,1,2);
    importTemplateLayout->addWidget(m_chooseFileButton,0,3,1,1);
    //importTemplateLayout->addWidget(m_importedFormatLabel,1,1);
    importTemplateLayout->addWidget(m_importedFormatCombo,0,2);
    importTemplateLayout->addWidget(m_importedNameLabel,2,0);
    importTemplateLayout->addWidget(m_importedNameEdit,2,1,1,2);
    importTemplateLayout->addWidget(m_startImportingButton,2,3);

    m_importTemplateProxy=new ProxyWidget();
    m_proxyWidgetVector.push_back(m_importTemplateProxy);
    m_graphicScene.addItem(m_importTemplateProxy);
    m_importTemplateProxy->setWidget(m_importTemplateGroupBox);
    m_importTemplateGroupBox->setVisible(false);

    m_searchPrintProxy->dtSetDisplayInfos(0,0,m_width,m_height);
    m_displayedWidgetVector.push_back(m_searchPrintProxy);

    displayWidgets(m_displayedWidgetVector);




}


MainWin::~MainWin()
{
}


void MainWin::displayWidgets(QVector<ProxyWidget *> displayedWidget)
{
    for(int i=0;i<m_proxyWidgetVector.size();++i)
    {
        m_proxyWidgetVector[i]->widget()->setVisible(false);
    }
    for(int i=0;i<displayedWidget.size();++i)
    {
        displayedWidget[i]->widget()->setVisible(true);
    }

}
