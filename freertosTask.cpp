#include <freertosTask.h>

namespace freeRTOS
{ 

    Task::Task(string taskName, uint32_t taskStackSize, uint32_t taskPriority)
    : _taskStackSize(taskStackSize),_taskPriority(taskPriority), _taskName(taskName),_taskHandle(NULL)
    {
        xTaskCreatePinnedToCore(taskHandler, taskName.c_str(), taskStackSize, this, taskPriority, &_taskHandle, 1);
    }
    
    Task::Task(string taskName)
    : Task(taskName, defaultStackSize, defaultPriority) // Calls base ctor with default stack size and priority.
    {        
    }
    
    Task::~Task()
    {
#if INCLUDE_vTaskDelete
        // If the task was created, we need to Destroy it when object is deleted.
        if(this->_taskHandle != NULL) {
            vTaskDelete(this->_taskHandle);
        }
#endif
    }

    void Task::taskHandler(void *_params)
    {
        Task *p = static_cast<Task *>(_params);
        p->taskMain();

        // Should only get here if task ends. Run destructor.
        p->~Task();
    }
    
    void Task::start() 
    {
        
    }

    string Task::getTaskName()
    {
        return _taskName;
    }
    
    uint32_t Task::getTaskPriority()
    {
        return _taskPriority;
    }
    
    uint32_t Task::getTaskStackSize()
    {
        return _taskStackSize;
    }

#if INCLUDE_vTaskSuspend  
    void Task::suspend() 
    {
        vTaskSuspend(this->_taskHandle);
    }
    
    void Task::resume() 
    {
        vTaskResume(this->_taskHandle);
    }
#endif

} // namespace freeRTOS