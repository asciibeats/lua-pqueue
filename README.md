# lua-pqueue
*A priority queue lua module*
```lua
local pqueue = require('pqueue')
local pq = pqueue.new()

pq:enqueue(2, "hello")
pq:enqueue(1, 42)
pq:enqueue(3, {1, 2, 3})
pq:enqueue(4, 23)

local priority, value = pq:dequeue()

for priority, value in pq:pairs() do
  --something
end
```

## Build
```
git clone https://github.com/asciibeats/lua-pqueue.git
cd lua-pqueue
make
```

## Install
You need to be root. Installs to /usr/lib/lua/<lua_version>.
```
make install
```

## Demo
```
lua demo.lua
```
