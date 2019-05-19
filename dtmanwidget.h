#ifndef DTMANWIDGET_H
#define DTMANWIDGET_H

#include <QtWidgets>
#include <QtCore>
#include "dtmantype.h"
class Controller;

class TemplateForm : public QWidget
{
    Q_OBJECT

    friend class Controller;
public:
    explicit TemplateForm(QWidget *parent=0);
    void initForm(QVector<QString> variables);
private:
    QVector<QLabel*> m_labelTab;
    QVector<QLineEdit*> m_editTab;
    QGridLayout *m_mainLayout;
signals:

public slots:
};

class ProxyWidget:public QGraphicsProxyWidget
{
    Q_OBJECT

    public:

        ProxyWidget(QGraphicsItem *parent=nullptr);
    public:
        void dtSetPos(int x,int y);
        void dtSetDisplayInfos(int x,int y,int width,int height);
    public:
        WidgetDisplayInfos *m_displayInfos;

    private:
        QTransform m_tranform;
};

#endif // DTMANWIDGET_H
