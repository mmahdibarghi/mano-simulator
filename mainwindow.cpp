#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFileDialog>
#include<fstream>
#include<QFile>
#include<QMessageBox>
#include<sstream>
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
    ui->ram_tb->setRowCount(0);
    for(int v=0;v<4096;v++)
    {
        QString result = QString::number( v, 16 ).toUpper();
        QTableWidgetItem *itm = new QTableWidgetItem();
        QTableWidgetItem *empty = new QTableWidgetItem();
        itm->setText(result);
        empty->setText(QString::number( ram[v].to_ulong(), 16 ).toUpper());
        ui->ram_tb->insertRow(v);
        ui->ram_tb->setItem(v,1,itm);
        ui->ram_tb->setItem(v,3,empty);

    }
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


}

void MainWindow::on_compile_btn_clicked()
{
    ui->console->setText("");
    QStringList commands = ui->editor->toPlainText().split('\n', QString::SkipEmptyParts);
    int tcommmands=commands.size();
    for(int i=0;i<tcommmands;i++)
    {
        //each lines

        QStringList riz = commands.at(i).split(' ', QString::SkipEmptyParts);
        int tparts=commands.size();
        for (int j=0;j<tparts;j++)
        {
            //each words
            if(riz.at(0)=="//")
            {
                //comment
                ui->console->setText("error in line:"+QString::number(i+1)+"\n");
                break;
            }
            else
            {

            }

        }

    }


}
