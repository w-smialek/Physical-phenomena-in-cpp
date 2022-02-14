import numpy as np
import matplotlib.pyplot as plt
import csv

###
### PLOT
###

with open('C:/Users/Wojtek/Desktop/Programowanie/cpp/wirlog.txt') as file:
    lines = file.readlines()

x = [i for i in range(len(lines))]
y = [i for i in lines]
y = [a.split(',') for a in y]
y = [[int(i) for i in j[:-1]] for j in y]
y_all = [sum(i) for i in y]

print(x)
print(y_all)

fig, ax = plt.subplots()
ax.plot(x,y_all)
ax.grid(True)

ax.set_title("Ilość wirusów w kolejnych krokach, deactProb 0.01, reprProb 0.05", va='bottom')
plt.show()

