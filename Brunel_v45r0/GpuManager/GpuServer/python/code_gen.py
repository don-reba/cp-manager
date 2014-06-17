import sys

def printHelp():
  print >> sys.stderr, 'syntax: <output-type> [<handler-type> <handler-name> <handler-header>]...'
  print >> sys.stderr, '<output-type>=inst produces instantiation code'
  print >> sys.stderr, '<output-type>=incl produces includes'

if len(sys.argv) < 2:
  printHelp()
  sys.exit(1)
  
outputType = sys.argv[1]

if outputType != 'inst' and outputType != 'incl':
  printHelp()
  sys.exit(1)

if (len(sys.argv) - 2) % 3 != 0:
  printHelp()
  sys.exit(1)
  
params = []
for i in range(2, len(sys.argv)-1, 3):
  t = sys.argv[i+0]
  n = sys.argv[i+1]
  p = sys.argv[i+2]
  params.append((t, n, p));

if outputType == 'inst':
  def toInst(arg):
    t, n, p = arg
    return 'make_pair(unique_ptr<IGpuHandler>(new {0}()), {1})'.format(t, n)
  body = '\n, '.join(map(toInst, params))
  print '{{ {0}\n}}'.format(body)

if outputType == 'incl':
  def toDecl(arg):
    t, n, p = arg
    return '#include "{0}"'.format(p)
  print '\n'.join(map(toDecl, params))
