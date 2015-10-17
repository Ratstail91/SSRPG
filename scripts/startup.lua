print("Hello world from lua!")

local pagerlibs = require("region_pager")
local regionlibs = require("region")
local toolslibs = require("map_tools")

--expected arguments
local pager = ...

--NOTE: mapped to overworld.png
pagerlibs.SetOnCreate(pager, function(region)
	print("Creating: ", regionlibs.GetX(region), regionlibs.GetY(region))

	--initial state
	toolslibs.generateRaw(region)

	--prune the odd structures
--	toolslibs.pruneRidges(region)

	--smooth out the edges
--	toolslibs.smoothEdges(region)
end)

pagerlibs.SetOnUnload(pager, function(region)
	print("Unloading: ", regionlibs.GetX(region), regionlibs.GetY(region))
end)