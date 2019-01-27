# lua-pqueue
*A priority queue lua module*
```lua
local pqueue = require('pqueue')
local q = pqueue.new()

q:enqueue(2, "hello")
q:enqueue(1, 42)
q:enqueue(3, {1, 2, 3})
q:enqueue(4, 23)

local priority, value = q:dequeue()

for priority, value in q:pairs() do
  print(priority, value)
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
lua -v demo.lua
```
