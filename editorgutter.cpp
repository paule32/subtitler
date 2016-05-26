#include "mainwindow.h"
#include <QPainter>
#include <QMessageBox>

extern class EditorGutter * gutter;
extern class MainWindow   * w;

MyEditor::MyEditor(QWidget *parent)
    : QPlainTextEdit(parent)
{
    setObjectName("TitleEditor");
    setWordWrapMode(QTextOption::NoWrap);
    setAcceptDrops(true);

    gutter = new EditorGutter(this);
    lines  = 1;

    //scr1   = new QScrollArea(this);

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

bool MyEditor::get_input1()
{
    QTextEdit::ExtraSelection selection;
    selection.cursor =     textCursor();

    int y = selection.cursor.blockNumber()+1;
    int c = 0;
    int p = 2;

    QMap<int, bool> m2;

    m2.insert(1,false);
    m2.insert(2,false);

    for (c = 0; c < 2048; c++) {
        ++p; m2.insert(p,true );
        ++p; m2.insert(p,true );

        ++p; m2.insert(p,false);
        ++p; m2.insert(p,false);
    }

    if (m2[y] == true)
    return true ;
    return false;
}

void MyEditor::dragMoveEvent(QDragMoveEvent * e)
{
    e->ignore();
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
        if (!get_input1())
        insertPlainText("    ");
        break;

    case Qt::Key_Down:
    {
        QTextEdit::ExtraSelection selection;
        selection.cursor =     textCursor();
        switch (selection.cursor.blockNumber()+1)  {
        case 1:
        case 5:
        case 9:
        case 13: case 17: case 21: case 25: case 29:
        case 33: case 37: case 41: case 45: case 49:
        case 53: case 57: case 61: case 65: case 69:
        case 73: case 77: case 81: case 85: case 89:
        {
            moveCursor(QTextCursor::Down,QTextCursor::MoveAnchor);
            moveCursor(QTextCursor::Down,QTextCursor::MoveAnchor);
        }   break;
        default:
            moveCursor(QTextCursor::Down,QTextCursor::MoveAnchor);
            break; }
    }       break;
    case Qt::Key_Up:
    {
        QTextEdit::ExtraSelection selection;
        selection.cursor =     textCursor();

        switch (selection.cursor.blockNumber()+1)  {
        case 3:
        case 7:
        case 11:
        case 15: case 19: case 23: case 27: case 31:
        case 35: case 39: case 43: case 47: case 51:
        case 55: case 59: case 63: case 67: case 71:
        case 75: case 79: case 83: case 87: case 91:
        {
            moveCursor(QTextCursor::Up,QTextCursor::MoveAnchor);
            moveCursor(QTextCursor::Up,QTextCursor::MoveAnchor);
        }   break;
        default:
            moveCursor(QTextCursor::Up,QTextCursor::MoveAnchor);
            break; }
    }       break;

    case Qt::Key_Backspace:
    case Qt::Key_Delete:
    {
        if (get_input1()) return;
        QTextEdit::ExtraSelection selection;
        selection.cursor =     textCursor();

        int y = selection.cursor.blockNumber () + 1;
        int x = selection.cursor.columnNumber() + 1;

        if (x == 1) return;
        QPlainTextEdit::keyPressEvent(event);
    }   break;

    case Qt::Key_0: case Qt::Key_1: case Qt::Key_2: case Qt::Key_3: case Qt::Key_4: case Qt::Key_5:
    case Qt::Key_6: case Qt::Key_7: case Qt::Key_8: case Qt::Key_9:
    case Qt::Key_A: case Qt::Key_B: case Qt::Key_C: case Qt::Key_D: case Qt::Key_E: case Qt::Key_F:
    case Qt::Key_G: case Qt::Key_H: case Qt::Key_I: case Qt::Key_J: case Qt::Key_K: case Qt::Key_L:
    case Qt::Key_M: case Qt::Key_N: case Qt::Key_O: case Qt::Key_P: case Qt::Key_Q: case Qt::Key_R:
    case Qt::Key_S: case Qt::Key_T: case Qt::Key_U: case Qt::Key_V: case Qt::Key_W: case Qt::Key_X:
    case Qt::Key_Y: case Qt::Key_Z:

    case Qt::Key_Space:

    case Qt::Key_Adiaeresis:
    case Qt::Key_Odiaeresis:
    case Qt::Key_Udiaeresis:    {
        if (!get_input1()) return;
        QPlainTextEdit::keyPressEvent(event);
    }   break; default:
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
    //scr1->hide();
    setViewportMargins(gutterWidth(), 0,0,0);
/*
    QTextCursor cursor = textCursor();
    int y = cursor.blockNumber () + 1;
    int x = cursor.columnNumber() + 1;

    int fm = fontMetrics().height();
    int y1 = fm * y;

    scr1->resize(440,fm+3);
    scr1->move(gutterWidth()+53,y1+4);
    scr1->setStyleSheet("background-color: lime;");
    scr1->show();*/
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
