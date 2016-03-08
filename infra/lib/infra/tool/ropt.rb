#
module Infra

class ToolRandOpt < ExeContext
  # add options
  def self.init_options(o)
    o.add_banner "Usage: ropt [option...]"
    o.add_description <<DES
This is a tool is used to create random option file according to predefined spec.
Spec format example, a JSON file:
{
  "opts": {
    "-bool_option": null,
    "-single_val_option": "hello",
    "-multiple_vals_option": ["val1", "val2"],

    "-input": "path/to/file",
    "-output": "path/to/dir",
    "-additional_opt": null,
    "-generate": ["a", "b", "c"],
    "-select": ["a|b", "c|d"]
  },
  "usages": [
    ["-input", "-output", "-additional_opt"],
    ["-generate"]
  ]
}

The opts section lists all valid options for an application with valid arguments.
The usage section lists all valid use model to test multiple values options.
DES
    o.set_opt "-s", "ropt.json"
    o.set_opt "-o", "ropt_out"
    o.set_opt "-e", ".arg"

    o.add_opt "-s", "--spec <file>", "path to spec file, default is ropt.json"
    o.add_opt "-o", "--output <dir>", "output directory, default is ropt_out/"
    o.add_opt "-e", "--ext <.ext>", "file extension for output file, default is .arg"
    add_option_color o
  end

  def run
    spec = JSON.parse File.read @option.get_opt "-s"
    out  = @option.get_opt "-o"
    ext  = @option.get_opt "-e"

    ext = ".#{ext}" unless ext.start_with? "."

    oc = OptionCombiner.new spec["opts"], spec["usages"]
    FileUtils.mkdir_p out
    res = oc.enum_to_s do |ev, dat|
      @console.writeln "#{ev} => #{dat}"
    end
    res.each_with_index do |opts, i|
      out_file = File.join out, i.to_s, "#{i}#{ext}"
      FileUtils.mkdir_p File.dirname out_file
      System.write opts, out_file
    end
    true
  end
end

Main.add_tool ToolRandOpt, "ropt", "random option generator"

end
# vim: set ft=ruby ts=2 sts=2 sw=2:
