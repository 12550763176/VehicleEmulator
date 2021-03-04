#include "Channel.h"
#include "DataCalcManager.h"


DataCalcManager::DataCalcManager()
{
    vec = DataManager::getInstace()->getList();
    mRandom = thread(&DataCalcManager::randomSend, this);
}
DataCalcManager::~DataCalcManager(){
    mRandom.detach();
}

void DataCalcManager::randomSend(){
    default_random_engine e;

    e.seed((int)time(0));

    while(true){
        usleep(10000);
        {
        std::lock_guard<std::mutex> lock(mapMutex);

        for(auto it : randomMap){
            string value;
            //printf("adfsaf fcda  %s\n", it.first.c_str());
            if(!it.second->name.c_str()){continue;}

            string max = it.second->sig[it.first].max;

            string min = it.second->sig[it.first].min;

            string resolution = it.second->sig[it.first].resolution;

            string offset = it.second->sig[it.first].Offset;

            if((atol(resolution.c_str()) == atof(resolution.c_str())) && (atol(offset.c_str()) == atof(offset.c_str()))){
                 long maxValue = atol(max.c_str());
                 long minValue = atol(min.c_str());
                 //uniform_int_distribution<long> u(minValue, maxValue);
                 uniform_int_distribution<long> u(minValue, maxValue);
                 long tmp = u(e);
                 if(tmp > maxValue){
                     tmp = maxValue;
                 }
                 value = to_string(tmp);
            } else {
                float maxValue = atof(max.c_str());
                float minValue = atof(min.c_str());
                uniform_int_distribution<long> u((long)minValue, (long)maxValue);

                long tmp = u(e) + minValue - (int)minValue;
                if(tmp > maxValue){
                    tmp = maxValue;
                }
                value = to_string(tmp);
            }
            updateData(*it.second, it.first, value);
        }
        }

    }
}

void DataCalcManager::addRandom(Msg& msg, string sigName){

    {
        std::lock_guard<std::mutex> lock(mapMutex);
        if(msg.channel == SPI) return;
        randomMap[sigName] = &msg;

    }
}

void DataCalcManager::delRandom(Msg& msg, string sigName){
    {
        std::lock_guard<std::mutex> lock(mapMutex);
        if(msg.channel == SPI) return;
        if(randomMap.find(sigName) != randomMap.end()){
            randomMap.erase(randomMap.find(sigName));
        }
    }
}

void DataCalcManager::calcValue(Msg& msg){
//    printf("%s,  canid: %x \n", "calcValue", msg.id);
    if(msg.id == 0x155){return;}
    vector<uint8_t> data(msg.dataBytes, 0);//frame.len
    //vector<uint8_t> data(255, 0);//frame.len

    for (auto iter = msg.sig.begin();iter != msg.sig.end(); iter++){
        int curValue = 0;
        if(!iter->second.curValue.empty()){
            curValue = stol(iter->second.curValue);
        }
        uint64_t v = curValue << iter->second.moveSize;
        printf("sigName: %s, sigvalue: %x , movesize:%d , sigSize:%d , endByte: %d\n", iter->second.sigName.c_str(), v, iter->second.moveSize, iter->second.sigSize, iter->second.endByte);
        for(int i = 0; i <= ((iter->second.sigSize+iter->second.moveSize)/8);  i++){ // TODO
            //
            data[iter->second.endByte - i] = data[iter->second.endByte-i]|( v & 0xff);
            v = v >> 8;
        }
    }
    msg.data = data;
    printf("%s, %s, canid: %x  %d\n", "calcValue", msg.name.c_str(), msg.id, msg.dataBytes);
}



bool DataCalcManager::updateData(Msg& msg, string sigName, string sigValue)
{
   printf("updateData %x, %s %s, %s\n", msg.id, msg.name.c_str(), sigName.c_str(), sigValue.c_str());
    //handle sigValue
    if(sigValue.empty()){
        return false;
    }
    if(!Utils::isNum(sigValue)){
        vector<string> vec = Utils::split(sigValue, ":");
        if(vec.size()==2){
            sigValue =  vec[1];
            sigValue = Utils::trim(sigValue);
            if(!Utils::isNum(sigValue)){
                return false;
            }
        } else {
            return false;
        }
    }

    if(msg.channel == SPI){

        if(msg.sig[sigName].curValue == sigValue){
            return false;
        }
        double tmp = atof(sigValue.c_str());
        uint64_t value = atol(sigValue.c_str());

        msg.sig[sigName].curValue = sigValue;
            //TODO
        if(msg.data.size() < (msg.dataBytes +2)){
            msg.data.resize(msg.dataBytes + 2);
            msg.data[0] = (msg.dataBytes >> 8) & 0xff;
            msg.data[1] = (msg.dataBytes) &  0xff;
            }
        int index = msg.sig[sigName].startByte + 2;
//        printf("%x %s, %s, sigSize:%d, startByte:%d\n", msg.id, sigName.c_str(), msg.sig[sigName].curValue.c_str(), msg.sig[sigName].sigSize, msg.sig[sigName].startByte);

        for(int i = msg.sig[sigName].sigSize-1; i >= 0; i--){
            msg.data[index + i] =  value & 0xff;
            value = value >> 8;
        }
//        calcValue(msg);
//        for(uint8_t i : msg.data){
//            printf("%x  ", i);
//        }
//      printf("\n");
    } else {
        if(msg.sig[sigName].curValue == sigValue){
            return false;
        }
        double tmp = atof(sigValue.c_str());
        uint64_t value;
        double max = atof(msg.sig[sigName].max.c_str());
        double min = atof(msg.sig[sigName].min.c_str());
        if(max < tmp || tmp < min){
            msg.sig[sigName].curValue = "0";
        } else {
            double offset = atof(msg.sig[sigName].Offset.c_str());
            double resolution = atof(msg.sig[sigName].resolution.c_str());
            msg.sig[sigName].curValue = to_string((tmp - offset)/resolution);
        }
        //printf("%x %s, %s,  %s    sigSize:%d, endByte:%d\n", msg.id, msg.sig[sigName].curValue.c_str(), msg.sig[sigName].curValue.c_str(), msg.sig[sigName].sigSize, msg.sig[sigName].endByte);
        calcValue(msg);
//        for(uint8_t i : msg.data){
//            printf("%x  ", i);
//        }
//        printf("\n");
    }

    if(msg.isSelect){
        send(msg);
    }

    return true;
}

void DataCalcManager::send(Msg& msg)
{
    if(msg.channel == SPI && msg.data.size()  == 0){
        return;
    }
   // printf("DataCalcManager  id:%x  name:%s, %d", msg.id, msg.name.c_str(), msg.data.size());

    if(msg.data.size() != (msg.dataBytes + (msg.channel == SPI? 2 : 0))){
        vector<uint8_t> vec(msg.dataBytes, 0);
        msg.data = vec;

    }
    Channel::getInstace(msg.channel)->send(msg.interval, msg.id, msg.data);
}
void DataCalcManager::del(Msg& msg)
{

    Channel::getInstace(msg.channel)->del(msg.interval, msg.id);
}




//void SignalValueDelegate::setEditorData(QWidget *editor,
//                                    const QModelIndex &index) const
//{
//    QString value = index.model()->data(index, Qt::EditRole).toString();
//    QComboBox* box = static_cast<QComboBox*>(editor);
//    box->setCurrentText(value);
//    string inputValue = value.toStdString();
//    inputValue = Utils::trim(inputValue);
//    if(inputValue.empty()){
//       return;
//    }

//    CanFrame* frame;
//    QStandardItem* can = treeModel->item((index.parent().row()));
//    for(int i = 0; i < vec.size(); i++){
//        DBC* dbc = vec[i]->getDbc();
//        uint16_t canId;
//        sscanf(can->text().toStdString().c_str(), "0x%x", &canId);
//        frame = &(dbc->canFrame[canId]);
//    }
//    string sigName = can->child(index.row(),1)->text().toStdString();
//    sigName = Utils::trim(sigName);

//    if(!Utils::isNum(inputValue)){
//         vector<string> vec = Utils::split(inputValue, ":");
//         if(vec.size()==2){
//             inputValue =  vec[1];
//             inputValue = Utils::trim(inputValue);
//             if(!Utils::isNum(inputValue)){
//                inputValue = frame->sig[sigName].Invalid;
//             }
//         } else {
//             inputValue = frame->sig[sigName].Invalid;

//         }
//    }

//    if(frame->sig[sigName].curValue == inputValue){
//        return;
//    }else{
//        frame->sig[sigName].curValue = inputValue;
//    }

//    double tmp = atof(inputValue.c_str());
//    uint64_t sigValue;

//    double max = atof(frame->sig[sigName].max.c_str());
//    double min = atof(frame->sig[sigName].min.c_str());
//    if(max < tmp || tmp < min){
//        sigValue = frame->sig[sigName].Invalid;
//    } else {
//        double offset = atof(frame->sig[sigName].Offset.c_str());
//        double resolution = atof(frame->sig[sigName].resolution.c_str());
//        sigValue = (tmp - offset)/resolution;

//        sigValue <<= frame->sig[sigName].moveSize;
//    }

//    frame->data &= frame->sig[sigName].Invalid;
//    frame->data |= sigValue;

//
//}
