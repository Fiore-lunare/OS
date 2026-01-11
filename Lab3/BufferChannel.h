#include <iostream>
#include <mutex>
#include <condition_variable>
#include <utility>
#include <stdexcept>
#include <queue>


template <class T>
class BufferChannel {
private:
    int capacity_;
    std::queue<T> buffer_;
    std::mutex mutex_;
    std::condition_variable conditionRecv_;
    std::condition_variable conditionSend_;
    bool isClose_;

public:
    BufferChannel(int capacity_ = 12) {
        if (capacity_ <= 0) throw std::invalid_argument("Invalid argument");
        this->isClose_ = false;
        this->capacity_ = capacity_;
    }

    bool isClose() const {return this->isClose_;}
    int Capacity(){return this->capacity_;}
    bool isEmpty(){return this->buffer_.empty();}

    void Close() {
        std::unique_lock<std::mutex> lock(this->mutex_);
        this->isClose_ = true;
        this->conditionRecv_.notify_all();
        this->conditionSend_.notify_all();
    }

    std::pair<T, bool> Recv() {
        std::unique_lock<std::mutex> lock(this->mutex_);
        this->conditionRecv_.wait(lock, [&]{return !this->buffer_.empty() || this->isClose_;});
        if (this->isClose_ && this->buffer_.empty()) return std::make_pair(T(), false);
        T elem = std::move(this->buffer_.front());
        this->buffer_.pop();
        this->conditionSend_.notify_one();
        return std::make_pair(std::move(elem), true);
    }

    void Send (T value) {
        std::unique_lock<std::mutex> lock(this->mutex_);
        this->conditionSend_.wait(lock, [&] {return this->buffer_.size()< this->capacity_ || this->isClose_;});
        if (this->isClose_) throw std::runtime_error("Channel is closed");
        this->buffer_.push(std::move(value));
        this->conditionRecv_.notify_one();
    }
};