#include <buffer.h>

buffer::buffer(int bufferSize, bool dropFrame) : bufferSize(bufferSize), dropFrame(dropFrame)
{
    // Semaphore initializations
    freeSlots = new QSemaphore(bufferSize);//�������п���λ�õĸ���
    usedSlots = new QSemaphore(0);//����������ʹ��λ�õĸ���
    clearBuffer1 = new QSemaphore(2);//���������򻺳��������֡�Ĳ�������ͬʱֻ����һ�������򻺳��������֡
    clearBuffer2 = new QSemaphore(2);//���������ӻ�����������֡�Ĳ�������ͬʱֻ����һ�����̴ӻ�����������֡
    // Save value of dropFrame to private member
    this->dropFrame=dropFrame;//�Ƿ�����֡�ı�־
} // ImageBuffer constructor

//��ץ֡���̵��ã��򻺳������������֡
void buffer::addFrame1(const QImage& img)
{
    // Acquire semaphore
    clearBuffer1->acquire();
    // If frame dropping is enabled, do not block if buffer is full
    if(dropFrame)//������֡
    {
        //����λ��Ϊ0ʱ��ֻ�ǲ���֡�ŵ��������У�������������
        // Try and acquire semaphore to add frame
        if(freeSlots->tryAcquire())
        {
            //������λ�ò�Ϊ0����֡��ӵ�����������ӹ���Ҫ����
            // Add frame to queue
            imageQueueProtect.lock();
                imageQueue1.enqueue(img);//��ӵ���β
            imageQueueProtect.unlock();
            // Release semaphore
            usedSlots->release();//ʹ��ʹ��λ�ü�1
        }
    }
    // If buffer is full, wait on semaphore
    else//��������֡
    {

        // Acquire semaphore
        freeSlots->acquire();//ʹ����λ�ü�1��������λ��Ϊ0��������ץ֡����
        // Add frame to queue
        imageQueueProtect.lock();
            imageQueue1.enqueue(img);
        imageQueueProtect.unlock();
        // Release semaphore
        usedSlots->release();//ʹ��ʹ��λ�ü�1
    }
    // Release semaphore
    clearBuffer1->release();
} // addFrame()


//��ץ֡���̵��ã��򻺳������������֡
void buffer::addFrame2(char* img)
{
    // Acquire semaphore
    clearBuffer1->acquire();
    // If frame dropping is enabled, do not block if buffer is full
    if(dropFrame)//������֡
    {
        //����λ��Ϊ0ʱ��ֻ�ǲ���֡�ŵ��������У�������������
        // Try and acquire semaphore to add frame
        if(freeSlots->tryAcquire())
        {
            //������λ�ò�Ϊ0����֡��ӵ�����������ӹ���Ҫ����
            // Add frame to queue
            imageQueueProtect.lock();
                imageQueue2.enqueue(img);//��ӵ���β
            imageQueueProtect.unlock();
            // Release semaphore
            usedSlots->release();//ʹ��ʹ��λ�ü�1
        }
    }
    // If buffer is full, wait on semaphore
    else//��������֡
    {

        // Acquire semaphore
        freeSlots->acquire();//ʹ����λ�ü�1��������λ��Ϊ0��������ץ֡����
        // Add frame to queue
        imageQueueProtect.lock();
            imageQueue2.enqueue(img);
        imageQueueProtect.unlock();
        // Release semaphore
        usedSlots->release();//ʹ��ʹ��λ�ü�1
    }
    // Release semaphore
    clearBuffer1->release();
} // addFrame()





//��������̵��ã��ӻ���������������֡
QImage buffer::getFrame1()
{
    // Acquire semaphores
    clearBuffer2->acquire();
    usedSlots->acquire();//ʹ��ʹ��λ�ü�1����Ϊ0����������
    // Temporary data
    QImage tempFrame;
    // Take frame from queue
    imageQueueProtect.lock();
        tempFrame=imageQueue1.dequeue();//�Ӷ�ͷȡ��֡
    imageQueueProtect.unlock();
    // Release semaphores
    freeSlots->release();//ʹ����λ�ü�1
    clearBuffer2->release();
    // Return frame to caller
    return tempFrame;//�������
} // getFrame()



//��������̵��ã��ӻ���������������֡
char* buffer::getFrame2()
{
    // Acquire semaphores
    clearBuffer2->acquire();
    usedSlots->acquire();//ʹ��ʹ��λ�ü�1����Ϊ0����������
    // Temporary data
    char* tempFrame;
    // Take frame from queue
    imageQueueProtect.lock();
    tempFrame=imageQueue2.dequeue();//�Ӷ�ͷȡ��֡
    imageQueueProtect.unlock();
    // Release semaphores
    freeSlots->release();//ʹ����λ�ü�1
    clearBuffer2->release();
    // Return frame to caller
    return tempFrame;//�������
} // getFrame()



//��ջ�����
void buffer::clearBuffer()
{
    //����ǰ��������Ϊ��
    // Check if buffer is not empty
    if(imageQueue1.size()!=0)
    {
        // Stop adding frames to buffer
        clearBuffer1->acquire();//�����򻺳��������֡�Ĳ���
        // Stop taking frames from buffer
        clearBuffer2->acquire();//�����ӻ�����������֡�Ĳ���
        // Release all remaining slots in queue
        freeSlots->release(imageQueue1.size());//ʹ����λ�ñ���
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
        clearBuffer1->acquire();//�����򻺳��������֡�Ĳ���
        // Stop taking frames from buffer
        clearBuffer2->acquire();//�����ӻ�����������֡�Ĳ���
        // Release all remaining slots in queue
        freeSlots->release(imageQueue2.size());//ʹ����λ�ñ���
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
