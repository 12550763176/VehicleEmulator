#ifndef HEADERVIEW_H
#define HEADERVIEW_H

#include "mainwindow.h"

#include <QObject>
#include <QHeaderView>
#include <QPainter>
#include <QCheckBox>
#include <QStandardItemModel>
#include <QMouseEvent>
#include <QTreeView>
#include <QComboBox>

#include <QHeaderView>

class HeaderView : public QHeaderView

{
        Q_OBJECT

public:

       HeaderView(Qt::Orientation orientation, QWidget *parent = 0, MainWindow* mainWin = 0);

       void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;
       void mousePressEvent(QMouseEvent *event);
       void mouseReleaseEvent(QMouseEvent *event);
       bool event(QEvent *event);
       HeaderView* setTreeModel(QStandardItemModel* model);

       void selectAll(bool b);
       void selectRandom(bool checked);
public slots:
       void onStateChanged(int state);


private:
       bool m_bPressed;
       bool m_bChecked;
       bool m_bTristate;
       bool mRandomCheck;
       bool m_bMoving;
//       QCheckBox checkBox;
//       QCheckBox randomBox;

       QTreeView* treeView;
       MainWindow* win;
       QStandardItemModel* treeModel;
};


#endif // HEADERVIEW_H
