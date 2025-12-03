#include <QKeyEvent>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QButtonGroup>
#include <QString>
#include <QPainter>

int numDigits(int number)
{
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
{
    ui->setupUi(this);
    setLeftState();
    // listen to ALL key events in the app
    qApp->installEventFilter(this);

    ui->buttonPlus->setCheckable(true);
    ui->buttonMinus->setCheckable(true);
    ui->buttonTimes->setCheckable(true);
    ui->buttonDivide->setCheckable(true);

    ui->buttonPlus->setFocusPolicy(Qt::NoFocus);
    ui->buttonMinus->setFocusPolicy(Qt::NoFocus);
    ui->buttonTimes->setFocusPolicy(Qt::NoFocus);
    ui->buttonDivide->setFocusPolicy(Qt::NoFocus);

    buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(true);
    buttonGroup->addButton(ui->buttonPlus);
    buttonGroup->addButton(ui->buttonMinus);
    buttonGroup->addButton(ui->buttonTimes);
    buttonGroup->addButton(ui->buttonDivide);

    setNoneOperator();

    setFixedSize(this->size());
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);
    setFixedSize(450, 197);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    QPixmap pix(":/card.png");
    p.drawPixmap(0, 0, width(), height(), pix);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setNoneOperator() {
    _operator = None;
    buttonGroup->setExclusive(false);
    ui->buttonPlus->setChecked(false);
    ui->buttonMinus->setChecked(false);
    ui->buttonTimes->setChecked(false);
    ui->buttonDivide->setChecked(false);
    ui->labelOperator->setText("");
    buttonGroup->setExclusive(true);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        mousePosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

// When mouse moves with button pressed
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move((event->globalPosition().toPoint() - mousePosition));
        event->accept();
    }
}


void MainWindow::setLeftState() {
    qDebug() << "setting left state...";
    state = Left;
    ui->lcdLeft->setStyleSheet(
        "QLCDNumber {"
        "    border: 3px solid #0abdc6;"
        "    border-radius: 6px;"
        "}"
    );
    ui->lcdRight->setStyleSheet(
        "QLCDNumber {"
        "    all: unset;"
        "}"
    );
}

void MainWindow::setRightState() {
    qDebug() << "setting right state...";
    state = Right;
    ui->lcdRight->setStyleSheet(
        "QLCDNumber {"
        "    border: 3px solid #0abdc6;"
        "    border-radius: 6px;"
        "}"
    );
    ui->lcdLeft->setStyleSheet(
        "QLCDNumber {"
        "    all: unset;"
        "}"
    );
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        auto *keyEvent = static_cast<QKeyEvent*>(event);
        int key = keyEvent->key();
        QString labelResultSrring = ui->labelResult->text();
        ui->labelResult->setText("");
        if (key == Qt::Key_Left) {
            setLeftState();
            return true;
        } else if (key == Qt::Key_Right) {
            setRightState();
            return true;
        } else if (key == Qt::Key_Up || key == Qt::Key_Down) {
            qDebug() << "Nothing...";
            return true;
        } else if (key >= Qt::Key_0 && key <= Qt::Key_9) {
            int n = key - Qt::Key_0;
            if (state == Left) {
                int digits = numDigits(ui->lcdLeft->value());
                if (digits >= ui->lcdLeft->digitCount()) {
                    qDebug() << "lcdLeft reached max number of digits";
                    return true;
                }
                ui->lcdLeft->display(ui->lcdLeft->value() * 10 + n);
            } else if (state == Right) {
                int digits = numDigits(ui->lcdRight->value());
                if (digits >= ui->lcdRight->digitCount()) {
                    qDebug() << "lcdRight reached max number of digits";
                    return true;
                }
                ui->lcdRight->display(ui->lcdRight->value() * 10 + n);
            } else {
                qDebug() << "We are in the Result state...";
            }
            return true;
        } else if (key == Qt::Key_Backspace) {
            if (state == Left) {
                int value = ui->lcdLeft->value();
                if (value == 0) return true;
                ui->lcdLeft->display(value / 10);
            } else if (state == Right) {
                int value = ui->lcdRight->value();
                if (value == 0) return true;
                ui->lcdRight->display(value / 10);
            } else {
                qDebug() << "We are in the Result state...";
            }
            return true;
        } else if (key == Qt::Key_Plus) {
            _operator = Plus;
            ui->labelOperator->setText("+");
            ui->buttonPlus->setChecked(true);
            return true;
        } else if (key == Qt::Key_Minus) {
            _operator = Minus;
            ui->labelOperator->setText("-");
            ui->buttonMinus->setChecked(true);
        } else if (key == Qt::Key_Asterisk) {
            _operator = Times;
            ui->labelOperator->setText("x");
            ui->buttonTimes->setChecked(true);
        } else if (key == Qt::Key_Slash) {
            _operator = Divide;
            ui->labelOperator->setText("/");
            ui->buttonDivide->setChecked(true);
        } else if (key == Qt::Key_Return) {
            if (_operator == None) {
                qDebug() << "please insert an operator...";
            } else {
                int valueLeft  = ui->lcdLeft->value();
                int valueRight = ui->lcdRight->value();
                int result = 0;
                if (_operator == Plus) {
                    result = valueLeft + valueRight;
                } else if (_operator == Minus) {
                    result = valueLeft - valueRight;
                } else if (_operator == Times) {
                    result = valueLeft * valueRight;
                } else if (_operator == Divide) {
                    result = valueLeft / valueRight;
                }
                int digits = numDigits(result);
                if (digits > ui->lcdLeft->digitCount()) {
                    qDebug() << "Error: The results has too many digits...(" << result << ")";
                    return true;
                }
                ui->lcdLeft->display(result);
                ui->lcdRight->display(0);
                ui->labelResult->setText("=");
                setNoneOperator();
                setLeftState();
            }
            return true;
        } else if (key == Qt::Key_X) {
            _operator = Times;
            ui->labelOperator->setText("x");
            ui->buttonTimes->setChecked(true);
            return true;
        } else {
            ui->labelResult->setText(labelResultSrring);
            return true;
        }
    }

    // let Qt handle everything else
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::on_buttonPlus_clicked()
{
    _operator = Plus;
    ui->labelOperator->setText("+");
}


void MainWindow::on_buttonMinus_clicked()
{
    _operator = Minus;
    ui->labelOperator->setText("-");
}


void MainWindow::on_buttonTimes_clicked()
{
    _operator = Times;
    ui->labelOperator->setText("x");
}


void MainWindow::on_buttonDivide_clicked()
{
    _operator = Divide;
    ui->labelOperator->setText("/");
}

