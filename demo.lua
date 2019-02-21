local pqueue = require('pqueue')
local q = pqueue.new(1)

local function enqueue(p, v)
  q:enqueue(p, v)
  print('enqueue\t', #q, p, v)
end

local function dequeue()
  local p, v = q:dequeue()
  print('dequeue\t', #q, p, v)
end

print('\t', 'count', 'prio', 'val')
enqueue(2, 6)
enqueue(1, 3)
q:purge()
print('purge\t', #q)
enqueue(5, "hello")
enqueue(9, 5)
enqueue(1, "world")
print('\ntostring', tostring(q), '\n')
print('\t', 'count', 'prio', 'val')
print('peek\t', #q, q:peek())
dequeue()
enqueue(3, 42)

for p, v in q:pairs() do
  print('dequeue\t', #q, p, v)
end
