#include <cassert>
#include <iostream>
#include <chrono>

#define VERBOSE

#include "executive.h"
#include "rt/priority.h"





Executive::Executive(size_t num_tasks, unsigned int frame_length, unsigned int unit_duration)
	: p_tasks(num_tasks), frame_length(frame_length), unit_time(unit_duration)
{
}

void Executive::set_periodic_task(size_t task_id, std::function<void()> periodic_task, unsigned int wcet)
{
	assert(task_id < p_tasks.size()); // Fallisce in caso di task_id non corretto (fuori range)
	
	p_tasks[task_id].function = periodic_task;
	p_tasks[task_id].wcet = wcet;
}

void Executive::set_aperiodic_task(std::function<void()> aperiodic_task, unsigned int wcet)
{
 	ap_task.function = aperiodic_task;
 	ap_task.wcet = wcet;
}
		
void Executive::add_frame(std::vector<size_t> frame)
{
	for (auto & id: frame)
		assert(id < p_tasks.size()); // Fallisce in caso di task_id non corretto (fuori range)
	
	frames.push_back(frame);

	
}

void Executive::start()
{
	for (size_t id = 0; id < p_tasks.size(); ++id)
	{
		assert(p_tasks[id].function); // Fallisce se set_periodic_task() non e' stato invocato per questo id
        
        p_tasks[id].task_cond = &exec_cond;
        p_tasks[id].status_mtx = &exec_mtx;
        p_tasks[id].idx = id;
		
		p_tasks[id].thread = std::thread(&Executive::task_function, std::ref(p_tasks[id]));

		rt::priority p(rt::priority::rt_max - ((id+1)*10) );
        rt::set_priority(p_tasks[id].thread,p);
	}
	
	if(ap_task.function)
    {
        ap_task.task_cond = &exec_cond;
        ap_task.status_mtx = &exec_mtx;
        ap_task.idx = 99;
        ap_task.thread = std::thread(&Executive::task_function, std::ref(ap_task));
        rt::priority ap_p(rt::priority::rt_min+1);
        rt::set_priority(ap_task.thread,ap_p);
    }
	
	exec_thread = std::thread(&Executive::exec_function, this);
    rt::priority exec_p(rt::priority::rt_max);
    rt::set_priority(exec_thread,exec_p);
	
	exec_thread.join();
}
	
void Executive::wait()
{
	exec_thread.join();
	
	ap_task.thread.join();
	
	for (auto & pt: p_tasks)
		pt.thread.join();
}

void Executive::ap_task_request()
{
    std::unique_lock<std::mutex> l(exec_mtx);
    if (ap_task.status == State::IDLE)
    {
        auto now = std::chrono::steady_clock::now();
        if(now+std::chrono::milliseconds(ap_task.wcet*unit_time) <= point) //se abbastanza tempo lo eseguo
            ap_task.status = State::READY;
    }
}

void Executive::task_function(Executive::task_data & task)
{
	std::unique_lock<std::mutex> l(*task.status_mtx);
	while (true) 
    {
        auto start = std::chrono::high_resolution_clock::now();
		// wait for activation
		while (task.status != State::READY)		// READY : task pronto per l'esecuzione
			task.task_cond->wait(l);
        task.status = State::RUNNING;			// RUNNING : task in esecuzione
		task.function();
        task.status = State::IDLE;
         auto end = std::chrono::high_resolution_clock::now();        
        std::chrono::duration<double, std::milli> elapsed(end - start);
        //std::cout << "Task "<< task.idx << " Elapsed after wake up [ms]: " << elapsed.count() << std::endl;
        task.task_cond->notify_one();
	}
}

void Executive::exec_function()
{

	size_t frame_id = 0;
    auto exec_period = std::chrono::milliseconds(frame_length*unit_time);
    std::cout << "Period of exec: " << exec_period.count() << std::endl;
    point = std::chrono::steady_clock::now();
	while (true)
	{
        auto start = std::chrono::high_resolution_clock::now();
		/* Rilascio dei task periodici del frame corrente e aperiodico (se necessario)... */
        std::cout << "Frame no: " << frame_id << std::endl;
        point += exec_period;
        std::cout << "Task queue: ";
        for (size_t task_id = 0; task_id < frames[frame_id].size(); task_id++ )
        {
            int task_idx = frames[frame_id][task_id];
            std::cout << " " << task_idx << " ";
            std::unique_lock<std::mutex> l(exec_mtx);
            p_tasks[task_idx].status = State::READY;
        }
        std::cout << std::endl;
        
        exec_cond.notify_all();
        
        //auto ret = exec_cond.wait_for(l,std::chrono::milliseconds(frame_length*unit_time));
		/* Attesa fino al prossimo inizio frame ... */
        //auto start{std::chrono::}
        std::this_thread::sleep_until(point);
        auto wakeup = std::chrono::high_resolution_clock::now();
        
        std::chrono::duration<double, std::milli> elapsed(wakeup - start);
        std::cout << "Exec Elapsed after wake up [ms]: " << elapsed.count() << std::endl;
        
		/* Controllo delle deadline ... */
        for (size_t task_id = 0; task_id < frames[frame_id].size(); task_id++ )
        {
            int task_idx = frames[frame_id][task_id];
            std::unique_lock<std::mutex> l(exec_mtx);
            std::cout << "Task "<< task_idx << " status: " << p_tasks[task_idx].status << std::endl;
            if (p_tasks[task_idx].status == State::READY)
            {
                std::cerr << "Task: " << task_id << " never started cancelled" << std::endl;
                p_tasks[task_idx].status = State::IDLE;
            }
            if (p_tasks[task_idx].status == State::RUNNING)
            {
                std::cerr << "Task: " << task_id << " deadline exceed" << std::endl;
                p_tasks[task_idx].status = State::READY;
                rt::priority p(rt::priority::rt_min);
                rt::set_priority(p_tasks[task_idx].thread,p); //settare la priorità più bassa
            }
        } 
        
        {
            std::unique_lock<std::mutex> l(exec_mtx);
            if(ap_task.status != State::IDLE)
                std::cout << "ap_task status: " << ap_task.status << std::endl;
            if (ap_task.status == State::RUNNING)
            {
                    std::cerr << "AP task: " << " deadline exceed" << std::endl;
                    ap_task.status = State::READY;
            }
        }
        
        if (++frame_id == frames.size())
		{
			frame_id = 0;
		}
    }
}


