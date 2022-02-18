# 平面上の点群を厚くする
# Usage
# cat some.3d | ruby thickener3d.rb [multiple] [distance]

multiples, distance = ARGV
multiples = multiples ? multiples.to_i : 5
distance = distance ? distance.to_f : 0.1
$stderr.puts multiples, distance
`dd`.split("\n").each{ |s|
  elems = s.split(",").map(&:to_f);
  (0...multiples).each{ |i|
    puts [ elems[0], elems[1], elems[2] + distance * i ] * ","
  }
}
