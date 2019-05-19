#include "dtmanwidget.h"

TemplateForm::TemplateForm(QWidget *parent) : QWidget(parent)
{
    m_mainLayout=new QGridLayout();
    this->setLayout(m_mainLayout);
}


void TemplateForm::initForm(QVector<QString> variables)
{
    for(auto pointer:m_editTab)
        delete pointer;
    for(auto pointer:m_labelTab)
        delete pointer;
    m_editTab.clear();
    m_labelTab.clear();
    int i=0;
    for(auto var:variables)
    {
        m_labelTab.push_back(new QLabel(var));
        m_editTab.push_back(new QLineEdit());

        m_mainLayout->addWidget(m_labelTab[i],i,0);
        m_mainLayout->addWidget(m_editTab[i],i,1);
        ++i;
    }

}


ProxyWidget::ProxyWidget(QGraphicsItem *parent):QGraphicsProxyWidget(parent)
{
    m_displayInfos=new WidgetDisplayInfos(this);
    m_displayInfos->ptrWidget=this;
}


void ProxyWidget::dtSetDisplayInfos(int x, int y, int width, int height)
{
    m_displayInfos->x=x;
    m_displayInfos->y=y;
    m_displayInfos->width=width;
    m_displayInfos->height=height;

    this->dtSetPos(x,y);
    this->widget()->setFixedSize(width,height);
}


void ProxyWidget::dtSetPos(int x, int y)
{
    m_displayInfos->x=x;
    m_displayInfos->y=y;
    m_tranform.translate(m_displayInfos->x,m_displayInfos->y);
    this->setTransform(m_tranform,true);
}
