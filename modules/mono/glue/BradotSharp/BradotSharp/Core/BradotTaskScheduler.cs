using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;

namespace Bradot
{
    /// <summary>
    /// BradotTaskScheduler contains a linked list of tasks to perform as a queue. Methods
    /// within the class are used to control the queue and perform the contained tasks.
    /// </summary>
    public sealed class BradotTaskScheduler : TaskScheduler, IDisposable
    {
        /// <summary>
        /// The current synchronization context.
        /// </summary>
        internal BradotSynchronizationContext Context { get; }

        /// <summary>
        /// The queue of tasks for the task scheduler.
        /// </summary>
        private readonly LinkedList<Task> _tasks = new LinkedList<Task>();

        /// <summary>
        /// Constructs a new BradotTaskScheduler instance.
        /// </summary>
        public BradotTaskScheduler()
        {
            Context = new BradotSynchronizationContext();
            SynchronizationContext.SetSynchronizationContext(Context);
        }

        protected sealed override void QueueTask(Task task)
        {
            lock (_tasks)
            {
                _tasks.AddLast(task);
            }
        }

        protected sealed override bool TryExecuteTaskInline(Task task, bool taskWasPreviouslyQueued)
        {
            if (SynchronizationContext.Current != Context)
                return false;

            if (taskWasPreviouslyQueued)
                TryDequeue(task);

            return TryExecuteTask(task);
        }

        protected sealed override bool TryDequeue(Task task)
        {
            lock (_tasks)
            {
                return _tasks.Remove(task);
            }
        }

        protected sealed override IEnumerable<Task> GetScheduledTasks()
        {
            lock (_tasks)
            {
                foreach (Task task in _tasks)
                    yield return task;
            }
        }

        /// <summary>
        /// Executes all queued tasks and pending tasks from the current context.
        /// </summary>
        public void Activate()
        {
            ExecuteQueuedTasks();
            Context.ExecutePendingContinuations();
        }

        /// <summary>
        /// Loops through and attempts to execute each task in _tasks.
        /// </summary>
        /// <exception cref="InvalidOperationException"></exception>
        private void ExecuteQueuedTasks()
        {
            while (true)
            {
                Task task;

                lock (_tasks)
                {
                    if (_tasks.Any())
                    {
                        task = _tasks.First.Value;
                        _tasks.RemoveFirst();
                    }
                    else
                    {
                        break;
                    }
                }

                if (task != null)
                {
                    if (!TryExecuteTask(task))
                    {
                        throw new InvalidOperationException();
                    }
                }
            }
        }

        public void Dispose()
        {
            Context.Dispose();
        }
    }
}
