#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "DBCFile.h"
#include "SignalValueDelegate.h"
#include "myfilterproxymodel.h"

#include <QMainWindow>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:

    void treeItemChanged(QStandardItem * item);

private slots:


    void on_pushButton_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_pushButton_2_clicked();
    void formatFile(std::string path);

    void on_listView_doubleClicked(const QModelIndex &index);
    void refresh_treeView();

    void on_treeView_clicked(const QModelIndex &index);

    void on_treeView_doubleClicked(const QModelIndex &index);

    void on_pushButton_3_clicked();
    void slot_treeView_pressed(QModelIndex modeIndex);
    void initHeader(bool newParameter = false);
    void initFilter();
    void filteringData();
    void addMsg(vector<MsgFile*> vec);
    void delMsg(vector<MsgFile*> vec);

    void on_NameFilter_editingFinished();

    void on_IdFilter_editingFinished();

    void on_DesFilter_editingFinished();

    void on_filter_clicked();

    void on_checkBox_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
    bool mState = false;
    std::string mDeviceId;
    bool isRefresh = false;
    QSortFilterProxyModel* idFilter = new QSortFilterProxyModel;
    QSortFilterProxyModel* nameFilter = new QSortFilterProxyModel;
    QSortFilterProxyModel* desFilter = new QSortFilterProxyModel;
    MyFilterProxyModel* filter = new MyFilterProxyModel;
    SignalValueDelegate* sigDelegate ;



};
#endif // MAINWINDOW_H
