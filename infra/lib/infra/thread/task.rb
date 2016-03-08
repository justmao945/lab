module Infra

# 
# Task is the minimum unit to run in manager.
# Every task has basic info and a +run+ method to do the task.
# 
class Task
  # Class independent
  def self.new_id
    @id_zygote = @id_zygote.nil? ? 0 : @id_zygote + 1
  end

  attr_reader :id, :name, :result, :time
  attr_accessor :owner
  no_record :owner

  def initialize(name)
    @id = self.class.new_id
    @name = name
    @result = nil
    @time = 0

    # will be set by its owner, e.g. Job
    @owner = nil
  end

  # overwirte this function to implement functions
  # @return should always be the result
  def run(*a, &b)
    nil
  end

  # call this to run a task
  # @a Passed to +run+
  def track_run(*a, &b)
    start = Time.now
    @result = run *a, &b
    @time = Time.now - start
    @result
  end
end

end
# vim: set ts=2 sts=2 sw=2:
