#
# Hacked builtin types
#
class Object
  @@no_record_attrs = []

  def self.no_record(*a)
    a.each do |s|
      @@no_record_attrs << "@#{s}".to_sym
    end
  end

  def to_record
    vars = instance_variables
    return "#<#{self.class}:#{__id__}>" if vars.empty?

    dict = {}
    vars.each do |var|
      next if @@no_record_attrs.include? var
      dict[var] = instance_variable_get(var).to_record
    end
    dict
  end

  def to_pretty
    begin
      JSON.pretty_generate to_record
    rescue JSON::GeneratorError
      to_record.to_json
    end
  end
end

class NilClass
  def to_record
    self
  end
end

class TrueClass
  def to_record
    self
  end
end

class FalseClass
  def to_record
    self
  end
end

class Symbol
  def to_record
    self
  end
end

class Numeric
  def to_record
    self
  end
end

class String
  def to_record
    self
  end

  def to_md5
    Digest::MD5.hexdigest self
  end

  def align_right(w)
    sprintf "%#{w}s", self
  end

  def align_left(w)
    sprintf "%-#{w}s", self
  end
end

class Array
  def to_record
    map{|i| i.to_record}
  end
end

class Hash
  def to_record
    res = {}
    each do |k,v|
      res[k.to_record] = v.to_record
    end
    res
  end
end

class Queue
  def to_a
    # please don't use dup as the undefined behavior
    a = []
    while not empty? do
      a << pop
    end
    a.each do |i|
      push i
    end
    a
  end
end

class Thread
  def kill_join
    kill
    join
  end
end

# vim: set ts=2 sts=2 sw=2:
