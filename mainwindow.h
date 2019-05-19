#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "texteditor.h"
#include "controller.h"
#include "dtmanwidget.h"
class MainWin : public QMainWindow
{
    Q_OBJECT

    friend class Controller;

    public:
        MainWin(QWidget *parent = 0);

        void createMenuBar();
        void createToolsBar();
        void createDock();
        void createStatusBar();
        void createCentralWidget();
        void displayWidgets(QVector<ProxyWidget*>);
        ~MainWin();

    private:

        Controller m_controller;
        TemplateForm *m_tempForm;

        QMenuBar *m_menuBar;
        QToolBar *m_toolBar;

        QMenu *m_aboutMenu,*m_editMenu;
        QMenu *m_import;
        QStatusBar *m_statusBar;

        ProxyWidget *m_searchPrintProxy;
        ProxyWidget *m_errorLabelProxy;
        ProxyWidget *m_tempFormProxy;
        ProxyWidget *m_editTemplateProxy;
        ProxyWidget *m_importTemplateProxy;

        QLabel *m_errorLabel;


        QWidget *m_centralWidget;
        QVBoxLayout *m_centralLayout;

           // SearchAndPrint GroupBox
        QGroupBox *m_searchAndPrintGroupBox;
        QLabel *m_searchDocLabel;
        QLineEdit *m_searchDocEdit;
        QTextBrowser *m_previewBrowser;
        QPushButton *m_printButton,*m_saveAsPdfButton,*m_previewDocButton, *m_editTempButton;

        //  CreateNewGroupBox GroupBox

        QGroupBox *m_editTemplateGroupBox;

        QLabel *m_nameOfNewDocLabel;
        QLineEdit *m_nameOfNewDocEdit;
        QPushButton *m_saveNewDocButton, *m_continueTempEditingButton,*m_previewTemplateButton,*m_cancelButton;
        TextEditor *m_textEditor;
        QTextBrowser *m_templateBrowser;

        QGroupBox *m_importTemplateGroupBox;
        QLineEdit *m_chooseFileEdit;
        QPushButton *m_chooseFileButton;
        QLabel *m_importedFormatLabel;
        QComboBox *m_importedFormatCombo;
        QLabel *m_importedNameLabel;
        QLineEdit *m_importedNameEdit;
        QPushButton *m_startImportingButton;

        QGraphicsScene m_graphicScene;
        QGraphicsView m_graphicsView;
        QString m_style;

        int m_height;
        int m_width;

        QVector<ProxyWidget*> m_displayedWidgetVector;
        QVector<ProxyWidget*> m_proxyWidgetVector;




};

#endif // MAINWINDOW_H
