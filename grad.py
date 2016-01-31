import math

xx = [1985, 1986, 1987, 1988, 1989, 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008]
yy = [340, 682, 1231, 2069, 3509, 5283, 7557, 11033, 16009, 24134, 33786, 44043, 55312, 69209, 86047, 109478, 128375, 140766, 158722, 182140, 207896, 233041, 255396, 270334]

for i in range(0,len(yy)):
	yy[i] /= 350000.0
for i in range(0,len(xx)):
	xx[i] = (xx[i] - 1997.0)/25.0

mi = 2222222

rate = 0.009
a = 2.4
b = 2.3
c = 0.0
d = 0.0


for i in range(0, 30000):
	a1 = 0.0
	b1 = 0.0
	c1 = 0.0
	d1 = 0.0
	s = 0.0
	for j in range(0, len(xx)):
		x = xx[j]
		y = yy[j]
		fx = a / (1.0 + math.exp(-b * x + c)) #+ d
		s += abs(y-fx)
		#d1 += (y-fx)
		a1 += (y-fx)*1.0/(1+math.exp(-b*x+c))
		b1 += (y-fx)*a*x /((1+math.exp(-b*x+c))**2)*math.exp(-b*x+c)
		c1 += (y-fx)*(-a)/((1+math.exp(-b*x+c))**2)*math.exp(-b*x+c)
	a += a1 * rate
	b += b1 * rate
	c += c1 * rate
	#d += d1 * rate
	print s
	

# for i in range(0, len(xx)):
# 	x = xx[i];
# 	fx = a / (1.0 + math.exp(-b * x + c))
# 	print yy[i]*350000

# print 

# for i in range(0, len(xx)):
# 	x = xx[i];
# 	fx = a / (1.0 + math.exp(-b * x + c))
# 	print fx*350000

	#print fx * 500000
# 2009
# 2010

# 285646
# 302859