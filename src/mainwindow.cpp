#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QKeyEvent>
#include <QButtonGroup>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

static int numDigits(int number)
{
    number = std::abs(number);
    if (number == 0)
        return 1;

    int digits = 0;
    while (number) {
        number /= 10;
        digits++;
    }
    return digits;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , buttonGroup(new QButtonGroup(this))
{
    ui->setupUi(this);

    qApp->installEventFilter(this);

    setInitialState();
    clearOperator();

    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);
    setFixedSize(450, 197);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    QPixmap pix(":/card.png");
    p.drawPixmap(0, 0, width(), height(), pix);
}

void MainWindow::setupButtons()
{
    QVector<QPushButton*> buttons {
        ui->buttonPlus, ui->buttonMinus, ui->buttonTimes, ui->buttonDivide
    };

    buttonGroup->setExclusive(true);
    for (auto *b : buttons) {
        b->setFocusPolicy(Qt::NoFocus);
        buttonGroup->addButton(b);
    }
}

void MainWindow::setInitialState()
{
    setupButtons();
    setLeftState();
}

void MainWindow::clearOperator()
{
    _operator = None;
    buttonGroup->setExclusive(false);
    ui->buttonPlus->setChecked(false);
    ui->buttonMinus->setChecked(false);
    ui->buttonTimes->setChecked(false);
    ui->buttonDivide->setChecked(false);
    ui->labelOperator->setText("");
    buttonGroup->setExclusive(true);
}

void MainWindow::setLeftState()
{
    state = Left;
    ui->lcdLeft->setStyleSheet(
        "QLCDNumber { border: 3px solid #0abdc6; border-radius: 6px; }"
    );
    ui->lcdRight->setStyleSheet(
        "QLCDNumber { border: none; background: transparent; color: inherit; }"
    );
}

void MainWindow::setRightState()
{
    state = Right;
    ui->lcdRight->setStyleSheet(
        "QLCDNumber { border: 3px solid #0abdc6; border-radius: 6px; }"
    );
    ui->lcdLeft->setStyleSheet(
        "QLCDNumber { border: none; background: transparent; color: inherit; }"
    );
}

int MainWindow::currentLCDValue() const
{
    return (state == Left) ? ui->lcdLeft->value()
                           : ui->lcdRight->value();
}

void MainWindow::setLCDValue(int value)
{
    if (state == Left)
        ui->lcdLeft->display(value);
    else
        ui->lcdRight->display(value);
}

bool MainWindow::handleDigitKey(int n)
{
    QLCDNumber *lcd = (state == Left ? ui->lcdLeft : ui->lcdRight);
    int digits = numDigits(lcd->value());

    if (digits >= lcd->digitCount()) {
        qDebug() << "LCD reached max digits";
        return true;
    }

    setLCDValue(lcd->value() * 10 + n);
    return true;
}

bool MainWindow::handleBackspace()
{
    int value = currentLCDValue();
    if (value == 0)
        return true;

    setLCDValue(value / 10);
    return true;
}

void MainWindow::applyOperator(Operator op, const QString &symbol)
{
    _operator = op;
    ui->labelOperator->setText(symbol);

    ui->buttonPlus->setChecked(op == Plus);
    ui->buttonMinus->setChecked(op == Minus);
    ui->buttonTimes->setChecked(op == Times);
    ui->buttonDivide->setChecked(op == Divide);
}

bool MainWindow::handleOperatorKey(int key)
{
    switch (key) {
    case Qt::Key_Plus:
        applyOperator(Plus, "+");
        return true;
    case Qt::Key_Minus:
        applyOperator(Minus, "-");
        return true;
    case Qt::Key_Asterisk:
    case Qt::Key_X:
        applyOperator(Times, "x");
        return true;
    case Qt::Key_Slash:
        applyOperator(Divide, "/");
        return true;
    }
    return false;
}

bool MainWindow::handleReturnKey()
{
    if (_operator == None) {
        qDebug() << "insert an operator first";
        return true;
    }

    int a = ui->lcdLeft->value();
    int b = ui->lcdRight->value();
    int result = 0;

    switch (_operator) {
    case Plus:
        result = a + b;
        break;
    case Minus:
        result = a - b;
        break;
    case Times:
        result = a * b;
        break;
    case Divide:
        if (b == 0) {
            qDebug() << "division by zero";
            return true;
        }
        result = a / b;
        break;
    default:
        break;
    }

    ui->labelResult->setText(QString::number(result));

    clearOperator();
    setLeftState();

    return true;
}

bool MainWindow::eventFilter(QObject *, QEvent *event)
{
    if (event->type() != QEvent::KeyPress)
        return false;

    QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
    int key = keyEvent->key();

    if (key == Qt::Key_Left)  {
        setLeftState();
        return true;
    }
    if (key == Qt::Key_Right) {
        setRightState();
        return true;
    }
    if (key == Qt::Key_Up || key == Qt::Key_Down)
        return true;

    if (key >= Qt::Key_0 && key <= Qt::Key_9)
        return handleDigitKey(key - Qt::Key_0);

    if (key == Qt::Key_Backspace)
        return handleBackspace();

    if (handleOperatorKey(key))
        return true;

    if (key == Qt::Key_Return)
        return handleReturnKey();

    return true;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        mousePosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPosition().toPoint() - mousePosition);
        event->accept();
    }
}

void MainWindow::on_buttonPlus_clicked()
{
    applyOperator(Plus, "+");
}

void MainWindow::on_buttonMinus_clicked()
{
    applyOperator(Minus, "-");
}

void MainWindow::on_buttonTimes_clicked()
{
    applyOperator(Times, "x");
}

void MainWindow::on_buttonDivide_clicked()
{
    applyOperator(Divide, "/");
}
