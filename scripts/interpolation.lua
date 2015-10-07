local libs = {}

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
function libs.bilinear(x1, x2, y1, y2, q11, q12, q21, q22, i, j)
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

return libs