module Infra
 
class Message < StandardError
  attr_reader :info

  def initialize(info)
    @info = info
  end

  def to_s
    @info
  end

  def self.new_exe_failed(cmd, exitcode)
    self.new "#{cmd} exits with #{exitcode}"
  end

  def self.new_file_exist(file)
    self.new "#{file} is already exist"
  end

  def self.new_inv_option(name)
    self.new "invalid option: #{name}"
  end

  def self.new_mis_option(opt)
    self.new "missing required option: #{opt}"
  end

  def self.new_no_such(file)
    self.new "no such file or directory: #{file}"
  end
end

class Error < Message
  attr_reader :error

  # @info string message
  def initialize(info)
    @error = "error: #{info}"
  end

  def to_s
    @error
  end
end

class Warn < Message
  attr_reader :warn

  # @info string message
  def initialize(info)
    @warn = "warning: #{info}"
  end

  def to_s
    @warn
  end
end

end
# vim: set ts=2 sts=2 sw=2:
