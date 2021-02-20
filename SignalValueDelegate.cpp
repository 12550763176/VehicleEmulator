#include "Channel.h"
#include "DataManager.h"
#include "DataCalcManager.h"
#include "SignalValueDelegate.h"

#include <qcombobox.h>

SignalValueDelegate::SignalValueDelegate(QStandardItemModel* model, QObject *parent): QStyledItemDelegate(parent)
{
    treeModel = model;
    //vec = DBCDatabase::getInstace()->getList();
}




QWidget *SignalValueDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &item,
    const QModelIndex & index ) const
{

    if(index.column() == 7) //只对第4列采用此方法编辑
    {
        QComboBox* box = new QComboBox(parent);
        box->setEditable(true);
        Msg* msg;
        string idstr;
        string channel;
        string sigName;
        if(index.parent().row() != -1){
            idstr = index.parent().data().toString().toStdString();
            channel = index.parent().siblingAtColumn(1).data().toString().toStdString();
        } else {
            idstr = index.siblingAtColumn(0).data().toString().toStdString();
            channel =  index.siblingAtColumn(1).data().toString().toStdString();
            if(channel  == CAN) {
                box->setEditable(false);
                return box;
            }
        }
        sigName =  index.siblingAtColumn(2).data().toString().toStdString();

        uint16_t canId;
        sscanf(idstr.c_str(), "0x%x", &canId);

        for (MsgFile* it : DataManager::getInstace()->getList()) {
            if(strcmp(it->getChannel().c_str(), channel.c_str()) == 0){
                msg = &(it->getMsg()[canId]);
            }
        }

        sigName = Utils::trim(sigName);

        QStringList list;
        list << "";
        if(msg->channel == CAN){
            for( std::pair<string, string> vm  :msg->sig[sigName].valueMap.map){
                list << (vm.second + "  :" + vm.first).c_str();
            }
        }
        list<<"Random";
        box->addItems(list);
        return box;
    }
    return NULL;
}

void SignalValueDelegate::setEditorData(QWidget *editor,
                                          const QModelIndex &index) const
{

    QString value = index.model()->data(index, Qt::EditRole).toString();
    QComboBox* box = static_cast<QComboBox*>(editor);

    Msg* msg;
    string idstr;
    string channel;
    string sigName;
    if(index.parent().row() != -1){
        idstr = index.parent().data().toString().toStdString();
        channel = index.parent().siblingAtColumn(1).data().toString().toStdString();
    } else {
        idstr = index.siblingAtColumn(0).data().toString().toStdString();
        channel =  index.siblingAtColumn(1).data().toString().toStdString();
    }

    sigName = index.siblingAtColumn(2).data().toString().toStdString();
    uint16_t canId;
    sscanf(idstr.c_str(), "0x%x", &canId);

    sigName = Utils::trim(sigName);

    //get sigValue
    string sigValue = value.toStdString();
    sigValue = Utils::trim(sigValue);

    for (MsgFile* it : DataManager::getInstace()->getList()) {
        if(strcmp(it->getChannel().c_str(), channel.c_str()) == 0){
            msg = &(it->getMsg()[canId]);
        }
    }

    if(sigValue.compare("Random") == 0){
        DataCalcManager::getInstance().addRandom(*msg, sigName);
        return;
    }else{
        DataCalcManager::getInstance().delRandom(*msg, sigName);
    }

    if(sigValue.empty()) { return;}

    if(!Utils::isNum(sigValue)){
        vector<string> values = Utils::split(sigValue, ":");
        if(values.size()==2){
            sigValue =  values[1];
            sigValue = Utils::trim(sigValue);
            if(!Utils::isNum(sigValue)){
                sigValue = "";
            }
        } else {
            sigValue = "";
        }
    }
    bool isUpdata = DataCalcManager::getInstance().updateData(*msg, sigName, sigValue);
    box->setEditText(QString(sigValue.c_str()));
}


void SignalValueDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QComboBox* box = static_cast<QComboBox*>(editor);
    model->setData(index, box->currentText(), Qt::EditRole);

}

void SignalValueDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index/* index */) const
{
    editor->setGeometry(option.rect);

}
