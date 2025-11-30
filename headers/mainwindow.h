#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

enum State {
    Left,
    Right,
    Result
};

enum Operator {
    Plus,
    Minus,
    Times,
    Divide,
    None
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setLeftState();
    void setRightState();
    void setResultState();
    void setNoneOperator();

private:
    Ui::MainWindow *ui;
    State state;
    Operator _operator;
    QButtonGroup *buttonGroup;
    QPoint mousePosition;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void on_buttonPlus_clicked();
    void on_buttonMinus_clicked();
    void on_buttonTimes_clicked();
    void on_buttonDivide_clicked();
};
#endif // MAINWINDOW_H
