#
module Infra

class ToolKillOrphan < ExeContext
  def self.init_options(o)
    o.add_banner "Usage: ko <pattern>..."
    o.add_description <<DES
Kill orphan processes, which is a child of process init(1).
DES
  end

  def run
    ps = ProcSystem.new
    @option.args.each do |pat|
      sel = ps.sons(1).select do |k,v|
        ps.sons(k).empty? && v.cmdline =~ /#{pat}/
      end
      sel.each do |k,v|
        begin
          @console << "killing #{k} #{v.cmdline}...\n" << :flush
          Process.kill "KILL", k
          Process.wait k
        rescue Exception=> e
          @console << "kill #{k} failed for #{e}\n" << :flush
        end
      end
    end
  end
end

Main.add_tool ToolKillOrphan, "ko", "kill orphan processes"

end
# vim: set ft=ruby ts=2 sts=2 sw=2:
