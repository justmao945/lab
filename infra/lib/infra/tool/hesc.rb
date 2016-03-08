#
module Infra

class ToolHTMLEsc < ExeContext
  def self.init_options(o)
    o.add_banner "Usage: hesc <file...>"
    o.add_description <<DES
read file and escape HTML special chars and print.
DES
    add_option_color o
  end

  def run
    @option.args.each do |f|
      @console.writeln "#{f}\n#{TextFile.new(f).escapeHTML}\n\n"
    end
  end
end

Main.add_tool ToolHTMLEsc, "hesc", "escape HTML"

end
# vim: set ft=ruby ts=2 sts=2 sw=2:
