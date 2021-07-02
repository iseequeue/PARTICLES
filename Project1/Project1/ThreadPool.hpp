#pragma once
#include <boost/thread/thread.hpp>
#include <boost/asio.hpp>
#include <boost/tuple/tuple.hpp>

class ThreadPool
{
public:
    ThreadPool(size_t Threads) :
        io_service(), work(new boost::asio::io_service::work(io_service)), nThreads(Threads), nTasks(0)
    {
        for (std::size_t i = 0; i < nThreads; ++i)
        {
            threads.create_thread(boost::bind(&boost::asio::io_service::run, &io_service));
        }
    }

    ~ThreadPool()
    {
        work.reset();
        io_service.run();
    }

    template<class F>
    void enqueue(F f)
    {
        {
            boost::unique_lock<boost::mutex> lock(mutex_);
            nTasks++;
        }

        void (ThreadPool:: * ff)(boost::tuple<F>) = &ThreadPool::wrapper<F>;

        io_service.post(boost::bind(ff, this, boost::make_tuple(f)));
    }
    void wait()
    {
        boost::unique_lock<boost::mutex> lock(mutex_);
        while (nTasks)
        {
            cond.wait(lock);
        }
    }

    std::size_t nThreads;


private:
    template<class F>
    void wrapper(boost::tuple<F> f)
    {
        boost::get<0>(f)();
        {
            boost::unique_lock<boost::mutex> lock(mutex_);
            nTasks--;
            cond.notify_one();
        }
    }



    boost::asio::io_service io_service;

    boost::shared_ptr<boost::asio::io_service::work> work;

    boost::thread_group threads;


    std::size_t nTasks;
    boost::mutex mutex_;
    boost::condition_variable cond;
};



