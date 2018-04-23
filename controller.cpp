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
    _capture_thread->start();//启动捕获进程
    _distortion_thread->start();//启动畸变校正线程
//    _bgr2rgb_thread->start();//启动颜色转换线程
//    _toimg_thread->start();//启动QImage转换线程
    return true;
}
