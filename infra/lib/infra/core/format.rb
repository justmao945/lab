module Infra

class Progress
  def initialize(i, n)
    @i, @n = i, n
  end

  def length
    to_s.length
  end

  def to_s
    "[#{@i.to_s.align_right @n.to_s.length}/#{@n}]"
  end
end


# Easy to format a table    
class Table
  attr_accessor :title, :padding
  attr_reader :data2d

  def initialize
    @padding = 2
    @title = []
    @data2d = []
  end

  def add(row)
    raise TypeError, row unless row.is_a? Array
    raise ArgumentErro, row unless row.length <= @title.length
    @data2d << row
  end

  ##
  #  {|e, r, s|}
  #  case e
  #  when :on_title, r is @title, s is the string
  #  when :on_row, r is the i-th row, s is the string
  # 
  def to_s
    max_width = @title.map{|h| h.to_s.length}
    @data2d.each do |row|
      row.each_with_index do |col, i|
        max_width[i] = col.to_s.length if col.to_s.length > max_width[i]
      end
    end
    
    # add title 
    line = ''
    @title.each_with_index do |h, i|
      line << h.to_s.align_left(@padding + max_width[i])
    end
    title = block_given? ? yield(:on_title, @title, line) : line
    res = "#{title}\n"

    # add data
    @data2d.each_with_index do |row, i|
      line = ''
      row.each_with_index do |col, j|
        line << col.to_s.align_left(@padding + max_width[j])
      end
      row_str = block_given? ? yield(:on_row, row, line) : line
      res << "#{row_str}\n"
    end

    res
  end
end


end
# vim: set ts=2 sts=2 sw=2:
