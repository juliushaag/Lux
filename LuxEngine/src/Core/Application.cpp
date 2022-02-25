#include "Application.h"
#include "Renderer/RendererAPI.h"

namespace Lux
{

Application* Application::s_Instance = nullptr;

Application::Application(const std::string& title)
    : m_title(title), m_window(title, (u32)m_width, (u32)m_height), m_renderer(RendererAPI::create())
{
    s_Instance = this;
}

void Application::loop()
{
    while(m_running)
    {

        m_frame_time = m_window.delta_time();
        // Event Processing
        do {
            
            prepare_event_buffer();
            m_window.poll_events();
            dispatch_event_buffer();

        } while(m_minimized);

        m_renderer->clear({ 1.0f, 1.0, 0.3f, 1.0f});

        // Reverse callstack
        for(int i = (int)m_layerstack.size() - 1; i >= 0; i--)
        {
            m_layerstack[i]->on_update();
        }


        m_window.swap_buffers();
    }
}

void Application::prepare_event_buffer()
{
    m_event_buffer.scrolled.reset(m_frame_time);
    m_event_buffer.mouse_moved.reset(m_frame_time);
    m_event_buffer.window_resize.reset(m_frame_time);


    m_event_buffer.key_events.clear();
    m_event_buffer.char_events.clear();
    m_event_buffer.mouse_button.clear();
}

void Application::dispatch_event_buffer()
{ 

    if(m_event_buffer.scrolled.valid)
        dispatch_single_event(m_event_buffer.scrolled);

    if(m_event_buffer.mouse_moved.valid)
        dispatch_single_event(m_event_buffer.mouse_moved);

    if(m_event_buffer.window_resize.valid)
        dispatch_single_event(m_event_buffer.window_resize);
    
    for(auto& event : m_event_buffer.mouse_button)
    {
        event.delta_time = m_frame_time;
        dispatch_single_event(event);
    }

    for(auto& event : m_event_buffer.key_events)
    {
        event.delta_time = m_frame_time;
        dispatch_single_event(event);
    }

    for(auto& event : m_event_buffer.char_events)
    {
        event.delta_time = m_frame_time;
        dispatch_single_event(event);
    }
}



void Application::dispatch_single_event(Event& event)
{
    for(auto layer : m_layerstack)
    {
        bool returnValue = layer->on_event(event);

        if(returnValue && event.is_erasable())
            return;
    }
}

}