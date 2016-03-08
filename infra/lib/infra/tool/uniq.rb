#
module Infra

class ToolUniq < ExeContext
  # add options
  def self.init_options(o)
    o.add_banner "Usage: uniq <list file>"
    o.add_description <<DES
Unique file list according to the content of file itself.
DES
    add_option_color o
  end

  def run
    flist = @option.args
    raise OptionParser::MissingArgument, "<list file>" if flist.empty?
    System.uniq_flist(System.readlines flist.first).each do |l|
      @console << l << "\n" << :flush
    end
  end
end

Main.add_tool ToolUniq, "uniq", "unique file list"

end
# vim: set ft=ruby ts=2 sts=2 sw=2:
