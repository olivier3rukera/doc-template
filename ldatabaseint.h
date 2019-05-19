#ifndef TEMPDATABASE_H
#define TEMPDATBASE_H

class Controller;
#include "dtmantype.h"
#include <QtSql>

class TempDataBase : public QObject
{
    friend class Controller;
    Q_OBJECT
    public:
    explicit TempDataBase(QObject *parent = nullptr);
    bool openDataBase(QString);
    bool saveTemplateSource(QString name,QString source,QString html="NULL",QString variables="NULL");
    Template* searchTemplate(QString name);
    QStringList getAllTemplateNames();
    bool editDocumentTemplate(QString id,QString name,QString sourceCode,QString,QString);

   signals:

   public slots:
   private:
       QSqlDatabase m_mainDb;
       QSqlQuery m_query;
       QString m_dbError;
};

#endif // TEMPDATABASE_H
