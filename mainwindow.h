#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_Exit_triggered();

    void on_savebtn_clicked();

    void on_actionSave_as_triggered();

    void on_action_Save_triggered();

    void on_openbtn_clicked();

    void on_action_Open_triggered();

    void on_action_New_triggered();

    void on_newbtn_clicked();

    void on_reset_btn_clicked();

    void on_compile_btn_clicked();

private:
    Ui::MainWindow *ui;
    QString issaved="";
    int Sc;
    int Pc;
    int Ar;
    int Ir;
    int Dr;
    int Ac;
    int Tr;
    int Inpr;
    int Outr;
    int I;
    int S;
    int E;
    int R;
    int Ien;
    int FGI;
    int Fgo;


};
#endif // MAINWINDOW_H
