#ifndef SIGNALVALUEDELEGATE_H
#define SIGNALVALUEDELEGATE_H

#include "DBCFile.h"

#include <QStyledItemDelegate>
#include <qstandarditemmodel.h>



class SignalValueDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    SignalValueDelegate(QStandardItemModel* model, QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const override;
private:
    vector<DBCFile*> vec;
    QStandardItemModel *treeModel;
};


#endif // SIGNALVALUEDELEGATE_H
