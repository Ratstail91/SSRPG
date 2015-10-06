local libs = {}

--[[
args:
	x1, x2, y1, y2		; positions used to find qAB with f(x, y)
	q11, q12, q21, q22	; f(xA, yB) = qAB
	i, j 				; find the value of f(i, j)
return:
	f(i, j)
notes:
	http://supercomputingblog.com/graphics/coding-bilinear-interpolation/
--]]
function libs.bilinear(x1, x2, y1, y2, q11, q12, q21, q22, i, j)
	--X axis
	local R1 = ((x2 - i)/(x2 - x1)) * q11 + (i - x1)/(x2 - x1) * q21
	local R2 = ((x2 - i)/(x2 - x1)) * q12 + (i - x1)/(x2 - x1) * q22

	--y axis
	local P = ((y2 - j)/(y2 - y1)) * R1 + ((j - y1)/(y2 - y1)) * R2

	return P
end

function libs.alt(x1, x2, y1, y2, q11, q12, q21, q22, i, j)
	--X axis
	local R1 = ((i - x1)/(x2 - x1)) * q11 + (x2 - i)/(x2 - x1) * q21
	local R2 = ((i - x1)/(x2 - x1)) * q12 + (x2 - i)/(x2 - x1) * q22

	--y axis
	local P = ((j - y1)/(y2 - y1)) * R1 + ((y2 - j)/(y2 - y1)) * R2

	return P
end

return libs