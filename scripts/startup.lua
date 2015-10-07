print("Hello world from lua!")

local pagerlibs = require("region_pager")
local regionlibs = require("region")
local hashlibs = require("hash")
local smoothlibs = require("interpolation")

--expected arguments
local pager = ...

pagerlibs.SetOnCreate(pager, function(region)
	--get the coords; NOTE: this algorithm doesn't strictly require the region bounds
	local x1 = regionlibs.GetX(region)
	local x2 = regionlibs.GetX(region) + regionlibs.GetWidth(region)
	local y1 = regionlibs.GetY(region)
	local y2 = regionlibs.GetY(region) + regionlibs.GetHeight(region)

	--get f(xAB, yAB), range is 0 to 4 inclusive
	local q11 = math.fmod(hashlibs.coordhash(x1, y1, 1), 5)
	local q12 = math.fmod(hashlibs.coordhash(x1, y2, 1), 5)
	local q21 = math.fmod(hashlibs.coordhash(x2, y1, 1), 5)
	local q22 = math.fmod(hashlibs.coordhash(x2, y2, 1), 5)

	--mapped to terrain.png

	--lambda definition
	for i = 1, regionlibs.GetWidth(region) do
		for j = 1, regionlibs.GetHeight(region) do
			local t = smoothlibs.bilinear(x1, x2, y1, y2, q11, q12, q21, q22, i, j)
			regionlibs.SetTile(region, i, j, 1, math.floor(t) * 3 + 18)
		end
	end

--	local t = hashlibs.coordhash(regionlibs.GetX(region), regionlibs.GetY(region), 1) --seed = 0 for now
--	print(t, math.fmod(t, 5), math.fmod(t, 5) * 36 + 14)
end)