module Infra

# Global command line options
class Option
  attr_reader :added, :required, :value, :args

  def initialize
    @op = OptionParser.new
    # opt => bool
    @added = {}
    # opt => bool
    @required = {}
    # opt => value
    @value = {}
    # remaining args, different with options
    @args = []
    
    # array of block
    #   { |@vlaue| return bool }
    @restrictions = []
  end

  # @opts [String or Array] options
  # @return true if is specified
  def get_opt(*opts)
    res = opts.map do |opt|
      raise Error.new_inv_option opt unless @added.include? opt
      @value[opt]
    end
    res.size == 1 ? res.first : res
  end

  def has_opt?(*opts)
    get_opt *opts
  end

  # Set option and value manually.
  # This is useful to set default values and construct options.
  def set_opt(opt, value)
    @added[opt] = true
    @value[opt] = value
  end

  # overwrite the remaining args.
  def set_args(args)
    raise TypeError args unless args.is_a? Array
    @args = args
  end

  def add_banner(str)
    @op.banner = str
  end

  def add_description(str)
    @op.separator ""
    @op.separator str
    @op.separator ""
  end

  # the first opt has the optional option :required
  def add_opt(*args)
    opt, required = args.first.split(":")
    opt_name = opt.split(" ").first.gsub(/\[.*\]/, "")
    @added[opt_name] = true
    if required
      @required[opt_name] = true
    end
    @op.on(*[opt, *args[1..-1]]) do |v|
      @value[opt_name] = v
    end
  end

  def add_restiction(error, &block)
    @restrictions.push [error, block]
  end

  # call the option parser, will not change args, call :args to 
  # get remaining parsed args.
  def order(args)
    @args = args.dup
    @op.order! @args
    @required.each do |opt, _|
      raise Error.new_mis_option(opt) unless has_opt? opt
    end
    @restrictions.each do |restriction|
      error, block = *restriction
      raise error unless block.call(@value)
    end
  end

  # translate values and args to command line args
  # @args Leading args, e.g. sub commands
  # @return Array of String
  def to_argv(*args)
    argv = args.dup
    @value.each do |opt, val|
      case val
      when true
        argv << opt
      when false
        # skip
      when Array
        argv << opt << val.join(",")
      else
        argv << opt << val
      end
    end
    argv.concat @args
  end

  def to_cmd(exe)
    "#{exe} #{to_argv.join " "}"
  end
end

end
# vim: set ts=2 sts=2 sw=2:
