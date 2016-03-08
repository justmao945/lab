
class TestToolLightCopy < Test::Unit::TestCase
  def test_lcp
    $log.writeln __FILE__
   
    args_dir = File.join TEST_DATA, "args"

    opts = {
      "args"=> [args_dir, "#{TEST_TMP_DIR}/tool/lcp"],
    }
    out = StringIO.new
    ToolLightCopy.from_code $stdin, out, opts
    $log.writeln out.string
  end
end

# vim: set ts=2 sts=2 sw=2:
