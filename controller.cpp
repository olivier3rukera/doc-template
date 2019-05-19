#include "controller.h"
#include "mainwindow.h"

Controller::Controller(MainWin *mainWindow) : QObject(mainWindow)
{
    m_templateDb.setParent(this);
    mw=mainWindow;
    m_errorSatus=false;
    m_editingTemplate=false;
}


QString Controller::callPandoc(QStringList parameters,QString inputStr,bool imputFile)
{
    QProcess *pandoc=new QProcess(this);
    QString path("D:/Projets/DocTemplate/Files/pandoc/pandoc.exe");
    if(imputFile)
    {
        parameters<<inputStr;

    }
    pandoc->start(path,parameters);
    pandoc->waitForStarted(10000);
    if(!imputFile)
    {
        pandoc->write(inputStr.toStdString().c_str());
        pandoc->closeWriteChannel();
    }
    pandoc->waitForFinished(10000);
    QString outputStr(pandoc->readAllStandardOutput());
    QString error(pandoc->readAllStandardError());
    delete pandoc;
    return outputStr;

}


void Controller::continueTempEditing()
{
    mw->m_templateBrowser->setVisible(false);
    mw->m_textEditor->setVisible(true);

}


void Controller::createNewTemplate()
{
    m_editingTemplate=false;
    mw->m_editTemplateProxy->dtSetDisplayInfos(0,0,m_width,m_height);
    m_displayedWidgets->clear();
    m_displayedWidgets->push_back(mw->m_editTemplateProxy);
    mw->displayWidgets(*(m_displayedWidgets));
}


void Controller::editDocumentTemplate()
{
    if(mw->m_searchDocEdit->text().isEmpty())
    {
        return;
    }
    else
    {
        m_editingTemplate=true;
        m_template=m_templateDb.searchTemplate(mw->m_searchDocEdit->text());
        mw->m_nameOfNewDocEdit->setText(m_template->name);
        mw->m_textEditor->setPlainText(m_template->sourceCode);
        m_displayedWidgets->clear();
        mw->m_editTemplateProxy->dtSetDisplayInfos(0,0,m_width,m_height);
        m_displayedWidgets->push_back(mw->m_editTemplateProxy);
        mw->displayWidgets(*m_displayedWidgets);
    }
}


QString Controller::getTemplateVariables(QString strText)
{
    QString variables;
    QRegularExpression varReg("\\{\\{[\\w\\s]+\\}\\}");
    QRegularExpressionMatchIterator regIt=varReg.globalMatch(strText);
    while(regIt.hasNext())
    {
        QRegularExpressionMatch regMatch=regIt.next();
        variables+=regMatch.captured(0);
        variables+=",";
    }
    variables.remove(variables.size()-1,1);
    variables.remove('{');
    variables.remove('}');
    return variables;

}


void Controller::home()
{
    m_displayedWidgets->clear();
    mw->m_searchPrintProxy->dtSetDisplayInfos(0,0,m_width,m_height);
    m_displayedWidgets->push_back(mw->m_searchPrintProxy);
    mw->displayWidgets(*m_displayedWidgets);
}


void Controller::importExistingTemplate()
{
    mw->m_importTemplateProxy->dtSetDisplayInfos(0,0,m_width,m_height);
    m_displayedWidgets->clear();
    m_displayedWidgets->push_back(mw->m_importTemplateProxy);
    mw->displayWidgets(*m_displayedWidgets);

}


void Controller::refreshNameTemplateCompletion(bool firstCall)
{
    m_listTemplateNames<<m_templateDb.getAllTemplateNames();
    if(firstCall)
    {
        m_nameTemplateCompleter=new QCompleter(m_listTemplateNames,this);
        m_nameTemplateCompleter->setCaseSensitivity(Qt::CaseInsensitive);
        mw->m_searchDocEdit->setCompleter(m_nameTemplateCompleter);
    }

}


void Controller::initParameters()
{
    m_width=mw->m_width;
    m_height=mw->m_height;
    m_displayedWidgets=&(mw->m_displayedWidgetVector);
}


void Controller::initTempDataBase(QString dbPath)
{
    if(m_templateDb.openDataBase(dbPath))
    {
        mw->m_statusBar->showMessage(tr("DataBase opened with success"),2000);
    }
    else
    {
        mw->m_statusBar->showMessage(tr("DataBase not opened"),2000);
    }
}


void Controller::previewDocument()
{
    if(!mw->m_tempForm->m_labelTab.isEmpty())
    {
        QString doc=m_template->html;
        for(int i=0;i<mw->m_tempForm->m_labelTab.size();++i)
        {
            QString field;
            field="{{"+mw->m_tempForm->m_labelTab[i]->text()+"}}";
            if(mw->m_tempForm->m_editTab[i]->text().isEmpty())
            {

                doc.replace(field,"************");
            }
            doc.replace(field,mw->m_tempForm->m_editTab[i]->text());

        }
        mw->m_previewBrowser->setText(doc);
    }
}


void Controller::previewTemplate()
{
    mw->m_tempForm->setVisible(false);    
    mw->m_continueTempEditingButton->setVisible(true);

    mw->m_templateBrowser->setVisible(true);
    mw->m_textEditor->setVisible(false);
    QStringList pandocArgument;
    pandocArgument<<"-f"<<"markdown"<<"-t"<<"html";
    QString html=this->callPandoc(pandocArgument,mw->m_textEditor->toPlainText());
    mw->m_templateBrowser->setText(html);
}


void Controller::saveDocumentSource()
{
    if(mw->m_nameOfNewDocEdit->text().isEmpty())
    {
        this->showErrorMsg("The name of the document can not be empty");
        return;
    }

    QString sourceCode=mw->m_textEditor->toPlainText();
    QString nameTemplate=mw->m_nameOfNewDocEdit->text();
    QString tempVariables=this->getTemplateVariables(sourceCode);


    if(sourceCode.isEmpty())
    {
        sourceCode=" ";
    }
    QStringList pandocArgument;
    pandocArgument<<"-f"<<"markdown"<<"-t"<<"html";
    QString html=this->callPandoc(pandocArgument,sourceCode);
    html.replace("\"", "xxquotes");
    bool success=false;
    if(m_editingTemplate)
    {
        success=m_templateDb.editDocumentTemplate(m_template->id,nameTemplate,sourceCode,html,tempVariables);
    }
    else
    {
        success=m_templateDb.saveTemplateSource(nameTemplate,sourceCode);
    }
    if(success)
    {
        mw->m_statusBar->showMessage(tr("Text saved with success"),5000);
    }
    else
    {
        QString error="Text not saved: " + m_templateDb.m_dbError;
        mw->m_statusBar->showMessage((error),5000);
    }
}


void Controller::startImporting()
{
    mw->m_errorLabel->setVisible(false);
    if(mw->m_importedNameEdit->text().isEmpty())
    {
        mw->m_errorLabel->setVisible(true);
        mw->m_errorLabel->setText("Give a name to the template");
        return;
    }
    QStringList parameters;
    QString path=mw->m_chooseFileEdit->text();
    parameters<<"-f"<<"docx"<<"-t"<<"markdown";
    QString markdown=this->callPandoc(parameters,path,true);
    parameters.clear();
    parameters<<"-f"<<"markdown"<<"-t"<<"html";
    QString html=this->callPandoc(parameters,markdown);
    QString variables=this->getTemplateVariables(markdown);
    html.replace("\"", "xxquotes");

    if(m_templateDb.saveTemplateSource(mw->m_importedNameEdit->text(),markdown,html,variables))
    {
        mw->m_statusBar->showMessage("Template imported with sucees");
        mw->m_importTemplateGroupBox->setVisible(false);
    }
    else
    {
        mw->m_statusBar->showMessage(m_templateDb.m_dbError);

    }
}


bool Controller::selectFile()
{
    mw->m_chooseFileEdit->setText(QFileDialog::getOpenFileName(mw,tr("Choose the file containing the template"),"",
                                                               "Microsofot Word Document(*.docx *.doc)"));
    return true;
}


void Controller::searchDocument()
{
    m_template=m_templateDb.searchTemplate(mw->m_searchDocEdit->text());
    m_template->html.replace("xxquotes","\"");
    m_template=m_templateDb.searchTemplate(mw->m_searchDocEdit->text());
    mw->m_previewBrowser->setText(m_template->html);
    QString strVar=m_template->variables;
    QStringList strListvar=strVar.split(",");
    QVector<QString> tabvar;
    for(auto var:strListvar)
    {
        tabvar.push_back(var);

    }
    mw->m_tempForm->initForm(tabvar);

    mw->m_tempFormProxy->dtSetDisplayInfos(0,0,m_width*0.25,m_height);
    mw->m_searchPrintProxy->dtSetDisplayInfos(m_width*0.25,0,m_width*0.75,m_height);
    mw->m_displayedWidgetVector.clear();
    mw->m_displayedWidgetVector.push_back(mw->m_searchPrintProxy);
    mw->m_displayedWidgetVector.push_back(mw->m_tempFormProxy);
    mw->displayWidgets(mw->m_displayedWidgetVector);
}


void Controller::showErrorMsg(QString error)
{
    mw->m_errorLabel->setVisible(true);
    mw->m_errorLabel->setText(error);
}
