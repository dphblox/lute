local system = require("@std/system")
local tableext = require("@std/tableext")
local task = require("@std/task")
local time = require("@std/time")

assert(type(tableext) == "table")
assert(type(task) == "table")
assert(type(time) == "table")
assert(type(system) == "table")

return { "successfully required @std modules" }
