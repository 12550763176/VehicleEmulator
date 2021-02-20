#ifndef CONQUEUE_H
#define CONQUEUE_H
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>

using namespace std;

template<typename T>
class ConQueue{
public:
    void waitForItems(){
        unique_lock<mutex> g(mLock);
        while(mQueue.empty()){
            mCond.wait(g);
        }
    }

    vector<T> flush(){
        vector<T> items;
        MuxGuard g(mLock);
        if(mQueue.empty()){
            return items;
        }
        while(!mQueue.empty()){
            items.push_back(move(mQueue.front()));
            mQueue.pop();
        }
        return items;
    }
    void push(const T& item){
        {
            MuxGuard g(mLock);
            mQueue.push(item);
        }
        mCond.notify_one();
    }
    size_t size(){
        return mQueue.size();
    }
    ConQueue() = default;
    ConQueue(const ConQueue &) = default;
    ConQueue &operator=(const ConQueue &) = delete;
private:
    queue<T> mQueue;
    mutable mutex mLock;
    condition_variable mCond;
    using MuxGuard = std::lock_guard<std::mutex>;
};


#endif // CONQUEUE_H
