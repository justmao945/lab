
class TestShell < Test::Unit::TestCase

  class Task1
    def initialize(t)
      @t = t
    end

    def run
      sleep @t
    end
  end

  class Task2
    def initialize(t)
      @t = t
    end

    def run
      sleep @t
      raise Parallel::Kill
    end
  end

  def test_parallel
    $log.writeln __FILE__

    t1 = Task1.new 5
    t2 = Task2.new 0
    
    time = Time.now
    res = Parallel.map [t1, t2], :in_threads=>2 do |t|
      t.run
    end
    $log.writeln res
    $log.writeln "#{Time.now - time}"
  end

  class Task3
    def initialize(q)
      @q = q
    end

    def run
      @q.pop
    end
  end

  class Task4
    def initialize(q)
      @q = q
    end

    def run
      @q.push 1
    end
  end

  def test_queue
    q = Queue.new
    t3 = Task3.new q
    t4 = Task4.new q
    res = Parallel.map [t3, t4], :in_threads=>2 do |t|
      t.run
    end
    $log.writeln res
  end
end

# vim: set ts=2 sts=2 sw=2:
