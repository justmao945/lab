module Infra

class ExeContext
  attr_reader :argv, :input, :output, :option, :console

  # Block called before parse option
  #   {|self| }
  # @input [IO]
  # @outupt [IO]
  # @argv when String, command line string
  #       when Array, command line args
  #       when Hash, opt=> value, "args"=> [list of args]
  def initialize(input, output, argv)
    @input = input
    @output = output
    
    case argv
    when String
      @argv = argv.shellsplit
    when Array, Hash
      @argv = argv.dup
    else
      raise TypeError, argv
    end

    @option = Option.new
    @console = Console.new output

    if block_given?
      # do option initialize..
      yield self
    end

    if @argv.is_a? Array
      @option.order @argv
    else
      @argv.each do |name, val|
        if name == "args"
          @option.set_args val
        else
          @option.set_opt name, val
        end
      end
    end

    if @option.added.include? "--color"
      @console.color_always = @option.has_opt? "--color"
    end
  end

  # Simply print to @console used by shell
  def shell_block(out_pattern=/.*/)
    ->(ev, dat) do
      next if dat.is_a?(String) && dat !~ out_pattern
      case ev
      when :on_ready
        @console << :note << "$ #{dat}"
      when :on_run
        @console << dat 
      when :on_exit
        @console << dat.severity << "exited with #{dat.exitcode}"
      end
      @console << :end << "\n" << :flush
    end
  end

  # main method to do all things
  def run
    raise RuntimeError, "unimplement"
  end

  # init options here
  def self.init_options(o)
  end

  def self.add_option_color(o)
    o.add_opt "--[no-]color", "always print color info even stdout is not a terminal"
  end

  # call from terminal, use standard input and output
  def self.from_term(args)
    new $stdin, $stdout, args do |ctx|
      init_options ctx.option
    end.run
  end

  # call from code for test or other component, input and
  # output are able to be controled
  def self.from_code(input, output, opts)
    new(input, output, opts).run
  end
end

end
# vim: set ts=2 sts=2 sw=2:
