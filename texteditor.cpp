#include "texteditor.h"

LmEditor::LmEditor(QWidget *parent) : QWidget(parent)
{
    QGridLayout *mainLayout=new QGridLayout();
    this->setLayout(mainLayout);

    m_toolsWidget=new QWidget();
    m_dockWidget=new QWidget();
    m_centralWidget=new QWidget();

    mainLayout->addWidget(m_toolsWidget,0,0,1,2);
    mainLayout->addWidget(m_dockWidget,1,1,2,1);
    mainLayout->addWidget(m_centralWidget,1,1,1,1);

    this->createCentralWidget();

}


void LmEditor::createCentralWidget()
{
    QVBoxLayout *centralLayout=new QVBoxLayout();
    m_centralWidget->setLayout(centralLayout);

    m_textEditor=new TextEditor();

    centralLayout->addWidget(m_textEditor);
}


TextEditor::TextEditor(QWidget *parent):QPlainTextEdit(parent)
{

    m_lineNumberWidget=new LineNumberWidget(this);
    connect(this,SIGNAL(updateRequest(QRect,int)),this,SLOT(updateLineNumberWidget(QRect,int)));
    connect(this,SIGNAL(blockCountChanged(int)),this,SLOT(updateLineNumberWidgetWidth(int)));
    connect(this,SIGNAL(cursorPositionChanged()),this,SLOT(highLightCurrentLine()));
    updateLineNumberWidgetWidth(0);
    highLightCurrentLine();
}


void TextEditor::highLightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelecs;
    if(!(this->isReadOnly()))
    {
        QTextEdit::ExtraSelection selection;
        QColor lineColor=QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection,true);
        selection.cursor=this->textCursor();
        selection.cursor.clearSelection();
        extraSelecs.append(selection);
    }
    this->setExtraSelections(extraSelecs);
}


int TextEditor::lineNumberWidgetWidth()
{
    int digitsNumber=1;
    int maxDigit=qMax(1,this->blockCount());
    while(maxDigit>=10)
    {
        maxDigit/=10;
        ++digitsNumber;
    }

    int widgetWidth=3+this->fontMetrics().width(QLatin1Char('9'))*digitsNumber;
    return widgetWidth;

}


void TextEditor::lineNumberWidgetPaintEvent(QPaintEvent *event)

{
    QPainter painter(m_lineNumberWidget);
    painter.fillRect(event->rect(),Qt::lightGray);
    QTextBlock block=this->firstVisibleBlock();
    int blockNumber=block.blockNumber();
    int top=(int)this->blockBoundingGeometry(block).translated(this->contentOffset()).top();
    int bottom=top+ (int)this->blockBoundingRect(block).height();

    while(block.isValid()&& top<=event->rect().bottom())
    {
        if(block.isVisible() && bottom>=event->rect().top())
        {
            QString number=QString::number(blockNumber+1);
            painter.setPen(Qt::black);
            painter.drawText(0,top,m_lineNumberWidget->width(),this->fontMetrics().height(),Qt::AlignRight,number);
        }
        block=block.next();
        top=bottom;
        bottom=top+(int)this->blockBoundingRect(block).height();
        ++blockNumber;
    }
}



void TextEditor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);
    QRect cr=this->contentsRect();
    m_lineNumberWidget->setGeometry(QRect(cr.left(),cr.top(),lineNumberWidgetWidth(),cr.height()));
}


void TextEditor::updateLineNumberWidget(const QRect & rect, int dy)
{
    if(dy)
    {
        m_lineNumberWidget->scroll(0,dy);
    }
    else
    {
        m_lineNumberWidget->update(0,rect.y(),m_lineNumberWidget->width(),rect.height());
    }

    if(rect.contains(this->viewport()->rect()))
    {
        this->updateLineNumberWidgetWidth(0);

    }
}


void TextEditor::updateLineNumberWidgetWidth(int /*blockCount*/)
{
    this->setViewportMargins(this->lineNumberWidgetWidth(),0,0,0);
}
