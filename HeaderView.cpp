#include "DataCalcManager.h"
#include "HeaderView.h"

#include <QLabel>

HeaderView::HeaderView(Qt::Orientation orientation, QWidget *parent, MainWindow* mainWin)
    : QHeaderView(orientation, parent),
      m_bPressed(false),
      m_bChecked(false),
      m_bTristate(false),
      m_bMoving(false),
      win(mainWin)
{
//     setStretchLastSection(true);

    treeView = (QTreeView*)parent;

    setHighlightSections(false);
    setMouseTracking(true);

    // 响应鼠标
    setSectionsClickable(true);
}

// 槽函数，用于更新复选框状态
void HeaderView::onStateChanged(int state)
{
//    printf("onStateChanged\n");
//    if (state == Qt::PartiallyChecked) {
//        m_bTristate = true;
//    }

//    m_bChecked = (state != Qt::Unchecked);
//    update();
}

// 绘制复选框
void HeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    painter->save();
    QHeaderView::paintSection(painter, rect, logicalIndex);



    painter->restore();
    if (logicalIndex == 0) {
        QStyleOptionButton option;
        option.initFrom(this);
        option.iconSize = QSize(20, 20);
        option.rect = rect;
        option.rect.setWidth(16);
        option.state |= m_bChecked ? QStyle::State_On : QStyle::State_Off;
        option.state &= ~QStyle::State_MouseOver;
        //style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &option, painter, &checkBox);

        style()->drawControl(QStyle::CE_CheckBox, &option, painter, this);
    } /*else if(logicalIndex == 8){
        QStyleOptionButton option;

        option.initFrom(this);
        option.iconSize = QSize(20, 20);
        option.rect = rect;
        //option.rect.setWidth(16);
        option.state |= QStyle::State_ReadOnly;
        painter->setPen(QColor(0,100,0));

        //option.state = QTableCornerButton::section{background:yellow;}
        style()->drawPrimitive(QStyle::PE_Widget, &option, painter, this);

        //style()->drawControl(QStyle::CE_Header, &option, painter, this);
    }*/
}

// 鼠标按下表头
void HeaderView::mousePressEvent(QMouseEvent *event)
{
    return QHeaderView::mousePressEvent(event);
//    printf("mousePressEvent\n");
//    update();

//    int nColumn = logicalIndexAt(event->pos());
//    if (nColumn == 0){
//        m_bPressed = true;
//    } else {
//        QHeaderView::mousePressEvent(event);
//    }
}

void  HeaderView::selectAll(bool checked){
    Qt::CheckState state = checked ? Qt::Checked : Qt::Unchecked;
    //checkBox.setCheckState(state);
    this->viewport()->update();
    printf("selectAll  %d\n", checked);
    int count = treeModel->rowCount();
    for(int i = 0; i < (count-1); i++){
        QStandardItem* item =  treeModel->item(i);
        item->setCheckState(state);
    }
}
void  HeaderView::selectRandom(bool checked){
    printf("selectRandom \n");
    Qt::CheckState state = checked ? Qt::Checked : Qt::Unchecked;
    //randomBox.setCheckState(state);

    this->viewport()->update();
}
// 鼠标从表头释放，发送信号，更新model数据
void HeaderView::mouseReleaseEvent(QMouseEvent *event)
{
    int nColumn = logicalIndexAt(event->pos());
    int x = event->x();
    update();

    if (nColumn == 0 && x < 15){
//        QAbstractItemDelegate *localItemDelegateForColumn = this->itemDelegateForColumn(0);
//        printf("mouseReleaseEvent  %d\n", localItemDelegateForColumn);
        m_bChecked = !m_bChecked;
        selectAll(m_bChecked);
    } else if(nColumn == 8){
        mRandomCheck = !mRandomCheck;
        selectRandom(mRandomCheck);

    }else {
        QHeaderView::mouseReleaseEvent(event);
    }
}

// 鼠标滑过、离开，更新复选框状态
bool HeaderView::event(QEvent *event)
{
    return QHeaderView::event(event);

//    printf("event\n");

//    if (event->type() == QEvent::Enter || event->type() == QEvent::Leave)
//    {
//        QMouseEvent *pEvent = static_cast<QMouseEvent *>(event);
//        int nColumn = logicalIndexAt(pEvent->x());
//        if (nColumn == 0)
//        {
//            m_bMoving = (event->type() == QEvent::Enter);

//            update();
//            return true;
//        }
//    }

    //    return QHeaderView::event(event);
}

HeaderView* HeaderView::setTreeModel(QStandardItemModel* model){
    treeModel  =  model;
    return this;
}
