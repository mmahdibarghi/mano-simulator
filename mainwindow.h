#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<bitset>
#include <QMainWindow>
#include<map>
using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    inline quint16 getE() { return E.to_ulong(); }
    inline quint16 getDR() { return DR.to_ulong(); }
    inline quint16 getAC() { return AC.to_ulong(); }
    inline bool getACleadingBit() { return AC[15]; }
    inline quint16 getIR() { return IR.to_ulong(); }
    inline quint16 getTR() { return TR.to_ulong(); }
    inline quint16 getAR() { return AR.to_ulong(); }
    inline quint16 getPC() { return PC.to_ulong(); }
    inline quint16 getINPR() { return INPR.to_ulong(); }
    inline quint16 getOUTR() { return OUTR.to_ulong(); }


    inline void setE(const quint16& E) { this->E = std::bitset<1>(E); }
    inline void setDR(const quint16& DR) { this->DR = std::bitset<16>(DR); }
    inline void setAC(const quint16& AC) { this->AC = std::bitset<16>(AC); }
    inline void setIR(const quint16& IR) { this->IR = std::bitset<16>(IR); }
    inline void setTR(const quint16& TR) { this->TR = std::bitset<16>(TR); }
    inline void setAR(const quint16& AR) { this->AR = std::bitset<12>(AR); }
    inline void setPC(const quint16& PC) { this->PC = std::bitset<12>(PC); }
    inline void setINPR(const quint8& INPR) { this->INPR = std::bitset<8>(INPR); }
    inline void setOUTR(const quint8& OUTR) { this->OUTR = std::bitset<8>(OUTR); }


    inline void inrAR() { setAR(getAR() + 1); }
    inline void inrPC() { setPC(getPC() + 1); }
    inline void inrDR() { setDR(getDR() + 1); }
    inline void inrAC() { setAC(getAC() + 1); }
    inline void inrTR() { setTR(getTR() + 1); }

    void emptyTable();
    void resetRam();
    inline void clrAR() { AR.reset(); }
    inline void clrPC() { PC.reset(); }
    inline void clrDR() { DR.reset(); }
    inline void clrAC() { AC.reset(); }
    inline void clrTR() { TR.reset(); }

    inline void logicUnitAND(){ AC = AC & DR; }
    inline void logicUnitCMA() { AC = ~AC; }
    void logicUnitCME() { E = ~E; }
    void arithmeticUnitADD();
    void arithmeticUnitCIR();
    void arithmeticUnitCIL();



    void printReg();
    void printTable();

    bool isNumber(const QString& str);

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

    void on_run_btn_clicked();

    void on_next_btn_clicked();

private:
    Ui::MainWindow *ui;
    QString issaved="";
    bitset<16> ram[4096];
    bitset<16> DR;
    bitset<16> SC;
    bitset<16> AC;
    bitset<16> IR;
    bitset<16> TR;
    bitset<12> AR;
    bitset<12> PC;
    bitset<8> INPR;
    bitset<8> OUTR;
    bitset<1> E;
    bitset<1> I;
    bitset<1> S;
    bitset<1> R;
    bitset<1> IEN;
    bitset<1> FGI;
    bitset<1> FGO;
    map<QString,int> allDatas;
    map<QString,int> firstallDatas;
    QStringList commands;


    int reseter=1;
    int compiled=0;




};
#endif // MAINWINDOW_H
