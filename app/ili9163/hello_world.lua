
print (ili9163.hw)

function prepare()
     disp:setFont(u8g.font_6x10)
     disp:setFontRefHeightExtendedText()
     disp:setDefaultForegroundColor()
     disp:setFontPosTop()
end

function start()
    local x
    disp = u8g.ili9163c(sla)

    prepare()

    disp:setRot90()
    disp:firstPage()
    repeat
	for x = 10, 120, 1 do
	    disp:setRGB(255-2*x, x, 2*x)
	    disp:drawPixel(x, 60)
	    disp:drawPixel(x, 61)
	    disp:drawPixel(x, 62)
	end
	disp:setRGB(255, 255, 0)
	disp:drawStr(30, 30, "Hello World!")

	disp:setRGB(255, 128, 0)
	disp:setFont(u8g.font_chikita)
	disp:drawStr(60, 80, "- Jens")
	
    until disp:nextPage() == false

    print ("Ende")
end

start()


-- Local Variables:
--  compile-command: "nodemcu-uploader.py exec hello_world.lua"
-- End:
