
def sf_a(arr)
  arr.each_with_index do |x, i|
    j = rand(i+1)
    arr[i], arr[j] = arr[j], arr[i]
  end
  arr
end

def sf_b(arr)
  arr.each_with_index do |x, i|
    j = i + rand(arr.size - i)
    arr[i], arr[j] = arr[j], arr[i]
  end
  arr
end

# this will not produce the real random sequence, why?
def sf_bug(arr)
  arr.each_with_index do |x, i|
    j = rand arr.size
    arr[i], arr[j] = arr[j], arr[i]
  end
  arr
end

arr = [1,2,3]

cnt_a, cnt_b, cnt_c = {}, {}, {}

def inc(hash, key)
  hash[key] ||= 0
  hash[key] += 1
end

100000.times do 
  a = sf_a arr.dup
  inc cnt_a, a.to_s
  b = sf_b arr.dup
  inc cnt_b, b.to_s
  c = sf_bug arr.dup
  inc cnt_c, c.to_s
end

p cnt_a
p cnt_b
p cnt_c
