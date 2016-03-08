module Infra

class Manager
  attr_reader :jobs

  def initialize(jobs)
    raise TypeError, jobs unless jobs.is_a? Array
    # larger jobs has higher priority
    @jobs = jobs
    jobs.each {|j| j.owner = self}
  end

  def get_job_by_name(name)
    @jobs.select{|j| j.name == name}.first
  end

  def size
    @jobs.size
  end

  # jobs are run in order
  def time
    @jobs.inject(0) {|s,x| s + x.time}
  end

  # {|event, data, i|}
  #   :on_job_ready,   data is job, i is the job index in the queue
  #   :on_task_ready,  data is task, i is the task index in the list
  #   :on_task_done,   data is task, i-th finished task, not the index in the list
  #   :on_job_done,    data is job,  i is the job index in the queue
  # @opts
  # @return [job...]
  def run(opts={})
    @jobs.each_with_index.map do |job, job_i|
      yield :on_job_ready, job, job_i if block_given?
      job.track_run(opts) do |ev, task, task_i|
        yield ev, task, task_i if block_given?
      end
      yield :on_job_done, job, job_i if block_given?
      job
    end
  end
end

end
# vim: set ts=2 sts=2 sw=2:

