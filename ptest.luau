local ps = require("@lute/process")

local res = ps.run({"ls"})

print(res.stdout)
