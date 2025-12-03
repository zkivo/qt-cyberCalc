#pragma once

#include <QMainWindow>
#include <QPoint>
#include <QButtonGroup>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private slots:
    void on_buttonPlus_clicked();
    void on_buttonMinus_clicked();
    void on_buttonTimes_clicked();
    void on_buttonDivide_clicked();

private:
    enum State { Left, Right };
    enum Operator { None, Plus, Minus, Times, Divide };

    Ui::MainWindow *ui;
    QButtonGroup *buttonGroup;

    State state = Left;
    Operator _operator = None;
    QPoint mousePosition;

    // helpers
    int currentLCDValue() const;
    void setLCDValue(int value);

    bool handleDigitKey(int n);
    bool handleBackspace();
    bool handleOperatorKey(int key);
    bool handleReturnKey();

    void applyOperator(Operator op, const QString &symbol);
    void setLeftState();
    void setRightState();
    void clearOperator();

    void setupButtons();
    void setInitialState();
};
