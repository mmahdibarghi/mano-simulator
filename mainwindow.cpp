#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFileDialog>
#include<fstream>
#include<QFile>
#include<QMessageBox>
#include<sstream>
#include <ctype.h>
using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("MyText Editor");
    ui->editor->setFocus();
    ui->savebtn->setIcon(QIcon(":/pic/saveicon"));
    ui->openbtn->setIcon(QIcon(":/pic/openicon"));
    ui->newbtn->setIcon(QIcon(":/pic/newicon"));
    on_reset_btn_clicked();

}

MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::emptyTable()
{
    for(int v=0;v<4096;v++)
    {
        QString result = QString::number( v, 16 ).toUpper();
        QTableWidgetItem *itm = new QTableWidgetItem();
        QTableWidgetItem *empty = new QTableWidgetItem();
        itm->setText(result);
        empty->setText("0000");
        ui->ram_tb->insertRow(v);
        ui->ram_tb->setItem(v,1,itm);
        ui->ram_tb->setItem(v,3,empty);

    }
}

void MainWindow::resetRam()
{
    for(int i=0;i<4096;i++)
    {
        ram[i].reset();
    }
}

void MainWindow::printReg()
{
    ui->sc_line->setText(QString::number( SC.to_ulong(), 16 ).toUpper());
    ui->pc_line->setText(QString::number( PC.to_ulong(), 16 ).toUpper());
    ui->ar_line->setText(QString::number( AR.to_ulong(), 16 ).toUpper());
    ui->ir_line->setText(QString::number( IR.to_ulong(), 16 ).toUpper());
    ui->dr_line->setText(QString::number( DR.to_ulong(), 16 ).toUpper());
    ui->ac_line->setText(QString::number( AC.to_ulong(), 16 ).toUpper());
    ui->tr_line->setText(QString::number( TR.to_ulong(), 16 ).toUpper());
    ui->inpr_line->setText(QString::number( INPR.to_ulong(), 16 ).toUpper());
    ui->outr_line->setText(QString::number( OUTR.to_ulong(), 16 ).toUpper());
    ui->i_line->setText(QString::number( I.to_ulong(), 16 ).toUpper());
    ui->s_line->setText(QString::number( S.to_ulong(), 16 ).toUpper());
    ui->e_line->setText(QString::number( E.to_ulong(), 16 ).toUpper());
    ui->r_line->setText(QString::number( R.to_ulong(), 16 ).toUpper());
    ui->ien_line->setText(QString::number( IEN.to_ulong(), 16 ).toUpper());
    ui->fgi_line->setText(QString::number( FGI.to_ulong(), 16 ).toUpper());
    ui->fgo_line->setText(QString::number( FGO.to_ulong(), 16 ).toUpper());

}

void MainWindow::printTable()
{
    if(reseter)
    {
        ui->ram_tb->setRowCount(0);
        for(int v=0;v<4096;v++)
        {
            QString address = QString::number( v, 16 ).toUpper();
            QTableWidgetItem *itmaddr = new QTableWidgetItem();
            QTableWidgetItem *itmHex = new QTableWidgetItem();
            itmaddr->setText(address);
            itmHex->setText("0000");
            ui->ram_tb->insertRow(v);
            ui->ram_tb->setItem(v,1,itmaddr);
            ui->ram_tb->setItem(v,3,itmHex);
            reseter=0;
        }
    }
    else
    {
        for(int v=0;v<4096;v++)
        {
            QString address = QString::number( v, 16 ).toUpper();
            QTableWidgetItem *itmaddr = new QTableWidgetItem();
            QTableWidgetItem *itmHex = new QTableWidgetItem();
            itmaddr->setText(address);
            QString checkerString =QString::number( ram[v].to_ulong(), 16 ).toUpper();
            if(checkerString.size()==3){
                checkerString = "0" + checkerString;
            }
            else if(checkerString.size()==2){
                checkerString = "00" + checkerString;
            }
            else if(checkerString.size()==1){
                checkerString = "000" + checkerString;
            }
            itmHex->setText(checkerString);
            ui->ram_tb->setItem(v,1,itmaddr);
            ui->ram_tb->setItem(v,3,itmHex);

        }
    }
}

bool MainWindow::isNumber(const QString &str)
{
    string check=str.toStdString();
    for (char const &c : check)
    {
        if(c=='a' || c=='b' || c=='c' || c=='d' || c=='e' || c=='f'|| c=='A' || c=='B' || c=='C' || c=='D' || c=='E' || c=='F'|| c=='-')
        {
            continue;
        }
        if (isdigit(c) == 0) return false;
    }
       return true;
}



void MainWindow::arithmeticUnitCIR() {
    E[0] = AC[0];
    AC = AC >> 1;
    AC[15] = E[0];
}

/**
 * @brief Performs a circular left shift
 */
void MainWindow::arithmeticUnitCIL() {
    E[0] = AC[15];
    AC = AC << 1;
    AC[0] = E[0];
}








void MainWindow::on_action_Exit_triggered()
{
    QMessageBox::warning(this, tr("My Application"),
                                   tr("The document has been modified.\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard
                                   | QMessageBox::Cancel,
                                   QMessageBox::Save);
    this->close();

}

void MainWindow::on_savebtn_clicked()
{
    if(issaved=="")
    {
        on_actionSave_as_triggered();
    }
    else
    {
        ofstream f;
        f.open(issaved.toLocal8Bit(),ios::out);
        f<<ui->editor->toPlainText().toStdString();
        f.flush();
        f.close();

    }

}

void MainWindow::on_actionSave_as_triggered()
{
    /*
    QString fileName = QFileDialog::getSaveFileName(this,tr("Save"), "",tr("Mytext editor file (*.bxb)"));

    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(ui->editor->toPlainText().toLocal8Bit());
    }
    file.close();
    */

    QString fileName = QFileDialog::getSaveFileName(this,tr("Save"), "",tr("Mytext editor file (*.bxb)"));
    if (fileName.isEmpty())
         return;
     else {


        ofstream f;
        f.open(fileName.toLocal8Bit(),ios::out);
        f<<ui->editor->toPlainText().toStdString();
        f.flush();
        f.close();

        issaved=fileName;
    }

}

void MainWindow::on_action_Save_triggered()
{
    on_savebtn_clicked();
}

void MainWindow::on_openbtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Text file"), "",tr("Myeditor file (*.bxb)"));
    if (fileName.isEmpty())
        return;
    else {
        ui->editor->clear();
        issaved=fileName;
        ifstream infile;
        infile.open(fileName.toLocal8Bit());
        char tmp[300];
        while(infile.eof()!=true)
        {
            //infile>>tmp;
            infile.getline(tmp,300);
            ui->editor->insertPlainText(QString::fromStdString(tmp));
            ui->editor->insertPlainText("\n");

        }


    }


}

void MainWindow::on_action_Open_triggered()
{
    on_openbtn_clicked();
}

void MainWindow::on_action_New_triggered()
{
    ui->editor->clear();
    issaved="";
}

void MainWindow::on_newbtn_clicked()
{
    on_action_New_triggered();
}

void MainWindow::on_reset_btn_clicked()
{
    reseter=1;
    SC.reset();
    PC.reset();
    AR.reset();
    IR.reset();
    DR.reset();
    AC.reset();
    TR.reset();
    INPR.reset();
    OUTR.reset();
    I.reset();
    S.reset();
    E.reset();
    R.reset();
    IEN.reset();
    FGI.reset();
    FGO.reset();
    resetRam();
    printTable();
    printReg();
    ui->console->setText("");
    compiled=0;


}

void MainWindow::on_compile_btn_clicked()
{
    compiled=1;
    int lc=0;
    ui->console->setText("");
    QStringList commands = ui->editor->toPlainText().split('\n', QString::SkipEmptyParts);
    int tcommmands=commands.size();
    int endp1=0;
    for(int i=0;i<tcommmands;i++)
    {
        //each lines

        QStringList riz = commands.at(i).split(' ', QString::SkipEmptyParts);
//        int tparts=commands.size();

            //each words
            if(riz.at(0)=="//" || riz.at(0)[0]=='/')
            {
                //comment
                //ui->console->setText("comment in line:"+QString::number(i+1)+"\n");
                continue;
            }
            else if(riz.at(0)=="ORG")
            {
                if(isNumber(riz.at(1)))
                {
                    bool ok=1;
                    lc=riz.at(1).toInt(&ok,16);

                }
                else
                {
                    ui->console->setText("error in line:"+QString::number(i+1)+"\n You need hex number after ORG. \n");
                    compiled=0;
                    break;
                }
            }
            else if(riz.at(0)[riz.at(0).size()-1]==',')
            {
                allDatas[riz.at(0)]=i+1;
                QTableWidgetItem *itmlabel = new QTableWidgetItem();
                itmlabel->setText(riz.at(0));
                ui->ram_tb->setItem(lc,0,itmlabel);
                QTableWidgetItem *itmintraction = new QTableWidgetItem();
                itmintraction->setText(commands.at(i));
                ui->ram_tb->setItem(lc,2,itmintraction);
                int convertNumber;
                if(riz.at(1)=="HEX")
                {
                    if(isNumber(riz.at(2)))
                    {
                        bool ok=1;
                        convertNumber=riz.at(2).toInt(&ok,16);
                        ram[lc]=convertNumber;
                    }
                    else
                    {
                        ui->console->setText("error in line:"+QString::number(i+1)+"\n You need valid number after HEX. \n");
                        compiled=0;
                        break;
                    }
                }
                else if(riz.at(1)=="DEC")
                {
                    if(isNumber(riz.at(2)))
                    {
                        bool ok=1;
                        convertNumber=riz.at(2).toInt(&ok,10);
                        ram[lc]=convertNumber;
                    }
                    else
                    {
                        ui->console->setText("error in line:"+QString::number(i+1)+"\n You need valid number after DEC. \n");
                        compiled=0;
                        break;
                    }
                }
                //ui->console->setText("test label:"+QString::number(allDatas["test,"])+"\n");
                lc++;
                continue;
            }
            ///////////////////////////////////////////////////////////////////////////////////////////////

            //TO DO










            ///////////////////////////////////////////////////////////////////////////////////////////////
            else if(riz.at(0)=="END")
            {
                endp1=1;
                printTable();
                if(compiled)
                {
                    ui->console->setText("Compiled Successfully!\n");
                }
                else
                {
                    ui->console->append("\nERROR IN COMPILING!\n");
                }
                break;
            }
            else
            {
                //bad command
                ui->console->setText("error in line:"+QString::number(i+1)+"\n Bad Command. \n");
                compiled=0;
                break;
            }




        //first for
    }
    if(!endp1)
    {
        ui->console->setText("error in compiling \n there is no END!\n");
        compiled=0;
    }


}

void MainWindow::on_run_btn_clicked()
{
    if(!compiled)
    {
        ui->console->setText("\n You should compile your program first!\n");
    }
    else
    {

    }
}

void MainWindow::on_next_btn_clicked()
{
    if(!compiled)
    {
        ui->console->setText("\n You should compile your program first!\n");
    }
    else
    {

    }
}
