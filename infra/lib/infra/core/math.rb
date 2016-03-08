module Infra

# simple math utils
class Math
  def self.clamp(val, min, max)
    [val, min, max].sort[1]
  end

  def self.factorial(n, o=1)
    (o..n).inject(1){|prod, x| prod * x}
  end

  def self.max(a, b)
    a > b ? a : b
  end

  def self.min(a, b)
    a < b ? a : b
  end

  def self.rCn(r, n)
    raise ArgumentError if r > n
    factorial(n) / factorial(r) / factorial(n - r)
  end

  #
  # [1,2,3] => [[1], [2], [3], [1, 2], [1, 3], [2, 3], [1,2,3]]
  #
  # @len max
  # @return [[], [elem...], ...]
  def self.combination(elems, len=elems.size)
    raise TypeError,elems unless elems.is_a? Array
    res = []
    (0..clamp(len, 0, elems.size)).each do |i|
      t = elems.combination(i).to_a
      if block_given?
        res << yield(t)
      else
        res.concat t
      end
    end
    res
  end

  #
  # Combination for small array is not a problem, but you know, it will explode
  # when the array becomes larger, so we set a limit and pick random values from elems.
  # Saddly, we can not handle such large array.
  #  
  # @len  max length of a combination in result
  # @limit max number of combinations with the same length
  def self.rand_combination(elems, len=elems.size, limit=100)
    raise TypeError,elems unless elems.is_a? Array
    res = []
    (0..len).each do |r|
      rcn = rCn(r, elems.size)
      if rcn <= limit
        res.concat elems.combination(r).to_a
      else
        limit.times do |i|
          res << elems.sample(r)
        end
      end
    end
    res
  end

  #  [[1,2], [3,4]] => [[1,3], [1, 4], [2, 3], [2, 4]]
  #
  # @elems
  # @return nil if have no elems
  #         elems itself if only has one elem 
  #         >= 2 elems, production array
  def self.product(elems)
    raise TypeError, elems unless elems.is_a? Array
    if elems.empty? 
      nil
    elsif elems.size == 1
      elems
    else
      elems[0].product(*elems[1..-1])
    end
  end

end

end
# vim: set ts=2 sts=2 sw=2:
