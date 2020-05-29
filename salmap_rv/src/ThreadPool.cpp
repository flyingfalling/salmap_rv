#include <salmap_rv/include/ThreadPool.hpp>

using namespace salmap_rv;


ThreadPool::ThreadPool(size_t threads)
  :   stop(false)
{
  for(size_t i = 0; i<threads; ++i)
    {
      workers.emplace_back(
			   [this,i]
			   {
			     for(;;)
			       {
				 std::function<void( size_t )> task; 
				 {
				   std::unique_lock<std::mutex> lock(this->queue_mutex);
				   while( false == this->stop && true == this->tasks.empty() ) 
				     {
				       this->condition.wait_for( lock, std::chrono::microseconds(100) );
				     }
				     
				   
				   
				   if(this->stop && this->tasks.empty())
				     {
				       return;
				     }
				   task = std::move(this->tasks.front()); 
				   this->tasks.pop();
				 }
				 
				 
				 task( i );
			       }
			   }
			   );
    }
}




ThreadPool::~ThreadPool()
{
  {
    std::unique_lock<std::mutex> lock(queue_mutex);
    stop = true;
  }

  condition.notify_all();
  for(std::thread &worker: workers)
    {
      worker.join();
    }
}
const size_t ThreadPool::get_size() const
{
  return workers.size();
}
