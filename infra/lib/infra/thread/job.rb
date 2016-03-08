module Infra

#
# A Job is a task that contains many tasks.
# Job is abso able to run tasks in parallel.
#
class Job < Task
  attr_accessor :tasks
  no_record :tasks

  def initialize(name, tasks)
    raise TypeError, tasks unless tasks.is_a? Array
    super(name)
    @result = tasks
    @tasks = tasks
    tasks.each {|t| t.owner = self}
  end

  def size
    tasks.size
  end

  #
  #   {|event, task, i|}
  #   when
  #     :on_task_ready, i is the index of task in the list
  #     :on_task_done, i means the task is the i-th finished one
  # @opts
  #   :in_threads=>n
  #   :start=>->(item, i, result)
  #   :finish=>->(item, i, result)
  # @return[task...], unfinished tasks are nil
  def run(opts={})
    mutex = Mutex.new
    finished_i = 0
    Parallel.map_with_index(tasks, opts) do |task, task_i|
      yield :on_task_ready, task, task_i if block_given?
      task.track_run
      yield :on_task_done, task, finished_i if block_given?
      mutex.synchronize { finished_i += 1 }
      task
    end
  end
end

end
# vim: set ts=2 sts=2 sw=2:
