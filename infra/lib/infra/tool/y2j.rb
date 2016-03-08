
module Infra

class ToolYAML2JSON < ExeContext
  # add options
  def self.init_options(o)
    o.add_banner "Usage: y2j <input yaml> <output json>"
    o.add_description <<DES
Translate YAML file to JSON file.
DES
  end

  def run
    raise OptionParser::InvalidArgument, "require <intput> <output>" if @option.args.length != 2
    input, output = *@option.args
    System.write YAML.load_file(input).to_pretty, output
  end
end

Main.add_tool ToolYAML2JSON, "y2j", "yaml to json"

end
# vim: set ft=ruby ts=2 sts=2 sw=2:
