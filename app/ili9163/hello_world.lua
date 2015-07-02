
count = 0

function prepare()
	disp:setFont(u8g.font_6x10)
	disp:setFontRefHeightExtendedText()
	disp:setDefaultForegroundColor()
	disp:setFontPosTop()
end

function draw()
	for x = 10, 120, 1 do
		disp:setRGB(255-2*x, x, 2*x)
		for y = 60, 70 do
			disp:drawPixel(x, y)
		end
		tmr.wdclr()
	end
	disp:setRGB(255, 255, 0)
	disp:setFont(u8g.font_6x10)
	disp:drawStr(30, 30, "Hello World!")
	
	-- disp:drawStr(10, 100, ""..count)
	
	disp:setRGB(255, 128, 0)
	disp:setFont(u8g.font_chikita)
	disp:drawStr(90, 120, "- Jens")
end


function start()
	local x
	if (not disp) then
		disp = u8g.ili9163c(sla)
		prepare()
	end

	disp:setRot90()
	disp:firstPage()
	repeat
		draw()
	until disp:nextPage() == false
	count = count + 1
end

tmr.alarm(0, 5000, 1, start)

-- Local Variables:
--  compile-command: "nodemcu-uploader.py exec hello_world.lua"
-- End:
--  compile-command: "nodemcu-uploader.py upload hello_world.lua:init.lua"
