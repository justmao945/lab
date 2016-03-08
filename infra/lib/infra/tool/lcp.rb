#
# rename this file to <tool name>.rb and add this file to modules.rb
# to add a new tool.
#
module Infra

class ToolLightCopy < ExeContext
  # add options
  def self.init_options(o)
    o.add_banner "Usage: lcp <src dir> <dest dir>"
    o.add_description <<DES
Light weight copy tool, only create directories in destination directory,
files will be symbolic linked for saving space and keep link with source.
DES
    add_option_color o
  end

  def run
    raise OptionParser::MissingArgument, "<src dir> <dest dir>" unless @option.args.length >= 2
    src_dir, dest_dir = *System.expand_paths(@option.args)
    System.lcp_r src_dir, dest_dir
  end
end

Main.add_tool ToolLightCopy, "lcp", "light weight copy"

end
# vim: set ft=ruby ts=2 sts=2 sw=2:
