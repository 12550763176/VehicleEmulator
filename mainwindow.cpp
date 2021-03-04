#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "SocketMoudle.h"
#include "QFileDialog"
#include "QStandardItemModel"
#include "DBCFile.h"
#include "DataManager.h"
#include "SignalValueDelegate.h"
#include "DataCalcManager.h"
#include "MsgFile.h"
#include "MDBFile.h"
#include "HeaderView.h"

#include <string>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <qtextcodec.h>
#include <QClipboard>


QStandardItemModel* itemModel;
DataManager* database;
QStandardItemModel *treeModel;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //ui->pushButton->setIcon();
    ui->setupUi(this);
    itemModel = new QStandardItemModel(this);
    database =  DataManager::getInstace();
    treeModel = new QStandardItemModel(ui->treeView);

    connect(ui->treeView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(slot_treeView_pressed(QModelIndex)));
    //connect(treeModel, &QStandardItemModel::itemChanged, this, &MainWindow::treeItemChanged);

    initHeader();
    initFilter();
}

void  MainWindow::initHeader(bool refresh)
{

    QTreeView* tree = ui->treeView;
    QStringList lables;
    lables.push_back(QStringLiteral("canID"));
    lables.push_back(QStringLiteral("type"));
    lables.push_back(QStringLiteral("canName/SignalName"));
    lables.push_back(QStringLiteral("Signal Description"));
    lables.push_back(QStringLiteral("max"));
    lables.push_back(QStringLiteral("min"));
    lables.push_back(QStringLiteral("defaultValue"));
    lables.push_back(QStringLiteral("value"));
    lables.push_back("...");
    treeModel->setHorizontalHeaderLabels(lables);

    if(refresh){
        tree->setColumnWidth(0, 100);
        tree->setColumnWidth(1, 100);
        tree->setColumnWidth(2, 400);
        tree->setColumnWidth(3, 500);
        tree->setColumnWidth(4, 60);
        tree->setColumnWidth(5, 60);
        tree->setColumnWidth(6, 60);
        tree->setColumnWidth(7, 600);
        tree->setColumnWidth(8, 15);
        return;
    }
    HeaderView* header = new HeaderView(Qt::Horizontal, tree, this);
    header->setTreeModel(treeModel);
    tree->setHeader(header);

    tree->header()->setModel(treeModel);
    tree->header()->setDefaultAlignment(Qt::AlignCenter);
    tree->header()->setHighlightSections(true);
    tree->header()->setStretchLastSection(true);
    tree->header()->setEditTriggers(QTreeView::AllEditTriggers);
    //tree->header()->setSortIndicator(1,Qt::AscendingOrder);
    //header()->resizeColumnsToContents()
    tree->setSortingEnabled(true);
    //tree->setHeader(header);
    connect(treeModel, &QStandardItemModel::itemChanged, this, &MainWindow::treeItemChanged);

    sigDelegate = new SignalValueDelegate(treeModel);
    tree->setSelectionBehavior(QTreeView::SelectItems);			//一次选中item
    tree->setColumnWidth(0, 100);
    tree->setColumnWidth(1, 100);
    tree->setColumnWidth(2, 400);
    tree->setColumnWidth(3, 500);
    tree->setColumnWidth(4, 60);
    tree->setColumnWidth(5, 60);
    tree->setColumnWidth(6, 60);
    tree->setColumnWidth(7, 600);
    tree->setColumnWidth(8, 15);


    tree->setFocusPolicy(Qt::StrongFocus);

    tree->setModel(treeModel);


    tree->setItemDelegate(new SignalValueDelegate(treeModel));

    tree->setEditTriggers(QTreeView::AllEditTriggers);

}

void MainWindow::initFilter()
{

    ui->type->clear();
    ui->type->addItem("ID");
    ui->type->addItem("Name");
    ui->type->addItem("Description");



    filter->setSourceModel(treeModel);
//    idFilter->setSourceModel(treeModel);
//    idFilter->setFilterKeyColumn(1);

//    nameFilter->setSourceModel(treeModel);
//    nameFilter->setFilterKeyColumn(2);

//    desFilter->setSourceModel(treeModel);
//    desFilter->setFilterKeyColumn(3);


//    ui->treeView->setModel(idFilter);
//    ui->treeView->setModel(nameFilter);
//    ui->treeView->setModel(desFilter);
    ui->treeView->setModel(filter);

}




void MainWindow::slot_treeView_pressed(QModelIndex modeIndex)
{
      ui->treeView->resizeColumnToContents(modeIndex.row());
      QList<QVariant> list = ui->treeView->model()->itemData(modeIndex).values();
      if(list.size() > 0){
          QString selectedRowTxt = ui->treeView->model()->itemData(modeIndex).values()[0].toString();
          QClipboard* clipboard = QApplication::clipboard();
          clipboard->setText(selectedRowTxt);
          //printf("slot_treeView_pressed %s\n", selectedRowTxt.toStdString().c_str());
      }
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->comboBox->clear();

    vector<string>  vec = SocketMoudle::getInstance().listDevices();
    for(string  s: vec){
        ui->comboBox->addItem(s.c_str());
    }
}


void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    mDeviceId = arg1.toStdString();
    SocketMoudle::getInstance().selectDevice(mDeviceId);
}


void MainWindow::on_pushButton_2_clicked()
{
    QFileDialog* dialog = new QFileDialog(this);
    dialog->setWindowTitle(QStringLiteral("选文件"));
    dialog->setDirectory("");
    dialog->setNameFilter(tr("File(*.xls or *.dbc)"));
    //dialog->setNameFilter(tr("File(*.dbc)"));

    dialog->setFileMode(QFileDialog::ExistingFiles);
    dialog->setViewMode(QFileDialog::Detail);
    QStringList fileNames;

    if(dialog->exec()){
        fileNames = dialog->selectedFiles();
    }

    vector<MsgFile*> vec;

    for(QString s: fileNames){
        QList<QStandardItem*> list = itemModel->findItems(s);
        if(list.length() == 0){
            MsgFile* file;
            //QStandardItem* tmp = new QStandardItem(s);
            if(s.endsWith("xls")){
                file = new MDBFile(s.toStdString());
            } else if (s.endsWith("dbc")){
                file = new DBCFile(s.toStdString());
            }
            vec.push_back(file);
            database->put(s.toStdString(), file);
        }
    }
    //解析导入的文件
    database->parse();
    for(auto file: vec){
       QStandardItem* it = new QStandardItem(file->getPath().c_str());
       it->setTextAlignment(Qt::AlignRight);
       itemModel->appendRow(it);//new QStandardItem(s));
    }
    ui->listView->setModel(itemModel);
    //显示导入的文件
    addMsg(vec);
    //refresh_treeView();
}

void MainWindow::formatFile(std::string path) {}

void  MainWindow::addMsg(vector<MsgFile*> vec){
    //QAbstractItemDelegate *delegate =  ui->treeView->itemDelegate();

    isRefresh = true;
    int i = treeModel->rowCount();
    treeModel->columnCount();
    for (MsgFile* it : vec) {
        for(std::pair<uint16_t, Msg> frame  : it->getMsg()){
            char tmp[200];
            if(frame.second.id == 0) continue;
            sprintf(tmp,"0x%04X", frame.second.id);
            QStandardItem* id = new QStandardItem(QString(tmp));
            id->setEditable(false);
            treeModel->setItem(i,0, id);
            treeModel->setItem(i,1, new QStandardItem(QString(frame.second.channel.c_str())));

            treeModel->setItem(i,2, new QStandardItem(QString(frame.second.name.c_str())));
            treeModel->setItem(i,3, new QStandardItem(QString(std::to_string(frame.second.interval).c_str())));
            treeModel->item(i)->setCheckable(true);

            treeModel->item(i)->setCheckState(Qt::CheckState(frame.second.isSelect ? Qt::CheckState::Checked : Qt::CheckState::Unchecked));
            //connect(treeModel, &QStandardItemModel::itemChanged, this, &MainWindow::treeItemChanged);
            QList<QStandardItem*> sigList ;
            for(std::pair<string, Signal> sig : frame.second.sig){
                sigList.clear();
                QStandardItem* item = new QStandardItem("");
                item->setEnabled(false);
                sigList.append(item);
                sigList.append(item);

                sigList.append(new QStandardItem(tr("\t") + tr(sig.second.sigName.c_str())));
                sigList.append(new QStandardItem(sig.second.description.comment.c_str()));
                sigList.append(new QStandardItem(sig.second.max.c_str()));
                sigList.append(new QStandardItem(sig.second.min.c_str()));
                sigList.append(new QStandardItem(QString::fromLocal8Bit(sig.second.defaultValue.c_str())));
                sigList.append(new QStandardItem(""));

                treeModel->item(i)->appendRow(sigList);

            }
            i++;
        }
    }

    isRefresh = false;

    //refresh_treeView();
}

void  MainWindow::delMsg(vector<MsgFile*> vec){
    refresh_treeView();
}



void MainWindow::refresh_treeView(){

    treeModel->clear();

    isRefresh = true;
    initHeader(true);

    QList<QStandardItem*> itms;
    int i = 0;
    for (MsgFile* it : DataManager::getInstace()->getList()) {
        for(std::pair<uint16_t, Msg> frame  : it->getMsg()){
            char tmp[200];
            if(frame.second.id == 0) continue;
            sprintf(tmp,"0x%04X", frame.second.id);
            QStandardItem* id = new QStandardItem(QString(tmp));
            id->setEditable(false);
            treeModel->setItem(i,0, id);
            treeModel->setItem(i,1, new QStandardItem(QString(frame.second.channel.c_str())));

            treeModel->setItem(i,2, new QStandardItem(QString(frame.second.name.c_str())));
            treeModel->setItem(i,3, new QStandardItem(QString(std::to_string(frame.second.interval).c_str())));
            treeModel->item(i)->setCheckable(true);

            treeModel->item(i)->setCheckState(Qt::CheckState(frame.second.isSelect ? Qt::CheckState::Checked : Qt::CheckState::Unchecked));
            //connect(treeModel, &QStandardItemModel::itemChanged, this, &MainWindow::treeItemChanged);

            QList<QStandardItem*> sigList ;
            for(std::pair<string, Signal> sig : frame.second.sig){
                sigList.clear();
                QStandardItem* item = new QStandardItem("");
                item->setEnabled(false);
                sigList.append(item);
                sigList.append(item);

                sigList.append(new QStandardItem(tr("\t") + tr(sig.second.sigName.c_str())));
                sigList.append(new QStandardItem(sig.second.description.comment.c_str()));
                sigList.append(new QStandardItem(sig.second.max.c_str()));
                sigList.append(new QStandardItem(sig.second.min.c_str()));
                sigList.append(new QStandardItem(QString::fromLocal8Bit(sig.second.defaultValue.c_str())));
                sigList.append(new QStandardItem(""));

                treeModel->item(i)->appendRow(sigList);
            }
            i++;
        }
    }


//    tree->setSelectionMode(QTreeView::MultiSelection);
    isRefresh = false;

    //    tree->setAlternatingRowColors(true);
    //    tree->setSelectionBehavior(QTreeView::SelectRows);
    //tree->setItemDelegate(new QTreeView(&tree));
    //ui->treeView->header()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->treeView->resizeColumnToContents(0);
    //ui->treeView->setSectionResizeMode(QHeaderView::Stretch);

    //ui->treeView->resizeColumnToContents(1);
}


void MainWindow::filteringData()
{


}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
   QStandardItem* item = itemModel->itemFromIndex(index);
   //printf( "%s  %d  %d\n", item->text().toStdString().c_str(), index.row(), index.column());
   database->del(item->text().toStdString());


   /*
   QDialog* dialog = new QDialog(this);
   dialog->setWindowTitle("del");
   dialog->show();
   */
   database->parse();


   itemModel->removeRow(index.row());
   ui->listView->update();
   delMsg(vector<MsgFile*>());
}


void MainWindow::treeItemChanged(QStandardItem * item){
    int canId;
    Msg* msg;
    if(isRefresh ) return; //|| (item->column()!= 7 && item->column() != 0))

//    printf("treeItemChanged\n");

    if(item->parent()){
        sscanf(item->parent()->text().toStdString().c_str(), "0x%x", &canId);
    }else {
        sscanf(treeModel->item(item->row(),0)->text().toStdString().c_str(), "0x%x", &canId);
    }
//    printf("treeItemChanged %x\n", canId);

    for (MsgFile* it : DataManager::getInstace()->getList()) {
        if(strcmp(it->getChannel().c_str(), treeModel->item(item->row(),1)->text().toStdString().c_str()) == 0){
            msg = &(it->getMsg()[canId]);
        }
    }
//    if(item->column() == 7 &&  msg->channel == SPI){
//        DataCalcManager::getInstance().updateData(*msg, "",item->text().toStdString());
//    }else if(item->column() == 7 &&  msg->channel == CAN  && item->parent()){
//        string signame = item->parent()->child(item->row(),2)->text().toStdString();
//        signame = Utils::trim(signame);
//        DataCalcManager::getInstance().updateData(*msg, signame, item->text().toStdString());
//    } else
    if(item->column() == 0){
        if(item->checkState()){
            msg->isSelect = true;
            DataCalcManager::getInstance().send(*msg);
        } else {
            msg->isSelect = false;
            DataCalcManager::getInstance().del(*msg);
        }
    }
    //printf("treeItemChanged end %x\n", canId);

}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
}

void MainWindow::on_treeView_doubleClicked(const QModelIndex &index)
{
}

void MainWindow::on_pushButton_3_clicked()
{
    if(!mState){
        string selectId = SocketMoudle::getInstance().getDeviceId();
        if(selectId.empty()){
            QMessageBox::about(NULL, "About", "please select device");
            return;
        }
        ui->pushButton_3->setText(QString(">>"));
        SocketMoudle::getInstance().stopSocket();
        SocketMoudle::getInstance().startSocket();
    } else {
        ui->pushButton_3->setText(QString("II"));
        SocketMoudle::getInstance().stopSocket();
    }
     mState = !mState;
}



void MainWindow::on_NameFilter_editingFinished()
{
    printf("Name: %s  %s\n", ui->NameFilter->text().toStdString().c_str(), nameFilter->filterRegExp().pattern().toStdString().c_str());
    QRegExp regExp( ui->NameFilter->text().toStdString().c_str(), Qt::CaseInsensitive, QRegExp::Wildcard);
    nameFilter->setFilterRegExp(regExp);

}

void MainWindow::on_IdFilter_editingFinished()
{
    printf("id: %s  %s \n", ui->IdFilter->text().toStdString().c_str(), idFilter->filterRegExp().pattern().toStdString().c_str());
    QRegExp regExp("0x07*", Qt::CaseInsensitive, QRegExp::Wildcard);
    idFilter->setFilterRegExp(regExp);
}

void MainWindow::on_DesFilter_editingFinished()
{
    printf("des: %s  %s\n", ui->DesFilter->text().toStdString().c_str(), desFilter->filterRegExp().pattern().toStdString().c_str());
    QRegExp regExp( ui->DesFilter->text().toStdString().c_str(), Qt::CaseInsensitive, QRegExp::Wildcard);
    desFilter->setFilterRegExp(regExp);
}

void MainWindow::on_filter_clicked()
{

    filter->clear();
    QString type = ui->type->currentText();
    printf("des: on_filter_clicked  %s \n", type.toStdString().c_str());

    if(type == QString::fromLocal8Bit("ID")){
        filter->setFilterKeyColumn(0);
    } else if(type == QString::fromLocal8Bit("Name")){
        filter->setFilterKeyColumn(2);
    } else if(type == QString::fromLocal8Bit("Description")){
        filter->setFilterKeyColumn(3);
    }
    string expStr = "*";
    expStr.append(ui->lineEdit->text().toStdString());
    expStr.append("*");
    QRegExp regExp(expStr.c_str(), Qt::CaseInsensitive, QRegExp::Wildcard);
    filter->setFilterRegExp(regExp);
    ui->treeView->setModel(filter);
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    Msg* msg;

    for (MsgFile* it : DataManager::getInstace()->getList()) {
            unordered_map<uint16_t, Msg> map  = it->getMsg();
            for(std::pair<uint16_t, Msg> pair : map){
                  msg = &(it->getMsg()[pair.first]);
                  for(std::pair<string, Signal> sigName : msg->sig){
                    if(arg1){
                        DataCalcManager::getInstance().addRandom(*msg, sigName.first);
                    }else{
                        DataCalcManager::getInstance().delRandom(*msg, sigName.first);
                    }
                  }
            }

        }
    }


