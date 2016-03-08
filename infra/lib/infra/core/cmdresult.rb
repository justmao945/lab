module Infra

# http://www.tldp.org/LDP/abs/html/exitcodes.html
class CmdResult
  attr_reader :cwd, :cmd, :output, :time, :timeout, :memout, :exitcode

  def initialize(opts={})
    @cwd = opts[:cwd] || "."
    @cmd = opts[:cmd] || "false"
    @exitcode = opts[:exitcode] || -1
    @timeout = opts[:timeout] || false
    @memout = opts[:memout] || false
    @time = opts[:time] || 0
    @output = opts[:output] || ""
  end

  def filter_output
    @output = yield @output
    self
  end

  # is also killed
  def timeout?
    !success? && @timeout
  end

  def out_of_mem?
    !success? && @memout
  end
  alias memout? out_of_mem?

  def success?
    @exitcode == 0
  end
  alias passed? success?

  def general_error?
    @exitcode == 1
  end

  def shell_error?
    @exitcode == 2
  end

  # Note: this is user specified exit value
  def skipped?
    @exitcode == 3
  end

  def command_can_not_execute?
    @exitcode == 126
  end

  def command_not_found?
    @exitcode == 127
  end

  def invliad_arg_to_exit?
    @exitcode == 128
  end

  def interruped?
    @exitcode == 130
  end

  # kill -9 or -15
  def killed?
    @exitcode == 137 || @exitcode == 143
  end

  # include timeout, memout killed by 9
  def signaled?
    @exitcode > 128 && @exitcode < 255
  end

  def crashed?
    signaled? && ! timeout? && ! out_of_mem?
  end
  
  def exit_out_of_range?
    @exitcode == 255
  end

  # include skipped
  def exited?
    ! signaled? && ! success?
  end

  def failed?
    ! skipped? && exited?
  end

  # should be marked as success
  def ignored?
    skipped? || timeout? || out_of_mem?
  end

  def fatal?
    crashed? || failed?
  end

  def severity
    if success?
      :success
    elsif ignored?
      :warning
    elsif fatal?
      :error
    else
      raise RuntimeError, "bug"
    end
  end

  def need_reproduce?
    #timeout? || out_of_mem? || fatal?
    !success?
  end

  def need_display_output?
    !success?
  end

  def desc
    if passed?
      "passed"
    elsif timeout?
      "timeout"
    elsif out_of_mem?
      "memout"
    elsif skipped?
      "skipped"
    elsif crashed?
      "crashed"
    elsif failed?
      "failed"
    else
      raise RuntimeError, "bug"
    end
  end

  def aligned_desc
    # 7 is the max lenght of desc
    desc.align_left 7
  end
end

end # end of Infra
# vim: set ts=2 sts=2 sw=2:
