#include "mainwindow.h"
#include <QPainter>
#include <QMessageBox>

extern class EditorGutter *gutter;

MyEditor::MyEditor(QWidget *parent)
    : QPlainTextEdit(parent)
{
    setObjectName("dBaseEditor");
    setWordWrapMode(QTextOption::NoWrap);
    setAcceptDrops(true);

    gutter = new EditorGutter(this);
    lines  = 1;

    scr1   = new QScrollArea(this);

    //connect(this, SIGNAL(blockCountChanged(int)  ), this, SLOT(updateGutterWidth(int)));
    connect(this, SIGNAL(cursorPositionChanged() ), this, SLOT(on_cursorPositionChanged()));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(on_linesUpdate(QRect,int)));

    highlighter = new Highlighter(this);

    on_gutterUpdate(0);
    on_cursorPositionChanged();
}

void MyEditor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    gutter->setGeometry(
    QRect(cr.left(),
          cr.top (), gutterWidth(),
          cr.height()));
}

void MyEditor::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Escape:
        qApp->quit();
        break;

    case Qt::Key_F1:
        //on_dockHelpOpen();
        break;

    case Qt::Key_F2:
        //on_parseText();
        break;

    case Qt::Key_Return:
    case Qt::Key_Enter:
        break;
    case Qt::Key_Tab:
        insertPlainText("    ");
        break;
    case Qt::Key_Delete:
    {
        QList<QTextEdit::ExtraSelection> extraSelections;

        QTextEdit::ExtraSelection selection;
        selection.cursor =     textCursor();

        int y = selection.cursor.blockNumber () + 1;
        int x = selection.cursor.columnNumber() + 1;

        if (x == 1) return;
        QPlainTextEdit::keyPressEvent(event);
    }   break;

    default:
        QPlainTextEdit::keyPressEvent(event);
        break;
    }
}

void MyEditor::mousePressEvent(class QMouseEvent  *event)
{

    QPlainTextEdit::mousePressEvent(event);
    on_cursorPositionChanged();
}

void MyEditor::on_parseText()
{
    //QMessageBox::information(this,"nnn","parser");
    //parseText();
}

void MyEditor::on_dockHelpOpen() {
    //w->on_dockHelpOpen();
}

void MyEditor::on_cursorPositionChanged()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;
        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor =   textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void MyEditor::on_gutterUpdate(int) {
    scr1->hide();
    setViewportMargins(gutterWidth(), 0,0,0);

    QTextCursor cursor = textCursor();
    int y = cursor.blockNumber () + 1;
    int x = cursor.columnNumber() + 1;

    int fm = fontMetrics().height();
    int y1 = fm * y;

    scr1->resize(440,fm+3);
    scr1->move(gutterWidth()+3,y1+4);
    scr1->setStyleSheet("background-color: lime;");
    scr1->show();
}

void MyEditor::on_linesUpdate(const QRect &rect, int dy)
{
    if (dy)
    gutter->scroll(0,dy); else
    gutter->update(0,rect.y(),
    gutter->width() ,rect.height());

    if (rect.contains(viewport()->rect()))
       on_gutterUpdate(0);
}

void MyEditor::linePaintEvent(class QPaintEvent *event)
{
    QPainter painter(gutter);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).
                    translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    lines = 0;
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, gutter->width(),
                             fontMetrics().height(),
                             Qt::AlignRight, number);
            ++lines;
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

int MyEditor::gutterWidth()
{
    int digits = 2;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

Highlighter::Highlighter(MyEditor *parent)
    : QSyntaxHighlighter(parent->document())
{
    //HighlighterRule rule;

    cppCommentFormat.setForeground(Qt::red);
    cppCommentFormat.setFontWeight(QFont::Bold);
    cppCommentFormat.setFontItalic(true);

    commentStart = QRegExp("/\\*");
    commentEnd   = QRegExp("\\*/");
}

void Highlighter::highlightBlock(const QString &text)
{
    enum { NormalState = -1, InsideCStyleComment };

    int state = previousBlockState();
    int start = 0;

    for (int i = 0; i < text.length(); ++i) {
        if (state == InsideCStyleComment) {
            if (text.mid(i, 2) == "*/") {
                state = NormalState;
                setFormat(start, i - start + 2, cppCommentFormat);
            }   }   else {
            if (text.mid(i,2) == "**") {
                setFormat(i, text.length() - i, cppCommentFormat);
                break;
            }
            // C++ comment
            if (text.mid(i,2) == "//") {
                setFormat(i, text.length() - i, cppCommentFormat);
                break;  }
                else if (text.mid(i, 2) == "/*") {
                start = i;
                state = InsideCStyleComment;
            }
        }
    }

    if (state == InsideCStyleComment)
    setFormat(start, text.length() - start, cppCommentFormat);

    setCurrentBlockState(state);
}
