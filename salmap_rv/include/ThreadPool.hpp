#pragma once
#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>


namespace salmap_rv
{



struct ThreadPool
{
  ThreadPool(size_t);


  
  template<class F, class... Args>
  auto enqueue(F&& f, Args&&... args) 
    -> std::future<typename std::result_of<F(size_t, Args...)>::type>;

  
  ~ThreadPool();
  
  const size_t get_size() const; 
  
  
  
  std::vector< std::thread > workers;
  
  std::queue< std::function<void( size_t )> > tasks;
  
  
  std::mutex queue_mutex;
  std::condition_variable condition;
  bool stop;
};

}

using namespace salmap_rv;



template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) 
  -> std::future<typename std::result_of<F(size_t, Args...)>::type>
{
  using return_type = typename std::result_of<F(size_t, Args...)>::type;
  
  auto task_contents = std::bind( std::forward<F>(f), std::placeholders::_1, std::forward<Args>(args)... );
  
  auto task = std::make_shared< std::packaged_task<return_type(size_t)> >( task_contents );
  
  std::future<return_type> res = task->get_future();
  
  {
    std::unique_lock<std::mutex> lock(queue_mutex);
    
    
    if(stop)
      {
	throw std::runtime_error("enqueue on stopped ThreadPool");
      }
    
    
    
    tasks.emplace([task](size_t a){ (*task)(a); }); 
    
    
  }

  
  condition.notify_one();
  
  return res;
}
