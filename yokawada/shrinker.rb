RW = 20
RH = 40 * 96

lines = `dd`.split("\n")
W = lines[0].size
H = lines.size

IW = W.to_f / RW
IH = H.to_f / RH

(0...RH).each{ |i|
  puts (0...RW).map{ |j|
    c = 0
    s = 0
    ((IH * i).to_i ... [(IH * (i + 1)).to_i, H].min).each{ |ii|
      ((IW * j).to_i ... [(IW * (j + 1)).to_i, W].min).each{ |jj|
        s += 1
        c += (lines[ii][jj] == '#' ? 1 : 0)
      }
    }
    # p [i, j, s, c]
    c >= s / 2 ? "#" : " "
  } * ""
}
