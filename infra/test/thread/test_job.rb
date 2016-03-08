
class TestJob < Test::Unit::TestCase
  def test_job_run
    $log.writeln __FILE__

    job = Job.new "job", [
      Task.new("task1"),
      Task.new("task2"),
    ]
    res = job.run
    $log.writeln job.to_pretty
  end
end

# vim: set ts=2 sts=2 sw=2:
