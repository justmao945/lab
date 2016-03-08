module Infra
 
# extend Infra::Message
class Message
  def self.new_not_project(dir)
    self.new "#{dir} is not a project directory"
  end

  def self.new_not_component(dir)
    self.new "#{dir} is not a component directory"
  end

  def self.new_unk_component(name)
    self.new "unknown component: #{name}"
  end

  def self.new_unk_cmd(name)
    self.new "unknown command: #{name}"
  end
end

end
# vim: set ts=2 sts=2 sw=2:
