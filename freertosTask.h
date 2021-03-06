#pragma once

#include <string.h>
#include <iostream>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

namespace freeRTOS
{
    class Task
    {
    private:

        bool isTaskStarted = false;

    protected:
        /****************************************************************************************************************/
        /*                                           Task Constants                                                     */
        /****************************************************************************************************************/

        //! Default Task Stack size when there is not one provided.
        static const uint32_t defaultStackSize = 2048 * 2;

        //! DEfault Tasks Priority when there is not one provided.
        static const uint32_t defaultPriority = 5;

        /****************************************************************************************************************/
        /*                                           Task Common Variables                                              */
        /****************************************************************************************************************/

        //! Holds the stack size for the task.
        uint32_t _taskStackSize;

        //! Holds the priority level for the task.
        uint32_t _taskPriority;

        //! Holds the name of the task.
        char _taskName[64];

        //! Holds the Task Handle for the task.
        TaskHandle_t _taskHandle;

        /****************************************************************************************************************/
        /*                                           Task Constructors                                                  */
        /****************************************************************************************************************/
    public:
        /**
         * @brief Construct a new Task object with all needed values
         * 
         * @param taskName Name of the FreeRTOS task.
         * @param taskStackSize Stack Size to allocate for the task.
         * @param taskPriority Priority level for the task. 
         */
        Task(const char* taskName, uint32_t taskStackSize, uint32_t taskPriority);

        /**
         * @brief Construct a new Task object with only a Name. Uses Default priority and stack size.
         * 
         * @param taskName Name of the FreeRTOS task.
         */
        Task(const char* taskName);

        /****************************************************************************************************************/
        /*                                           Task destructors                                                   */
        /****************************************************************************************************************/

        /**
         * @brief Destroy the Task object. Deletes the FreeRtos Task.
         */
        ~Task();

        /****************************************************************************************************************/
        /*                                           Task Main Methods                                                  */
        /****************************************************************************************************************/

        /**
         * @brief Static function that all FreeRTOS tasks call to start task. Calls overriden taskMain Function/ 
         * 
         *  This is the main task function that all FreeRTOS Objects end up first calling. This functions gets called indirectly 
         *  when a new task is created. The param pointer input contains the "this" pointer. That is used to call the overriden
         *  taskMain function using dynamic Polymorphism.
         * 
         *  \note This function should never be called except as a function pointer to xTaskCreate function.
         * 
         * @param _params Parameters of the FreeRTOS task. In reality contains pointer to 'this' Object.
         */
        static void taskHandler(void *_params);

        /****************************************************************************************************************/
        /*                                           Task Public Methods                                                */
        /****************************************************************************************************************/

        /**
         * @brief Main function of the FreeRTOS task. 
         * 
         */
        virtual void taskMain() = 0;

        /**
         * @brief Starts the task, if not already running.
         * 
         */
        void start();

        /**
         * @brief Gets the Task Name.
         * 
         * @return string The name of the task.
         */
        const char* getTaskName();

        /**
         * @brief Get the Task Priority.
         * 
         * @return uint32_t The priority level of the task.
         */
        uint32_t getTaskPriority();

        /**
         * @brief Gets the Task Stack Size.
         * 
         * @return uint32_t The Stack size of the task.
         */
        uint32_t getTaskStackSize();

        /**
         * @brief Suspends the task.
         * 
         */
        void suspend();

        /**
         * @brief Resumes a suspended task.
         * 
         */
        void resume();

        /**
         * @brief Delays the task by the number of provided milliseconds
         * 
         * @param ms Amount to delay, in ms.
         */
        void delay(uint32_t ms);

        /**
         * @brief Get the current FreeRTOS Tick count since power on.
         *  
         * @return TickType_t Current Tick count.
         */
        static TickType_t getTickCount();


        /**
         * @brief Gets the current time in ms since power on.
         * 
         * @return uint32_t The current time, in ms.
         */
        static uint32_t getCurrentTimeMs();
    };

} // namespace freeRTOS
