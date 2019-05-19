#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H
class LineNumberWidget;
class TextEditor;
#include <QtGui>
#include <QtWidgets>

class LmEditor:public QWidget
{
    Q_OBJECT
    public:
        explicit LmEditor(QWidget *parent = nullptr);
        void createToolsWidget();
        void createCentralWidget();
        void createDockWidget();

    private:
        QWidget *m_toolsWidget, *m_dockWidget,*m_centralWidget;
        TextEditor *m_textEditor;

};

class TextEditor : public QPlainTextEdit
{
    Q_OBJECT
    public:
        explicit TextEditor(QWidget *parent = nullptr);

        int lineNumberWidgetWidth();
        void lineNumberWidgetPaintEvent(QPaintEvent *event);
    protected:
        void resizeEvent(QResizeEvent *event) override;
    private slots:

        void highLightCurrentLine();
        void updateLineNumberWidget(const QRect &,int);
        void updateLineNumberWidgetWidth(int);

    private:

    LineNumberWidget *m_lineNumberWidget;


};

class LineNumberWidget:public QWidget
{
    Q_OBJECT
    public:
        LineNumberWidget(TextEditor *editor):QWidget(editor)
        {
           m_textEditor=editor;
        }

        QSize sizeHint() const override
        {
            return QSize(m_textEditor->lineNumberWidgetWidth(),0);
        }
    protected:

        void paintEvent(QPaintEvent *event) override
        {
            m_textEditor->lineNumberWidgetPaintEvent(event);
        }
    private:
        TextEditor *m_textEditor;
};
#endif // TEXTEDITOR_H
