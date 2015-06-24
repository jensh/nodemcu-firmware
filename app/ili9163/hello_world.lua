
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
    disp:firstPage()
    repeat
	for x = 10, 120, 1 do
	    disp:setRGB(255-x, x, x)
	    disp:drawPixel(x, 60)
	end
	disp:setRGB(255, 255, 0)
	disp:drawStr(30, 30, "Hello World!")
    until disp:nextPage() == false

    print ("Ende")
end

start()


-- Local Variables:
--  compile-command: "nodemcu-uploader.py exec hello_world.lua"
-- End:
