#
# rename this file to <tool name>.rb and add this file to modules.rb
# to add a new tool.
#
module Infra

class ToolClass < ExeContext
  # add options
  def self.init_options(o)
    o.add_banner "Usage: tool_name [option...]"
    o.add_description <<DES
This is a tool template.    
DES
    o.add_opt "-t", "--test <arg>", "option description"
  end

  def run
  end
end

Main.add_tool ToolClass, "tool_name", "tool description"

end
# vim: set ft=ruby ts=2 sts=2 sw=2:
