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
void buffer::addFrame(const QImage& img)
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
                imageQueue.enqueue(img);//��ӵ���β
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
            imageQueue.enqueue(img);
        imageQueueProtect.unlock();
        // Release semaphore
        usedSlots->release();//ʹ��ʹ��λ�ü�1
    }
    // Release semaphore
    clearBuffer1->release();
} // addFrame()

//��������̵��ã��ӻ���������������֡
QImage buffer::getFrame()
{
    // Acquire semaphores
    clearBuffer2->acquire();
    usedSlots->acquire();//ʹ��ʹ��λ�ü�1����Ϊ0����������
    // Temporary data
    QImage tempFrame;
    // Take frame from queue
    imageQueueProtect.lock();
        tempFrame=imageQueue.dequeue();//�Ӷ�ͷȡ��֡
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
    if(imageQueue.size()!=0)
    {
        // Stop adding frames to buffer
        clearBuffer1->acquire();//�����򻺳��������֡�Ĳ���
        // Stop taking frames from buffer
        clearBuffer2->acquire();//�����ӻ�����������֡�Ĳ���
        // Release all remaining slots in queue
        freeSlots->release(imageQueue.size());//ʹ����λ�ñ���
        // Acquire all queue slots
        freeSlots->acquire(bufferSize);//
        // Reset usedSlots to zero
        usedSlots->acquire(imageQueue.size());
        // Clear buffer
        imageQueue.clear();
        // Release all slots
        freeSlots->release(bufferSize);//
        // Allow getFrame() to resume
        clearBuffer2->release();
        // Allow addFrame() to resume
        clearBuffer1->release();
        qDebug() << "Image buffer successfully cleared.";
    }
    else
        qDebug() << "WARNING: Could not clear image buffer: already empty.";
} // clearBuffer()

int buffer::getSizeOfImageBuffer()
{
    return imageQueue.size();
} // getSizeOfImageBuffer()
