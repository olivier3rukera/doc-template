#include "ldatabaseint.h"

TempDataBase::TempDataBase(QObject *parent) : QObject(parent)
{

}


bool TempDataBase::editDocumentTemplate(QString id,QString name,QString sourceCode,QString html
                                        ,QString tempVariables)
{
    name="\""+name+"\"";
    sourceCode="\""+sourceCode+"\"";
    html="\""+html+"\"";
    tempVariables="\""+tempVariables+"\"";
    QString insert="UPDATE templateTable SET templateName="+name+",templateSourceCode="+sourceCode;
    insert+=",templateVariables="+tempVariables+",templateHtml="+html;
    insert=insert+" WHERE templateId="+id;
    bool success=m_query.exec(insert);
    m_dbError=m_query.lastError().text();
    return success;
    
}


QStringList TempDataBase::getAllTemplateNames()
{
    QStringList nameList;
    bool success=m_query.exec("SELECT templateName FROM templateTable");
    m_dbError=m_query.lastError().text();
    if(!success)
    {
        return nameList;
    }
    while(m_query.next())
    {
        nameList<<m_query.value(0).toString();
    }
    return nameList;
}



bool TempDataBase::openDataBase(QString dbPath)
{
    m_mainDb=QSqlDatabase::addDatabase("QSQLITE","mainCnx");
    m_mainDb.setDatabaseName(dbPath);
    m_query=QSqlQuery(m_mainDb);
    return m_mainDb.open();
}


bool TempDataBase::saveTemplateSource(QString name, QString source,QString html,QString variables)
{
    bool success=false;
    m_query.prepare("INSERT INTO templateTable(templateName,templateSourceCode,templateHtml,templateVariables) values(?,?,?,?)");
    m_query.addBindValue(name);
    m_query.addBindValue(source);
    m_query.addBindValue(html);
    m_query.addBindValue(variables);
    success=m_query.exec();
    m_dbError=m_query.lastError().text();
    return success;
}


Template* TempDataBase::searchTemplate(QString name)
{
    Template *tempTemplate=new Template;
    name="\""+name+"\"";

    QString selectQuery="SELECT templateId,templateName,templateSourceCode,templateHtml,templateVariables FROM templateTable WHERE templateName="+name;
    m_query.exec(selectQuery);
    m_query.next();
    tempTemplate->id=m_query.value(0).toString();
    tempTemplate->name=m_query.value(1).toString();
    tempTemplate->sourceCode=m_query.value(2).toString();
    tempTemplate->html=m_query.value(3).toString();
    tempTemplate->variables=m_query.value(4).toString();

    return tempTemplate;

}
