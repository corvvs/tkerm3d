W = 16
H = 6

slines = `dd`.split("\n").map{
  |s| (" " * W) + s + " " + (" " * W)
};
FW = slines[0].size
slines = ([" " * FW] * H) + slines + ([" " * FW] * H)
FH = slines.size
$stderr.puts FW, FH
OL = 23
OR = 25
OU = 0
OD = 6

plines = slines[OU .. -1 - OD]
lines = plines.map{ |s|
  s.split("")[OL .. -1 - OR].map{ |c| c == " " ? c : "#" }
}
$stderr.puts lines.size, lines[0].size

LN = lines.size
GH = (FH - OU - OD) / H
GW = (FW - OL - OR) / W

$stderr.puts GW, GH
(0...H).map{ |k| 
  sublines = lines[k * GH ... (k + 1) * GH].transpose
  (0...W).map{ |l|
    puts sublines[l * GW ... (l + 1) * GW].transpose.map{ |s| s.join }
  }
}
