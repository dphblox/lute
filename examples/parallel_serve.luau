local task = require("@std/task")
local vm = require("@lute/vm")

local threadCount = 8

for _ = 1, threadCount do
	task.spawn(vm.create("./parallel_serve_helper").serve)
end

print(`Created {threadCount} server threads`)
