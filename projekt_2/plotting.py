import numpy as np
import matplotlib.pyplot as plt
import csv

data = []

with open('tr.txt') as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    for row in csv_reader:
        data.append([row[0],row[1]])

x = [float(i[0]) for i in data]
fi = x[-1]
x = x[:-1]
y = [float(i[1]) for i in data]
czass = y[-1]
y = y[:-1]

zx = [389.1727367]
zy = [0.]
kx = [389.1727367 + np.cos((fi+czass)*2*np.pi)]
ky = [np.sin((fi+czass)*2*np.pi)]
# x = [float(i[0]) * np.cos(float(i[1])) for i in data]
# y = [float(i[0]) * np.sin(float(i[1])) for i in data]

# print(x)
# print(y)

fig, ax = plt.subplots()#subplot_kw={'projection': 'polar'})
ax.plot(x,y)#theta, r)
ax.plot(zx, zy, marker="o", markersize=20, markeredgecolor="blue", markerfacecolor="green")
ax.plot(kx, ky, marker="o", markersize=10, markeredgecolor="black", markerfacecolor="grey")

# ax.set_rmax(2)
# ax.set_rticks([0.5, 1, 1.5, 2])  # Less radial ticks
# ax.set_rlabel_position(-22.5)  # Move radial labels away from plotted line
ax.grid(True)

ax.set_title("A line plot on a polar axis", va='bottom')
plt.show()

histot = []
with open('hs.txt') as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    for row in csv_reader:
        histot = row
histot = histot[:-1]
histot = [int(i) for i in histot]
# histot = [2,7,9,6,2,6,1,3,5,1,1,2,3,1,0,0,0,1,0,1,2,2,0,1,3,1,1,0,7,2,4,3,6,4,5,4]

chi_kw = 0.
sum = 0.

for i in histot:
    sum += i

for i in histot:
    chi_kw += ((i - sum/360) ** 2)/(sum/360)

histot = [i * [j] for j,i in enumerate(histot)]
histot = [item for sublist in histot for item in sublist]

n, bins, patches = plt.hist(histot, bins=360, facecolor='g', alpha=0.75)
plt.show()
