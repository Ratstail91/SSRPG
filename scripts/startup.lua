print("Hello world from lua!")

local pagerlibs = require("region_pager")
local regionlibs = require("region")
local hashlibs = require("hash")
local toolslibs = require("map_tools")

--expected arguments
local pager = ...

--NOTE: mapped to overworld.png
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

	--define the tiles
	for i = 1, regionlibs.GetWidth(region) do
		for j = 1, regionlibs.GetHeight(region) do
			local t = toolslibs.bilinearInterpolation(x1, x2, y1, y2, q11, q12, q21, q22, i, j)
			regionlibs.SetTile(region, i, j, 1, math.floor(t) * toolslibs.margin + toolslibs.base)
		end
	end

--[[
	--DEBUG: testing smoothing algorithm
	if (regionlibs.GetX(region) == 0 and regionlibs.GetY(region) == 0) then
		regionlibs.SetTile(region, 10, 10, 1, 4 * 3 + 18)
		regionlibs.SetTile(region, 10, 11, 1, 4 * 3 + 18)
		regionlibs.SetTile(region, 11, 10, 1, 4 * 3 + 18)
		regionlibs.SetTile(region, 11, 11, 1, 4 * 3 + 18)

		regionlibs.SetTile(region, 13, 13, 1, 4 * 3 + 18)
	end
--]]

	--prune the odd structures
	toolslibs.pruneRidges(region)

	--smooth out the edges
	toolslibs.smoothEdges(region)
end)