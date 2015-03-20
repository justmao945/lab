
def fab(n)
    return 0 if n == 0
    return 1 if n == 1 || n == 2

    a = 1
    b = 1
    c = 0

    (n-2).times do
        c = a + b
        a = b
        b = c
    end
    return c
end


n = ARGV.first.to_i
puts fab n
