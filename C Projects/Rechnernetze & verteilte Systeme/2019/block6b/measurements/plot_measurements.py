# -*- coding: utf-8 -*-
"""
Created on Sun Jan 19 15:11:35 2020

@author: test
"""

import numpy as np
import matplotlib.pyplot as plt

a = np.loadtxt("0.de.pool.ntp.org.txt", skiprows=1,delimiter=';',usecols=(1,2,3,4,5))
b = np.loadtxt("1.de.pool.ntp.org.txt", skiprows=1,delimiter=';',usecols=(1,2,3,4,5))
c = np.loadtxt("2.de.pool.ntp.org.txt", skiprows=1,delimiter=';',usecols=(1,2,3,4,5))
d = np.loadtxt("3.de.pool.ntp.org.txt", skiprows=1,delimiter=';',usecols=(1,2,3,4,5))

plt.plot(a.T[0],a.T[4],label = "0.de.pool.ntp.org.txt")
plt.plot(a.T[0],b.T[4],label = "1.de.pool.ntp.org.txt")
plt.plot(a.T[0],c.T[4],label = "2.de.pool.ntp.org.txt")
plt.plot(a.T[0],d.T[4],label = "3.de.pool.ntp.org.txt")

plt.legend(loc='best')
plt.xlabel('Nummer des Aufrufs')
plt.ylabel("Offset" + " in Millisekunden")
plt.savefig("offset" + "_all" + ".pdf")
plt.show()


plt.plot(a.T[0],a.T[3],label = "0.de.pool.ntp.org.txt")
plt.plot(a.T[0],b.T[3],label = "1.de.pool.ntp.org.txt")
plt.plot(a.T[0],c.T[3],label = "2.de.pool.ntp.org.txt")
plt.plot(a.T[0],d.T[3],label = "3.de.pool.ntp.org.txt")

plt.legend(loc='best')
plt.xlabel('Nummer des Aufrufs')
plt.ylabel("Delay" + " in Millisekunden")
plt.savefig("delay" + "_all" + ".pdf")
plt.show()

plt.plot(a.T[0],a.T[1],label = "0.de.pool.ntp.org.txt")
plt.plot(b.T[0],b.T[1],label = "1.de.pool.ntp.org.txt")
plt.plot(c.T[0],c.T[1],label = "2.de.pool.ntp.org.txt")
plt.plot(d.T[0],d.T[1],label = "3.de.pool.ntp.org.txt")

plt.legend(loc='best')
plt.xlabel('Nummer des Aufrufs')
plt.ylabel("Root despersion" + " in Millisekunden")
plt.savefig("root_despersion" + "_all" + ".pdf")
plt.show()