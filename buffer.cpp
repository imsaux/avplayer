#include <buffer.h>

buffer::buffer(int bufferSize, bool dropFrame) : bufferSize(bufferSize), dropFrame(dropFrame)
{
    // Semaphore initializations
    freeSlots = new QSemaphore(bufferSize);//缓冲区中空闲位置的个数
    usedSlots = new QSemaphore(0);//缓冲区中已使用位置的个数
    clearBuffer1 = new QSemaphore(2);//用来锁定向缓冲区中添加帧的操作，即同时只能有一个进程向缓冲区中添加帧
    clearBuffer2 = new QSemaphore(2);//用来锁定从缓冲区中拿走帧的操作，即同时只能有一个进程从缓冲区中拿走帧
    // Save value of dropFrame to private member
    this->dropFrame=dropFrame;//是否允许丢帧的标志
} // ImageBuffer constructor

//被抓帧进程调用，向缓冲区队列中添加帧
void buffer::addFrame1(const QImage& img)
{
    // Acquire semaphore
    clearBuffer1->acquire();
    // If frame dropping is enabled, do not block if buffer is full
    if(dropFrame)//若允许丢帧
    {
        //空闲位置为0时，只是不把帧放到缓冲区中，而不阻塞进程
        // Try and acquire semaphore to add frame
        if(freeSlots->tryAcquire())
        {
            //若空闲位置不为0，将帧添加到缓冲区，添加过程要加锁
            // Add frame to queue
            imageQueueProtect.lock();
                imageQueue1.enqueue(img);//添加到队尾
            imageQueueProtect.unlock();
            // Release semaphore
            usedSlots->release();//使已使用位置加1
        }
    }
    // If buffer is full, wait on semaphore
    else//若不允许丢帧
    {

        // Acquire semaphore
        freeSlots->acquire();//使空闲位置减1，若空闲位置为0，则阻塞抓帧进程
        // Add frame to queue
        imageQueueProtect.lock();
            imageQueue1.enqueue(img);
        imageQueueProtect.unlock();
        // Release semaphore
        usedSlots->release();//使已使用位置加1
    }
    // Release semaphore
    clearBuffer1->release();
} // addFrame()


//被抓帧进程调用，向缓冲区队列中添加帧
void buffer::addFrame2(char* img)
{
    // Acquire semaphore
    clearBuffer1->acquire();
    // If frame dropping is enabled, do not block if buffer is full
    if(dropFrame)//若允许丢帧
    {
        //空闲位置为0时，只是不把帧放到缓冲区中，而不阻塞进程
        // Try and acquire semaphore to add frame
        if(freeSlots->tryAcquire())
        {
            //若空闲位置不为0，将帧添加到缓冲区，添加过程要加锁
            // Add frame to queue
            imageQueueProtect.lock();
                imageQueue2.enqueue(img);//添加到队尾
            imageQueueProtect.unlock();
            // Release semaphore
            usedSlots->release();//使已使用位置加1
        }
    }
    // If buffer is full, wait on semaphore
    else//若不允许丢帧
    {

        // Acquire semaphore
        freeSlots->acquire();//使空闲位置减1，若空闲位置为0，则阻塞抓帧进程
        // Add frame to queue
        imageQueueProtect.lock();
            imageQueue2.enqueue(img);
        imageQueueProtect.unlock();
        // Release semaphore
        usedSlots->release();//使已使用位置加1
    }
    // Release semaphore
    clearBuffer1->release();
} // addFrame()





//被处理进程调用，从缓冲区队列中拿走帧
QImage buffer::getFrame1()
{
    // Acquire semaphores
    clearBuffer2->acquire();
    usedSlots->acquire();//使已使用位置减1，若为0则阻塞进程
    // Temporary data
    QImage tempFrame;
    // Take frame from queue
    imageQueueProtect.lock();
        tempFrame=imageQueue1.dequeue();//从队头取出帧
    imageQueueProtect.unlock();
    // Release semaphores
    freeSlots->release();//使空闲位置加1
    clearBuffer2->release();
    // Return frame to caller
    return tempFrame;//返回深拷贝
} // getFrame()



//被处理进程调用，从缓冲区队列中拿走帧
char* buffer::getFrame2()
{
    // Acquire semaphores
    clearBuffer2->acquire();
    usedSlots->acquire();//使已使用位置减1，若为0则阻塞进程
    // Temporary data
    char* tempFrame;
    // Take frame from queue
    imageQueueProtect.lock();
    tempFrame=imageQueue2.dequeue();//从队头取出帧
    imageQueueProtect.unlock();
    // Release semaphores
    freeSlots->release();//使空闲位置加1
    clearBuffer2->release();
    // Return frame to caller
    return tempFrame;//返回深拷贝
} // getFrame()



//清空缓冲区
void buffer::clearBuffer()
{
    //若当前缓冲区不为空
    // Check if buffer is not empty
    if(imageQueue1.size()!=0)
    {
        // Stop adding frames to buffer
        clearBuffer1->acquire();//锁定向缓冲区中添加帧的操作
        // Stop taking frames from buffer
        clearBuffer2->acquire();//锁定从缓冲区中拿走帧的操作
        // Release all remaining slots in queue
        freeSlots->release(imageQueue1.size());//使空闲位置变满
        // Acquire all queue slots
        freeSlots->acquire(bufferSize);//
        // Reset usedSlots to zero
        usedSlots->acquire(imageQueue1.size());
        // Clear buffer
        imageQueue1.clear();
        // Release all slots
        freeSlots->release(bufferSize);//
        // Allow getFrame() to resume
        clearBuffer2->release();
        // Allow addFrame() to resume
        clearBuffer1->release();
        qDebug() << "Image buffer successfully cleared.";
    }
    if(imageQueue2.size()!=0)
    {
        // Stop adding frames to buffer
        clearBuffer1->acquire();//锁定向缓冲区中添加帧的操作
        // Stop taking frames from buffer
        clearBuffer2->acquire();//锁定从缓冲区中拿走帧的操作
        // Release all remaining slots in queue
        freeSlots->release(imageQueue2.size());//使空闲位置变满
        // Acquire all queue slots
        freeSlots->acquire(bufferSize);//
        // Reset usedSlots to zero
        usedSlots->acquire(imageQueue2.size());
        // Clear buffer
        imageQueue2.clear();
        // Release all slots
        freeSlots->release(bufferSize);//
        // Allow getFrame() to resume
        clearBuffer2->release();
        // Allow addFrame() to resume
        clearBuffer1->release();
        qDebug() << "Image buffer successfully cleared.";
    }
} // clearBuffer()

int buffer::getSizeOfImageBuffer1()
{
    return imageQueue1.size();
} // getSizeOfImageBuffer()
int buffer::getSizeOfImageBuffer2()
{
    return imageQueue2.size();
} // getSizeOfImageBuffer()
