#ifndef MYFILTERPROXYMODEL_H
#define MYFILTERPROXYMODEL_H

#include <qsortfilterproxymodel.h>
#include<regex>

#include <vector>

using namespace std;


class MyFilterProxyModel : public QSortFilterProxyModel
{
private:
    mutable vector<string> vec;
    regex* patern;

public:
    MyFilterProxyModel();
    bool clearVec();
    void setReg(string str);
    bool filterAcceptsRow (int source_row, const QModelIndex & source_parent ) const;
};

#endif // MYFILTERPROXYMODEL_H
