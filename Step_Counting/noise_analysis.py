import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from IPython.display import display
from scipy.signal import firwin, lfilter, freqz


def step_counter(a, threshold=0.0):
    steps = 0
    peaks = []
    peaks.append(0)
    for i in range(1, len(a)-1):
        peaks.append(int((a[i-1]+threshold) < a[i] and a[i] > (a[i+1]-threshold)))
    peaks.append(0)

    k=0
    D=0
    j=0
    for j in range(len(a)):
        if peaks[j] == 1:
            if k != 0:
                D=j - k - 1
                # print('D = j - k - 1: %d = %d - %d - 1 '%(D,j,k))
                if (D > 2):
                    steps=steps+1
            k=j
    if j == len(a):
        D=len(a)-k
        if(D > 2):
            steps=steps+1
    return steps, peaks


data=pd.read_csv('0.txt')
x=data['x']
y=data['y']
z=data['z']
a=np.sqrt(x*x+y*y+z*z)
samples=[]

step=5
for i in range(0, a.shape[0], step):
    samples.append(a[i])
    # print(i)
# np.savetxt("foo.csv", samples)
# count, peak=step_counter(a,0.12)


plt.plot(samples)
plt.title("Before")

n = np.std(a)
print(n)
# plt.show()
