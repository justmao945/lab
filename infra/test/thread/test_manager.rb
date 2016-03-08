
class TestManager < Test::Unit::TestCase
  def test_manager_run
    $log.writeln __FILE__

    mgr = Manager.new [
      Job.new("job1", [Task.new("task1")]),
      Job.new("job2", [Task.new("task2")]),
    ]
    mgr.run
    $log.writeln mgr.to_pretty
  end
end

# vim: set ts=2 sts=2 sw=2:
