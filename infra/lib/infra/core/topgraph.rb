module Infra

class TopGraph
  attr_reader :deps

  def initialize
    # name => deps
    @deps = {}
    # name => cond
    @conds = {}
  end

  # @name [String] unique name in this graph
  # @dep
  def set_relation(name, dep=[])
    @deps[name] = dep
    dep.each {|d| @deps[d] ||= [] }
  end

  def set_relations(rel)
    rel.each do |k,v|
      set_relation k, v
    end
  end

  def set_cond(name, cond)
    raise TypeError, cond unless cond == true || cond == false
    @conds[name] = cond
  end

  def test_cond(name)
    c = @conds[name]
    if c.nil?
      @deps[name] ||= []
      c = @deps[name].map {|n| test_cond n}.reduce :&
      @conds[name] = c.nil? ? true : c
    else
      c
    end
  end
end

end
# vim: set ts=2 sts=2 sw=2:
