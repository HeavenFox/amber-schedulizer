#!/usr/bin/env python
import sys

CARDINAL_SCALING_K = 20
CARDINAL_SCALING_B = 0
CARDINAL_SCALING_EXP = 1

PENALTY_SHORT_RECESS = -500
RECESS_FACTOR = 4

PENALTY_SAME_COLOR = -5
PENALTY_SAME_STYLE = -300

class Dance(object):
	def __init__(self):
		self.id = 0
		self.name = ''

		self.by = ''
		self.actress = set()
		self.color = ''
		self.recess_before = 0
		self.style = ''
		self.exp = '0'

	def get_cardinal_score(self, x):
		return int(eval(self.exp, {'x': x, 'k': CARDINAL_SCALING_K, 'b': CARDINAL_SCALING_B, 'e': CARDINAL_SCALING_EXP}))

dances = []

scores_p = {}
scores_c = {}

d = Dance()
current = 0
for line in open("dances.txt", "r"):
	l = line.strip()
	if l == '':
		d.id = len(dances)
		dances.append(d)
		d = Dance()
		current = 0
		continue

	if current == 0:
		d.name = l
	elif current == 1:
		d.by = l
	elif current == 2:
		d.actress = set(name.strip() for name in l.split(','))
	elif current == 3:
		d.color = l
	elif current == 4:
		d.recess_before = int(l)
	elif current == 5:
		d.style = l
	elif current == 6:
		d.exp = l

	current += 1

if len(sys.argv) > 1:
	print "Input numbers of show"
	r = raw_input()
	while (r != "done"):
		curr = 0
		ids = map(int, r.strip().split(" "))
		for i in ids:
			curr += 1
			print "%d\t%s (%s)" % (curr, dances[i].name, dances[i].by)
		r = raw_input()

	sys.exit(0)

# Print ID assignment
for d in dances:
	print "%d\t%s" % (d.id, d.name)

def must_same(a, b):
	return a == b and a != "unknown"

for i in xrange(len(dances)):
	for p in xrange(len(dances)):
		scores_c[i, p] = dances[i].get_cardinal_score(p)

	for j in xrange(len(dances)):
		if i == j:
			continue
		for k in xrange(1, len(dances)):
			score = 0
			# Recess
			if len(dances[i].actress & dances[j].actress) > 0:
				# minimum required
				if k-1 < dances[j].recess_before:
					score += PENALTY_SHORT_RECESS * (dances[j].recess_before - k + 1)
				else:
					score += RECESS_FACTOR * k

			# adjacent
			if k == 1:
				# clothing
				if must_same(dances[i].color, dances[j].color):
					score += PENALTY_SAME_COLOR
				# style
				if must_same(dances[i].style, dances[j].style):
					score += PENALTY_SAME_STYLE


			scores_p[i, j, k] = score

with open("positional.txt", "w") as f:
	for (i, j, k), l in scores_p.iteritems():
		f.write("%d %d %d %d\n" % (i,j,k,l))

with open("cardinal.txt", "w") as f:
	for (i, j), k in scores_c.iteritems():
		f.write("%d %d %d\n" % (i, j, k))
