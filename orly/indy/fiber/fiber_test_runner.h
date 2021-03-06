/* <orly/indy/fiber/fiber_test_runner.h>

   Copyright 2010-2014 OrlyAtomics, Inc.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. */

#pragma once

#include <base/thread_local_global_pool.h>
#include <orly/indy/fiber/fiber.h>

namespace Orly {

  namespace Indy {

    namespace Fiber {

      /* TODO */
      class TFiberTestRunner
          : public Indy::Fiber::TRunnable {
        NO_COPY_SEMANTICS(TFiberTestRunner);
        public:

        TFiberTestRunner(const std::function<void (std::mutex &, std::condition_variable &, bool &, TRunner::TRunnerCons &)> &test, size_t extra_runners = 0)
            : Test(test), RunnerCons(1UL + extra_runners), Fin(false) {
          const size_t stack_size = 8 * 1024UL * 1024UL;
          TRunner runner(RunnerCons);
          Base::TThreadLocalGlobalPoolManager<TFrame, size_t, TRunner *> frame_pool_manager(30UL, stack_size, &runner);
          if (!TFrame::LocalFramePool) {
            TFrame::LocalFramePool = new Base::TThreadLocalGlobalPoolManager<TFrame, size_t, TRunner *>::TThreadLocalPool(&frame_pool_manager);
          }
          auto launch_fiber_sched = [](TRunner *runner, Base::TThreadLocalGlobalPoolManager<TFrame, size_t, TRunner *> *frame_pool_manager) {
            if (!TFrame::LocalFramePool) {
              TFrame::LocalFramePool = new Base::TThreadLocalGlobalPoolManager<TFrame, size_t, TRunner *>::TThreadLocalPool(frame_pool_manager);
            }
            runner->Run();
            delete TFrame::LocalFramePool;
            TFrame::LocalFramePool = nullptr;
          };

          std::thread t1(std::bind(launch_fiber_sched, &runner, &frame_pool_manager));
          TFrame *frame = TFrame::LocalFramePool->Alloc();
          try {
            frame->Latch(&runner, this, static_cast<TRunnable::TFunc>(&TFiberTestRunner::Run));
          } catch (...) {
            TFrame::LocalFramePool->Free(frame);
            throw;
          }
          std::unique_lock<std::mutex> lock(Mut);
          while (!Fin) {
            Cond.wait(lock);
          }
          runner.ShutDown();
          t1.join();
          if (TFrame::LocalFramePool) {
            delete TFrame::LocalFramePool;
            TFrame::LocalFramePool = nullptr;
          }
        }

        void Run() {
          Test(Mut, Cond, Fin, RunnerCons);
        }

        private:

        const std::function<void (std::mutex &, std::condition_variable &, bool &, TRunner::TRunnerCons &)> &Test;

         TRunner::TRunnerCons RunnerCons;
        std::mutex Mut;
        std::condition_variable Cond;
        bool Fin;

      };

    }  // Fiber

  }  // Indy

}  // Orly