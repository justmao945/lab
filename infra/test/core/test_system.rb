require 'fileutils'

class TestShell < Test::Unit::TestCase
  def test_system_shell
    $log.writeln __FILE__

    res = System.shell("echo hello") do |ev, dat|
      $log.writeln "#{ev}, #{dat}"
    end
    $log.writeln res
  
    res1 = System.shell("echo world && echo this") do |ev, dat|
      $log.writeln "#{ev}, #{dat}"
    end
    $log.writeln res1

    tar_path = "#{TEST_TMP_DIR}/system/tar_a"
    FileUtils.mkdir_p tar_path
    System.write "hello world", "#{tar_path}/a.txt"
    Tar.tar_zcf "#{tar_path}.tar.gz", tar_path

    $log.writeln Tar.tar_xf_mem "#{tar_path}.tar.gz"

    cmds = {
      :a=> "echo a",
      :b=> "echo b",
      :c=> "false",
      :d=> "echo d",
    }
    graph = TopGraph.new
    graph.set_relations(
      :b=> [:a],
      :c=> [:b],
      :d=> [:c],
    )
    $log.writeln System.shell_graph(cmds, graph).to_pretty

  end
end

# vim: set ts=2 sts=2 sw=2:
