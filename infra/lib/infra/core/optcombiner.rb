module Infra

# Produce all combinations for given options for testing.
#  
# This combiner won't generate all possible combinations, because that's too large to handle.
# Only generate:
#   1. combine all given options with valid value, won't combine different values
#   2. combine all values for options which have value, won't combine diffent options
#
class OptionCombiner
  attr_reader :opts, :builtin_vals

  #
  # @opts Should fill valid used model, and combiner will produce invalid cases
  # {
  #   "-option"=> true, String, Number, or Array
  # }
  #
  # true means this option is used as a switch
  # String means this option has an value, and this string should be valid
  # Array means this option can have multiple values, and thsi array should be valid
  # 
  # please note that, an operator "|" can be put into a string, which acts
  # as selectable value.
  #
  # e.g.
  #  "-opt"=> ["a|b", "c|d"] 
  # only generates "-opt a c", "-opt b d", etc. instead of "-opt a b", "-opt c d".
  #
  # @usages Should be filled with vaild use model command line option names,
  #      this used to test options which has multiple values
  #  [
  #   ["-option", ...], ...
  #  ]
  #
  def initialize(opts, usages)
    raise TypeError, opts unless opts.is_a? Hash
    raise TypeError, usages unless usages.is_a? Array
    #
    @opts = opts
    @usages = usages
    # builtin values
    @builtin_vals = ["-1", "100", "", "/permission_denied"]
  end

  #
  # block for progress bar:
  # {|ev, data| block }
  #
  # @comb_limit combination set with same length limit number
  #
  # Please note that, opt val pair may not be unique for result.
  # @return [ {"-option"=> value, ...}, ... ]
  def enum(comb_limit=50)
    yield :enum_start if block_given?

    res = []
    # generate possible combination for all given options
    yield :combine_opts, @opts.size if block_given?
    opts = @opts.keys
    opt_combined = Math.rand_combination opts, opts.size, comb_limit
    opt_combined.each do |opt_grp|
      grp = {}
      opt_grp.each do |opt|
        grp[opt] = @opts[opt]
      end
      res << grp
    end

    yield :combine_vals if block_given?
    # generate possible values for each option
    opt_vals = {}
    @opts.each do |opt, val|
      case val
      when true
        # set to [], no need to interpolate
        opt_vals[opt] = []
      when String, Numeric
        opt_vals[opt] = [val.to_s] + @builtin_vals
      when Array
        # may be []
        vals = val + @builtin_vals
        opt_vals[opt] = Math.rand_combination vals, val.size, comb_limit
      else
        raise TypeError, opt
      end
    end
  
    yield :interpolate_vals, @usages.size if block_given?
    # interpolate possible values for each option in valid use model
    @usages.each do |cmd|
      grp = {}
      cmd.each do |opt|
        grp[opt] = @opts[opt]
      end

      cmd.each do |opt|
        vals = opt_vals[opt]
        next if vals.nil?
        vals.each do |val|
          # dup grp in block, should not be outside the block
          ngrp = grp.dup
          ngrp[opt] = val
          # ngrp may contain opt-val whose val is []
          res << ngrp
        end
      end
    end
    
    yield :select_opts if block_given?
    # get all options need post processing of the operator "|"
    sel = []
    res.each do |grp|
      grp.each do |opt, val|
        case val
        when true
        when String, Numeric
          if val.to_s.index "|"
            sel << grp
            break # loop grp
          end
        when Array
          if val.count{|i| i.index "|"} > 0
            sel << grp
            break
          end
        else
          System.assert false, "bug"
        end
      end
    end
 
    yield :product_selectors, sel.size if block_given?
    # find all extra result from sel
    extra = []
    sel.each do |grp|
      factors = []
      grp.each do |opt, val|
        case val
        when true 
          factors << [[opt, val]]
        when String, Numeric
          factors << val.to_s.split("|").map{|vsp| [opt, vsp]}
        when Array
          # ival may be empty, the builtin empty value "" should be there
          vals = Math.product val.map{|ival| ival.empty? ? [""] : ival.split("|")}
          # val may be [], so vals may be nil
          if vals.nil?
            factors << [[opt, []]]
          else
            factors << vals.map{|vsp| [opt, vsp]}
          end
        else
          System.assert false, "bug"
        end
      end
      Math.product(factors).each do |grp|
        ngrp = {}
        grp.each do |opt_val|
          opt, val = *opt_val
          ngrp[opt] = val
        end
        extra << ngrp
      end
    end
    yield :enum_done if block_given?

    res - sel + extra
  end

  # Command line string is uniq in result.
  # @return  Array of String command line args ["", "", ...]
  def enum_to_s(e=nil, &b)
    yield :enum2s_start if block_given?
    res = []
    (e || enum(&b)).each do |grp|
      args = ""
      grp.each do |opt, val|
        case val
        when true
          args << " #{opt}"
        when String, Numeric
          args << " #{opt} #{val}"
        when Array
          args << " #{opt} " << val.join(" ")
        else
          raise TypeError, val
        end
      end
      res << args
    end
    yield :enum2s_end if block_given?
  
    res.uniq!
  end
end

end 
# vim: set ts=2 sts=2 sw=2:
