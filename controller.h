#ifndef CONTROLLER_H
#define CONTROLLER_H

class MainWin;
class ProxyWidget;

#include "ldatabaseint.h"
#include <QCompleter>

class Controller : public QObject
{
    Q_OBJECT
    public:
        explicit Controller(MainWin *mainWinDow);
        void initTempDataBase(QString path="D:/Projets/DocTemplate/Files/templateDb.db");
        void refreshNameTemplateCompletion(bool firstCall=false);
        void initParameters();

    signals:

    public slots:
        void saveDocumentSource();
        void searchDocument();
        void previewTemplate();
        void previewDocument();
        void editDocumentTemplate();
        void createNewTemplate();
        void continueTempEditing();
        QString getTemplateVariables(QString strText);
        void importExistingTemplate();
        bool selectFile();
        QString callPandoc(QStringList parameters,QString inputStr,bool imputFile=false);
        void startImporting();
        void showErrorMsg(QString);
        void home();
    public:
        TempDataBase m_templateDb;
    private:
        MainWin *mw;
        bool m_errorSatus;
        QCompleter *m_nameTemplateCompleter;
        QStringList m_listTemplateNames;
        Template* m_template;
        bool m_editingTemplate;
        int m_width;
        int m_height;
        QVector<ProxyWidget*> *m_displayedWidgets;
};

#endif // CONTROLLER_H
