

def mul2x2(a, b)
    r = [[0, 0],[0 ,0]]
    r[0][0] = a[0][0] * b[0][0] + a[0][1] * b[1][0];
    r[0][1] = a[0][0] * b[0][1] + a[0][1] * b[1][1];
    r[1][0] = a[1][0] * b[0][0] + a[1][1] * b[1][0];
    r[1][1] = a[1][0] * b[0][1] + a[1][1] * b[1][1];
    r
end

def pow2x2(a, n)
    raise ArgumentError if n == 0
    return a if n == 1
    t = pow2x2 a, n/2
    t = mul2x2 t, t
    n % 2 != 0 ? mul2x2(t, a) : t
end

def fab(n)
    return 0 if n == 0
    return 1 if n == 1 || n == 2
    magic = [[1,1], [1,0]]
    t = pow2x2 magic, n-2
    t[0][0] + t[0][1]
end

n = ARGV.first.to_i
puts fab n
