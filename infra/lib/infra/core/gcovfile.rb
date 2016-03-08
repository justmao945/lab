module Infra

# 
# count:  nubmer:source
#
class GCovLine
  FMT = [(0..8), (10..14), (16..-1)]
  IGN = '-'
  UNC = '#####'

  EX_IGN_LINE_MARKER    = /\/\/\s*~/
  EX_IGN_BLK_BEG_MARKER = /\/\/\s*~{/
  EX_IGN_BLK_END_MARKER = /\/\/\s*~}/

  EX_IGN_NIL      = -1
  EX_IGN_LINE     = 0
  EX_IGN_BLK_BEG  = 1
  EX_IGN_BLK_BODY = 2
  EX_IGN_BLK_END  = 3

  # these lines should be treated as marked, as they should never be executed
  EX_IGN_LINE_STMTS = [
    /assert\(0/, # only has left side (
  ]

  attr_reader :raw, :count, :number, :source

  def initialize(s)
    @raw = s
    # -1 is ignored by gcov, 0 is not covered, >0 is the execute number
    @count = -1
    # line number
    @number = 0
    # source code line
    @source = nil

    @ex_ign = EX_IGN_NIL
  end

  def parse
    f0 = @raw[FMT[0]].strip
    case f0
    when IGN
      @count = -1
    when UNC
      @count = 0
    else
      @count = f0.to_i
    end

    f1 = @raw[FMT[1]].strip
    @number = f1.to_i

    f2 = @raw[FMT[2]]
    @source = f2

    if @source =~ EX_IGN_BLK_BEG_MARKER
      @ex_ign = EX_IGN_BLK_BEG
    elsif @source =~ EX_IGN_BLK_END_MARKER
      @ex_ign = EX_IGN_BLK_END
    elsif @source =~ EX_IGN_LINE_MARKER
      @ex_ign = EX_IGN_LINE
    end
  
    EX_IGN_LINE_STMTS.each do |stmt|
      if @source =~ stmt
        @ex_ign = EX_IGN_LINE
        break
      end
    end

    self
  end

  def set_ex_ign_blk_body
    @ex_ign = EX_IGN_BLK_BODY
  end

  def ex_ign_nil?
    @ex_ign == EX_IGN_NIL
  end

  def ex_ign_line?
    @ex_ign == EX_IGN_LINE
  end

  def ex_ign_blk_beg?
    @ex_ign == EX_IGN_BLK_BEG
  end

  def ex_ign_blk_end?
    @ex_ign == EX_IGN_BLK_END
  end

  def ex_ign_blk_body?
    @ex_ign == EX_IGN_BLK_BODY
  end
end


class GCovFile < TextFile

  attr_reader :lines

  def initialize(file)
    super
    @lines = []
    s = -1
    @data.each_line do |l|
      line = GCovLine.new(l).parse
      if line.ex_ign_blk_beg?
        s = 0
      elsif line.ex_ign_blk_end?
        s = 1
      elsif line.ex_ign_nil? && s == 0
        line.set_ex_ign_blk_body
      end
      @lines << line
    end
  end

  def source_file
    File.basename @file, ".gcov"
  end

  def cnt_cov(marked=true)
    if marked
      @lines.count{|l| l.count > 0 || !l.ex_ign_nil?}
    else
      @lines.count{|l| l.count > 0}
    end
  end

  def cnt_uncov(no_marked=true)
    if no_marked
      @lines.count{|l| l.count == 0 && l.ex_ign_nil?}
    else
      @lines.count{|l| l.count == 0}
    end
  end

  def cnt_marked
    @lines.count{|l| !l.ex_ign_nil?}
  end

  def cnt_false_marked
    @lines.count{|l| l.count > 0 && !l.ex_ign_nil?}
  end

  # @return [0, 100]
  def coverage
    self.class.calc_coverage cnt_cov, cnt_uncov
  end

  def color
    self.class.select_color coverage
  end

  def self.select_color(cov)
    if cov == 100
      :green
    elsif cov == 0
      :red
    else
      :brown
    end
  end

  def self.calc_coverage(cov, uncov)
    if uncov == 0
      cov == 0 ? 0 : 100
    else
      (1.0 * cov / (cov + uncov) * 100).to_i
    end
  end
end


class GCovFiles
  attr_reader :files, :parsed_files

  def initialize(files)
    @files = files
    @parsed_files = files.map{|f| GCovFile.new f}
  end

  def coverage
    GCovFile.calc_coverage cnt_cov, cnt_uncov
  end

  def cnt_cov
    @parsed_files.inject(0){|s,x| s + x.cnt_cov}
  end

  def cnt_uncov
    @parsed_files.inject(0){|s,x| s + x.cnt_uncov}
  end

  def cnt_marked
    @parsed_files.inject(0){|s,x| s + x.cnt_marked}
  end

  def cnt_false_marked
    @parsed_files.inject(0){|s,x| s + x.cnt_false_marked}
  end

  def empty?
    @parsed_files.empty?
  end

  def color
    GCovFile.select_color coverage
  end
end

end
# vim: set ts=2 sts=2 sw=2:

