module Infra

class TextFile
  attr_reader :file, :data

  def initialize(file)
    @file = file
    @data = File.read file
  end

  def save(file=nil)
    fname = file || @file
    FileUtils.mkdir_p File.dirname(fname)
    File.open(fname, "w"){|f| f.write @data}
  end

  def escapeHTML
    CGI.escapeHTML @data
  end

  def unescapeHTML
    CGI.unescapeHTML @data
  end
end

end
# vim: set ts=2 sts=2 sw=2:
