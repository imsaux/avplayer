#include "controller.h"


Controller::Controller()
{
} // Controller constructor

Controller::~Controller()
{
} // Controller destructor

bool Controller::Start()
{
    _capture_thread = new captureThread();
    _distortion_thread = new distortion_thread();
//    _bgr2rgb_thread = new BGR2RGB_thread();
//    _toimg_thread = new TOIMG_thread();
    _capture_thread->start();//�����������
    _distortion_thread->start();//��������У���߳�
//    _bgr2rgb_thread->start();//������ɫת���߳�
//    _toimg_thread->start();//����QImageת���߳�
    return true;
}
