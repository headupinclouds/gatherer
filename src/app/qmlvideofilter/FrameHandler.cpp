#include "FrameHandler.h"

FrameHandlerManager * FrameHandlerManager::m_instance = nullptr;

FrameHandlerManager::FrameHandlerManager()
{

}

FrameHandlerManager::~FrameHandlerManager()
{
    if(m_instance)
    {
        delete m_instance;
    }
    m_instance = 0;
}

FrameHandlerManager * FrameHandlerManager::get()
{
    if(!m_instance)
    {
        m_instance = new FrameHandlerManager;
    }
    return m_instance;
}
