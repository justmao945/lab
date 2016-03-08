class TestOptionCombiner < Test::Unit::TestCase
  def test_option_combiner
    $log.writeln __FILE__

    oc = OptionCombiner.new({
      "-help"=> true,
      "-input"=> "path/to/file",
      "-output"=> ["path/to/output1", "output2"],
      "-gen"=> ["a", "b"],
      "-select"=> ["a|b", "c|d"],
    },
    [
      ["-input", "-output", "-select"],
    ])

    e = oc.enum
    $log.writeln oc.enum_to_s(e).to_pretty
  end
end

# vim: set ts=2 sts=2 sw=2:
