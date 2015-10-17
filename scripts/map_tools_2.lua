local libs = {}

local pagerlibs = require("region_pager")
local hashlibs = require("hash")

--utility functions
function libs.Sqr(x)
	return x*x
end
function libs.Dist(x, y, i, j)
	return math.sqrt(libs.Sqr(x - i) + libs.Sqr(y - j))
end

--tile macros, mapped to the tilesheet "overworld.png"
libs.margin	= 3 --distance between the center tiles of each set
libs.base	= 18 --distance to the center of the first tile set
libs.blank	= 0
libs.water	= 18 + libs.margin * 0
libs.sand	= 18 + libs.margin * 1
libs.plains	= 18 + libs.margin * 2
libs.grass	= 18 + libs.margin * 3
libs.dirt	= 18 + libs.margin * 4

--"edge" macros
libs.edges = {}
libs.edges.north = -16
libs.edges.south = 16
libs.edges.east = 1
libs.edges.west = -1

--[[ --TODO: (0) update this documentation
args:
	x1, x2, y1, y2		; positions used to find qAB with f(x, y)
	q11, q12, q21, q22	; f(xA, yB) = qAB
	i, j 				; tile indicies, relative to the region object
return:
	f(i, j)
notes:
	http://supercomputingblog.com/graphics/coding-bilinear-interpolation/
	IMPORTANT! the values of the x & y parameters are in a different domain space than i & j
--]]
function libs.bilinearInterpolation(q11, q12, q21, q22, w, h, i, j)
	--X axis
	local R1 = math.abs((w - i)/w) * q11 + math.abs(i/w) * q21
	local R2 = math.abs((w - i)/w) * q12 + math.abs(i/w) * q22

	--y axis
	local P = math.abs((h - j)/h) * R1 + math.abs(j/h) * R2

	return P
end

--NOTE: this assumes that 'array' has been initialized with the correct storage space
function libs.generateRaw(array, x1, x2, y1, y2, seed)
	--get f(xAB, yAB), range is 0 to 4 inclusive
	local q11 = math.fmod(hashlibs.coordhash(x1, y1, seed), 5)
	local q12 = math.fmod(hashlibs.coordhash(x1, y2, seed), 5)
	local q21 = math.fmod(hashlibs.coordhash(x2, y1, seed), 5)
	local q22 = math.fmod(hashlibs.coordhash(x2, y2, seed), 5)

	local w = x2 - x1
	local h = y2 - y1

	--define the tiles
	for i = 1, w do
		for j = 1, h do
			local t = libs.bilinearInterpolation(q11, q12, q21, q22, w, h, i, j)
			array[i][j][1] = math.ceil(t) * libs.margin + libs.base
		end
	end
end

--TODO: set the past-the-edge tiles

--assume the array has the past-the-edge tiles set
function libs.pruneRidges(array, w, h)
	for i = 1, w do
		for j = 1, h do
			--horizontal check
			if array[i][j][1] > array[i-1][j][1] and array[i][j][1] > array[i+1][j][1] then
				array[i][j][1] = array[i][j][1] - libs.margin
			end

			--vertical check check
			if array[i][j][1] > array[i][j-1][1] and array[i][j][1] > array[i][j+1][1] then
				array[i][j][1] = array[i][j][1] - libs.margin
			end
		end
	end
end

function libs.smoothEdges(array, w, h)
	--make and pad an array to use
	local shiftArray = {}
	for i = 1, w do
		shiftArray[i] = {}
		for j = 1, h do
			shiftArray[i][j] = 0
		end
	end

	--build the shift array
	--TODO: incorporate the neighbouring regions for smoothing
	for i = 1, regionlibs.GetWidth(r) do
		for j = 1, regionlibs.GetHeight(r) do
			--if (regionlibs edge) and (west tile < this tile), etc.
			if i > 1 and regionlibs.GetTile(r, i - 1, j, 1) < regionlibs.GetTile(r, i, j, 1) then
				shiftArray[i][j] = shiftArray[i][j] + libs.edges.west
			end
			if j > 1 and regionlibs.GetTile(r, i, j - 1, 1) < regionlibs.GetTile(r, i, j, 1) then
				shiftArray[i][j] = shiftArray[i][j] + libs.edges.north
			end
			if i < regionlibs.GetWidth(r) and regionlibs.GetTile(r, i + 1, j, 1) < regionlibs.GetTile(r, i, j, 1) then
				shiftArray[i][j] = shiftArray[i][j] + libs.edges.east
			end
			if j < regionlibs.GetHeight(r) and regionlibs.GetTile(r, i, j + 1, 1) < regionlibs.GetTile(r, i, j, 1) then
				shiftArray[i][j] = shiftArray[i][j] + libs.edges.south
			end
		end
	end

	--finally apply this
	for i = 1, regionlibs.GetWidth(r) do
		for j = 1, regionlibs.GetHeight(r) do
			if shiftArray[i][j] ~= 0 then
				regionlibs.SetTile(r, i, j, 2, regionlibs.GetTile(r, i, j, 1) + shiftArray[i][j])
				regionlibs.SetTile(r, i, j, 1, regionlibs.GetTile(r, i, j, 1) - libs.margin)
			end
		end
	end
end

return libs