import sys

nodesfn = sys.argv[1]
linksfn = sys.argv[2]

nodesf = file(nodesfn)
linksf = file(linksfn)

print "digraph finite_state_machine {"
print "node [shape = circle];"

# Descartar la primera linea
nodesf.readline()
for l in nodesf:
	node, name = l.split()
	print "\t", node, '[ label = "' + name + '"]'


# Descartar la primera linea
linksf.readline()
for l in linksf:

	n1, n2 = l.split()
	print "\t", n1, "->", n2



# Cerramos la llave de digraph
print "}"
