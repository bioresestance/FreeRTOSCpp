#include <freertosTask.h>

namespace freeRTOS
{ 

    Task::Task(const char* taskName, uint32_t taskStackSize, uint32_t taskPriority)
    : _taskStackSize(taskStackSize),_taskPriority(taskPriority),_taskHandle(NULL)
    {
        // Copy the tasks name for future use.
        strlcpy(_taskName, taskName, sizeof(_taskName));

        // Create the task and suspend it right away so it doesn't start. Suspend scheduler while we create the task.
        vTaskSuspendAll();
        BaseType_t result = xTaskCreatePinnedToCore(taskHandler, taskName, taskStackSize, this, taskPriority, &_taskHandle, 1);
        this->suspend();
        xTaskResumeAll();
    }
    
    Task::Task(const char* taskName)
    : Task(taskName, defaultStackSize, defaultPriority) // Calls base ctor with default stack size and priority.
    {        
    }
    
    Task::~Task()
    {
#if INCLUDE_vTaskDelete
        // If the task was created, we need to Destroy it when object is deleted.
        if(this->_taskHandle != NULL) {
            vTaskDelete(this->_taskHandle);
            this->_taskHandle = NULL;
        }
#endif
    }

    void Task::taskHandler(void *_params)
    {
        //params are a pointer to the current object, so cast it as such.
        Task *p = static_cast<Task *>(_params);

        // Now we can run the tasks main function that it should have defined.
        p->taskMain();

        // Should only get here if task ends. Run destructor.
        p->~Task();
    }
    

    const char* Task::getTaskName()
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


    void Task::suspend() 
    {
        #if INCLUDE_vTaskSuspend  
        vTaskSuspend(this->_taskHandle);
        #endif
    }
    
    void Task::resume() 
    {
        #if INCLUDE_vTaskSuspend  
        vTaskResume(this->_taskHandle);
        #endif
    }
    
    void Task::delay(uint32_t ms) 
    {
        vTaskDelay(ms / portTICK_PERIOD_MS);
    }


    void Task::start() 
    {
        // Starts the task by resuming it.
        if(!this->isTaskStarted) {
            this->resume();
            this->isTaskStarted = true;
        }
        
    }

} // namespace freeRTOS