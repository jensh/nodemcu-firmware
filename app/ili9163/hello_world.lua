
count = 0

function prepare()
	disp:setFont(u8g.font_6x10)
	disp:setFontRefHeightExtendedText()
	disp:setDefaultForegroundColor()
	disp:setFontPosTop()
end

function ccyle(p)
	local x,a,b
	p=p % 1536
	x=p % 512 a=x b=255
	if (x >= 256) then
		a=255
		b=511-x
	end
	if (p < 512) then
		disp:setRGB(b,a,0)
	elseif (p < 1024) then
		disp:setRGB(0,b,a)
	else
		disp:setRGB(a,0,b)
	end
end

function draw()
	for x = 0, 128 do
		ccyle(x * 22)
		for y = 35, 45 do
			disp:drawPixel(x, y)
		end
		tmr.wdclr()
	end
	disp:setRGB(255, 255, 255)
	disp:setFont(u8g.font_6x10)
	disp:drawStr(30, 15, "Hello World!")

	disp:drawStr(5, 60, "ESP8266-01 with")
	disp:drawStr(5, 70, "128*128px SPI")
	disp:drawStr(5, 80, "display (ILI9163c)")

	disp:setFont(u8g.font_chikita)
	disp:drawStr(0, 100, "https://github.com/jensh/")
	disp:drawStr(5, 107, "nodemcu-firmware/")
	disp:drawStr(5, 114, "tree/jh/ili9163c")
	
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

--start()
tmr.alarm(0, 5000, 0, start)

-- Local Variables:
--  compile-command: "nodemcu-uploader.py exec hello_world.lua"
-- End:
--  compile-command: "nodemcu-uploader.py upload hello_world.lua:init.lua"
