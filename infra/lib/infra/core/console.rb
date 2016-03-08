module Infra

ANSI_COLOR = {
  :black=>         ["\033[30m", "\033[0m"],
  :red=>           ["\033[31m", "\033[0m"],
  :green=>         ["\033[32m", "\033[0m"],
  :brown=>         ["\033[33m", "\033[0m"],
  :blue=>          ["\033[34m", "\033[0m"],
  :magenta=>       ["\033[35m", "\033[0m"],
  :cyan=>          ["\033[36m", "\033[0m"],
  :gray=>          ["\033[37m", "\033[0m"],
  :bg_black=>      ["\033[40m", "\033[0m"],
  :bg_red=>        ["\033[41m", "\033[0m"],
  :bg_green=>      ["\033[42m", "\033[0m"],
  :bg_brown=>      ["\033[43m", "\033[0m"],
  :bg_blue=>       ["\033[44m", "\033[0m"],
  :bg_magenta=>    ["\033[45m", "\033[0m"],
  :bg_cyan=>       ["\033[46m", "\033[0m"],
  :bg_gray=>       ["\033[47m", "\033[0m"],
  :bold=>          ["\033[1m" , "\033[22m"],
  :reverse_color=> ["\033[7m" , "\033[27m"],
}


# local buffer
# Severity:
#   :note
#   :success
#   :warning
#   :error
# 
# Controllers
#   :end
#   :flush
#
class ConsoleBuffer
  attr_accessor :indent

  def initialize(console)
    @console = console
    @color = nil
    @data = ''
    @indent = 0
    @last_flushed = nil
  end

  def <<(s)
    case s
    when :note
      @color = :blue
    when :success
      @color = :green
    when :warning
      @color = :brown
    when :error
      @color = :red
    end

    case s
    when :end
      if @color
        @data << ansi_color(@color, 1)
        @color = nil
      end
    when :flush
      @last_flushed = @data[-1]
      @console.stream_write @data
      @data.clear
    when Symbol
      if last_is_enter?
        @data << ' ' * @indent
      end
      @data << ansi_color(@color, 0)
    else
      s.to_s.each_line do |l|
        if l == "\n" || !last_is_enter?
          @data << l
        else
          @data << ' ' * @indent << l
        end
      end
    end
    self
  end

  def flush
    self << :flush
  end

  def write(s)
    self << s.to_s
  end

  def writeln(s)
    self << s.to_s << "\n" << :flush
  end

  protected
  def ansi_color(c, i)
    if ANSI_COLOR.include?(c) && (@console.isatty? || @console.color_always?)
      ANSI_COLOR[c][i]
    else
      ''
    end
  end

  def last_is_enter?
    @data[-1] == "\n" || @last_flushed == "\n"
  end
end


# global console
class Console
  attr_accessor :color_always

  def initialize(stream, opts={})
    @stream = stream
    @mutex = opts[:mutex] || Mutex.new
    @color_always = opts[:color_always]
    @buffer = new_buf
  end

  def indent
    @buffer.indent
  end

  def indent=(x)
    @buffer.indent = x
  end

  def new_buf
    ConsoleBuffer.new self
  end

  def isatty?
    @stream.isatty
  end

  def color_always?
    @color_always
  end

  # write direct to stream and ignore buffer
  def stream_write(s)
    @mutex.synchronize do
      @stream.write s
      @stream.flush
    end
  end

  def <<(s)
    if s == :flush
      # recursive lock
      @buffer << s
    else
      @mutex.synchronize do
        @buffer << s
      end
    end
    self
  end

  def flush
    @buffer.flush
  end

  def write(s)
    @buffer.write s
  end

  def writeln(s)
    @buffer.writeln s
  end

  def close
    flush
    @stream.close
  end
end

end
# vim: set ts=2 sts=2 sw=2:
