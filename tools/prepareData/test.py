from statistics import *

tab_data = []
t = ["c1","c2","c3","c4","c5","c6"]
a = [11, 21, 31, 41, 51, 61]
b = [12, 22, 32, 42, 52, 62]
c = [13, 23, 33, 43, 53, 63]

tab_data.append(t)
tab_data.append(a)
tab_data.append(b)
tab_data.append(c)

col1 = [row[2] for row in tab_data[1:]]
print(col1)
print("Mean",mean(col1))
