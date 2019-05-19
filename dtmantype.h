#ifndef DTMANTYPE_H
#define DTMANTYPE_H

#include <QtCore>
class ProxyWidget;

struct Template : public QObject
{
    Q_OBJECT
    public:
        QString id;
        QString name;
        QString sourceCode;
        QString html;
        QString variables;
    public:
        Template(QObject * parent=nullptr);
};

struct WidgetDisplayInfos:public QObject
{
    Q_OBJECT
    public:
        ProxyWidget *ptrWidget=0;
        int x=0;
        int y=0;
        int width=0;
        int height=0;

    public:
        WidgetDisplayInfos(QObject *parent=nullptr);
};

#endif // DTMAN_TYPE
