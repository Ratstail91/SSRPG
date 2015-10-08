local libs = {}

local regionlibs = require("region")

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

--[[
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
function libs.bilinearInterpolation(x1, x2, y1, y2, q11, q12, q21, q22, i, j)
	--determine the size of the area
	local w = (x2 - x1)
	local h = (y2 - y1)

	--X axis
	local R1 = math.abs((w - i)/w) * q11 + math.abs(i/w) * q21
	local R2 = math.abs((w - i)/w) * q12 + math.abs(i/w) * q22

	--y axis
	local P = math.abs((h - j)/h) * R1 + math.abs(j/h) * R2

	return P
end

function libs.pruneRidges(r)
	--DOCS: http://kr-studios.tumblr.com/post/130724655872/so-in-theory-if-the-tiles-in-two-opposite
	for i = 1, regionlibs.GetWidth(r) do
		for j = 1, regionlibs.GetHeight(r) do
			--left edge, or left tile less than this tile
			if (i == 1 or regionlibs.GetTile(r, i - 1, j, 1) < regionlibs.GetTile(r, i, j, 1)) and
			--right edge, or right tile less than this tile
				(i == regionlibs.GetWidth(r) or regionlibs.GetTile(r, i + 1, j, 1) < regionlibs.GetTile(r, i, j, 1)) then
				regionlibs.SetTile(r, i, j, 1, regionlibs.GetTile(r, i, j, 1) - libs.margin)
			end

			--top edge, or top tile less than this tile
			if (j == 1 or regionlibs.GetTile(r, i, j - 1, 1) < regionlibs.GetTile(r, i, j, 1)) and
			--bottom edge, or bottom tile less than this tile
				(j == regionlibs.GetWidth(r) or regionlibs.GetTile(r, i, j + 1, 1) < regionlibs.GetTile(r, i, j, 1)) then
				--reduce this tile by 1 type
				regionlibs.SetTile(r, i, j, 1, regionlibs.GetTile(r, i, j, 1) - libs.margin)
			end
		end
	end
end

function libs.smoothEdges(r)
	--make and pad an array to use
	local shiftArray = {}
	for i = 1, regionlibs.GetWidth(r) do
		shiftArray[i] = {}
		for j = 1, regionlibs.GetHeight(r) do
			shiftArray[i][j] = 0
		end
	end

	--build the array
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