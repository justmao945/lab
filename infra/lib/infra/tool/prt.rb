module Infra

class ToolPrettyPrinter < ExeContext
  def self.init_options(o)
    o.add_banner "Usage: prt <option...> <file...>"
    o.add_description <<DES
This tool is used to print file pretty according to its format.
Currently support file types:
* JSON
DES

    o.add_opt "-t", "--type <type>", "file format type"
    o.add_opt "-e", "--execute <expr>", "execute ruby expression before printer"
    add_option_color o
  end

  def run
    ty = @option.get_opt "-t"
    ep = @option.get_opt "-e"
    @option.args.each do |file|
      if file.end_with? ".json"
        ty ||= "JSON"
      end

      case ty
      when "JSON"
        data = JSON.parse File.read file
        data = eval(ep || "data")
        @console.writeln data.to_pretty
      end
    end
  end
end

Main.add_tool ToolPrettyPrinter, "prt", "pretty print file"

end
# vim: set ft=ruby ts=2 sts=2 sw=2:
