#include "myfilterproxymodel.h"

#include <QStandardItemModel>

MyFilterProxyModel::MyFilterProxyModel()
{

}


bool MyFilterProxyModel::clearVec()
{
    vec.clear();
}






//bool MyFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex & source_parent) const
//{
//    // custom behaviour :
//    if(filterRegExp().isEmpty()==false)
//    {
//        // get source-model index for current row
//        QModelIndex source_index = sourceModel()->index(source_row, this->filterKeyColumn(), source_parent) ;
//        if(source_index.isValid())
//        {
//            // if any of children matches the filter, then current index matches the filter as well
//            int i, nb = sourceModel()->rowCount(source_index) ;
//            for(i=0; i<nb; ++i)
//            {
//                if(filterAcceptsRow(i, source_index))
//                {
//                    return true ;
//                }
//            }
//            // check current index itself :
//            QString key = sourceModel()->data(source_index, filterRole()).toString();
//            return key.contains(filterRegExp()) ;
//        }
//    }
//    // parent call for initial behaviour
//    return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent) ;
//}




bool MyFilterProxyModel::filterAcceptsRow ( int source_row, const QModelIndex & source_parent )const
{
     smatch result;
     QRegExp mReg = filterRegExp();

/*    QModelIndex source_index;
    printf("%d %s\n", sourceModel()->rowCount(), source_parent.data().toString().toStdString().c_str());
    if (filterKeyColumn() == 2 ) {
        source_index = source_parent.child(source_row, filterKeyColumn());
        if(QSortFilterProxyModel::filterAcceptsRow(source_row, source_index)){
            return true;
        }
        return false;
    } else if(filterKeyColumn() == 0){
        if(source_parent.data().toString().isEmpty()){
            return true;
        } else {
            //return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
        }
    }
    printf("%d, %s\n", source_row, sourceModel()->data());
    return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
*/

//    if (source_parent == qobject_cast<QStandardItemModel*>(sourceModel())->invisibleRootItem()->index())
//    {
//        // always accept children of rootitem, since we want to filter their children
//        return true;
//    }

    if(filterKeyColumn() == 0){

        if(QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent)){
            vec.push_back(sourceModel()->index(source_row, filterKeyColumn()).data().toString().toStdString().c_str());
            return true;
        } else if(!source_parent.data().toString().isEmpty() && std::find(vec.begin(), vec.end(), source_parent.data().toString().toStdString()) != vec.end()){
            return true;
        }
    } else if(filterKeyColumn() == 1){
        if(source_parent.data().toString().isEmpty()){
            QModelIndex index = sourceModel()->index(source_row, 0);
            if(QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent)){
                vec.push_back(sourceModel()->index(source_row, 0).data().toString().toStdString().c_str());
                return true;
            }
            for(int i = 0; i <=  sourceModel()->rowCount(index); i++){
                QModelIndex child = index.child(i, filterKeyColumn());
                if(mReg.exactMatch(child.data().toString())){
                    vec.push_back(index.data().toString().toStdString().c_str());
                    return true;
                }
            }
        }else{
            if(std::find(vec.begin(), vec.end(), source_parent.data().toString().toStdString()) != vec.end()){
                return true;
            }
        }

    } else if(filterKeyColumn() == 2){
        if(source_parent.data().toString().isEmpty()){
            QModelIndex index = sourceModel()->index(source_row, 0);
            //printf("%s || %d \n", index.data().toString().toStdString().c_str(), sourceModel()->rowCount(index));
            for(int i = 0; i <=  sourceModel()->rowCount(index); i++){
                QModelIndex child = index.child(i, filterKeyColumn());
                if(mReg.exactMatch(child.data().toString())){
                    vec.push_back(index.data().toString().toStdString().c_str());
                    return true;
                }
            }
        }else{
            if(std::find(vec.begin(), vec.end(), source_parent.data().toString().toStdString()) != vec.end()){
                return true;
            }
        }
    }

    return false;
}
