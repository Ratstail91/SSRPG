print("Hello world from lua!")

local pagerlibs = require("region_pager")
local regionlibs = require("region")
local hashlibs = require("hash")

print("Pager libs:", pagerlibs)
print("Region libs:", regionlibs)
print("Hash libs:", hashlibs)

--expected arguments
local pager = ...

pagerlibs.SetOnCreate(pager, function(region)
	--lambda definition
	for i = 1, 20 do
		for j = 1, 20 do
			local t = hashlibs.coordhash(regionlibs.GetX(region)+i, regionlibs.GetY(region)+j, 1) --seed = 0 for now
			regionlibs.SetTile(region, i, j, 1, math.fmod(t, 5) * 36 + 14) --mapped to terrain.png
		end
	end

--	local t = hashlibs.coordhash(regionlibs.GetX(region), regionlibs.GetY(region), 1) --seed = 0 for now
--	print(t, math.fmod(t, 5), math.fmod(t, 5) * 36 + 14)
end)